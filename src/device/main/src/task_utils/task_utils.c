/*******************************************************************************
 * @file        task_utils.c
 * @author      Leonardo Acha Boiano
 * @date        13 Jun 2023
 * @brief       This file contains the implementation of the tasks created in FreeRTOS
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#include "../task_utils/task_utils.h"

BaseType_t result;         // Define result globally

TaskHandle_t motorAdminTask;    // Define task globally
TaskHandle_t motorDefaultTask;  // Define task globally

#if ENABLE_BLE
TaskHandle_t bleNotificationTask;  // Define task globally
#endif

SemaphoreHandle_t motor1Mutex;
SemaphoreHandle_t motor2Mutex;

QueueHandle_t motorAnglesQueue;

TaskParams_t taskParams;   // Define taskParams globally

void initialize_tasks(void)
{
	// Handle creation
	motor1Mutex = xSemaphoreCreateMutex();
	motor2Mutex = xSemaphoreCreateMutex();
	// xQueueCreate(items in queue,  item's size in bytes)
	motorAnglesQueue = xQueueCreate(MOTOR_ANGLES_QUEUE_ITEM_NUMBER, sizeof(MotorAngles_t));

	taskParams.motorAnglesQueue = (MotorAngles_t*)motorAnglesQueue;

#if ENABLE_BLE
	TaskInitParams_t const TaskInitParameters[] = {
		// Pointer to the Task function, Task String Name, The task stack depth, Parameter Pointer, Task priority, Task Handle
		{(TaskFunction_t)MotorAdminControlTask, "motor_admin_control_task", TASK_MOTOR_ADMIN_CONTROL_STACK_DEPTH, NULL, TASK_MOTOR_ADMIN_CONTROL_PRIORITY, &motorAdminTask, TASK_MOTOR_ADMIN_CONTROL_CORE},
		{(TaskFunction_t)MotorDefaultControlTask, "motor_default_control_task", TASK_MOTOR_DEFAULT_CONTROL_STACK_DEPTH, NULL, TASK_MOTOR_DEFAULT_CONTROL_PRIORITY, &motorDefaultTask, TASK_MOTOR_DEFAULT_CONTROL_CORE},
        {(TaskFunction_t)BLENotificationTask, "ble_notification_task", TASK_BLE_NOTIFICATION_STACK_DEPTH, NULL, TASK_BLE_NOTIFICATION_PRIORITY, &bleNotificationTask, TASK_BLE_NOTIFICATION_CORE}
    };
#else
	TaskInitParams_t const TaskInitParameters[] = {
		// Pointer to the Task function, Task String Name, The task stack depth, Parameter Pointer, Task priority, Task Handle
		{(TaskFunction_t)MotorAdminControlTask, "motor_admin_control_task", TASK_MOTOR_ADMIN_CONTROL_STACK_DEPTH, NULL, TASK_MOTOR_ADMIN_CONTROL_PRIORITY, &motorAdminTask, TASK_MOTOR_ADMIN_CONTROL_CORE},
		{(TaskFunction_t)MotorDefaultControlTask, "motor_default_control_task", TASK_MOTOR_DEFAULT_CONTROL_STACK_DEPTH, NULL, TASK_MOTOR_DEFAULT_CONTROL_PRIORITY, &motorDefaultTask, TASK_MOTOR_DEFAULT_CONTROL_CORE}
    };
#endif

	// Loop through the task table and create each task.
	for (size_t TaskCount = 0;
		 TaskCount < sizeof(TaskInitParameters) / sizeof(TaskInitParameters[0]);
		 TaskCount++)
	{
		result = xTaskCreatePinnedToCore(TaskInitParameters[TaskCount].TaskCodePtr,
							 TaskInitParameters[TaskCount].TaskName,
							 TaskInitParameters[TaskCount].StackDepth,
							 (void*)TaskInitParameters[TaskCount].ParametersPtr,
							 TaskInitParameters[TaskCount].TaskPriority,
							 TaskInitParameters[TaskCount].TaskHandle,
                             TaskInitParameters[TaskCount].TaskCore
                             );
		configASSERT(result == pdPASS); // Make sure the task was created successfully
	}	
    //ESP_LOGI(TASK_LOG_TAG, "Motor Tasks Initialized");
}

void MotorAdminControlTask(void *pvParameters)
{
    TaskParams_t *params = (TaskParams_t *)pvParameters;
    if (params == NULL)
    {
        // Handle the error or return early
        // For example, you can log an error message and delete the task
        vTaskDelete(NULL);
    }

    while (1)
    {
        MotorAngles_t angles;
        if (xQueueReceive((QueueHandle_t)params->motorAnglesQueue, &angles, portMAX_DELAY) == pdTRUE)
        {
            // Acquire motor1Mutex before accessing MOTOR_1
            xSemaphoreTake(motor1Mutex, portMAX_DELAY);
            if (motor1.is_active)
            {
                move_motor(&motor1, angles.angle1);
            }
            xSemaphoreGive(motor1Mutex);

            // Acquire motor2Mutex before accessing MOTOR_2
            xSemaphoreTake(motor2Mutex, portMAX_DELAY);
            if (motor2.is_active)
            {
                move_motor(&motor2, angles.angle2);
            }
            xSemaphoreGive(motor2Mutex);
        }

        // Delay between iterations to control the task execution rate
        vTaskDelay(pdMS_TO_TICKS(300));  // Delay for 100 milliseconds
    }
    vTaskDelete(NULL);
}

void MotorDefaultControlTask(void *pvParameters)
{
    uint8_t angle = 0;
    bool increasing = true;
    double ewma_angle = 0.0;  // Exponentially Weighted Moving Average

    while (1)
    {
        // Acquire motor1Mutex before accessing MOTOR_1
        xSemaphoreTake(motor1Mutex, portMAX_DELAY);
        if (motor1.is_active)
        {
            move_motor(&motor1, ewma_angle);
            set_motor_angle(&motor1, ewma_angle);
        }
        xSemaphoreGive(motor1Mutex);

        // Acquire motor2Mutex before accessing MOTOR_2
        xSemaphoreTake(motor2Mutex, portMAX_DELAY);
        if (motor2.is_active)
        {
            move_motor(&motor2, ewma_angle);
            set_motor_angle(&motor2, ewma_angle);
        }
        xSemaphoreGive(motor2Mutex);

        // Update the EWMA with the current angle change
        ewma_angle = ALPHA * angle + (1 - ALPHA) * ewma_angle;

        if (ewma_angle >= THRESHOLD * angle)
        {
            if (increasing)
            {
                if (angle < SERVO_MAX_ANGLE)
                    angle++;
                else
                    increasing = false;
            }
            else
            {
                if (angle > 0)
                    angle--;
                else
                    increasing = true;
            }
        }

        // Delay between iterations to control the task execution rate
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1000 milliseconds
    }

    vTaskDelete(NULL);
}

#if ENABLE_BLE
void BLENotificationTask(void *pvParameters) // For sending notifications periodically as freetos task(after setting value of variable"notification")
{
  int rc;
  struct os_mbuf *om;
  while (1)
  {
    if (notify_state) // This value is checked so that we don't send notifications if no one has subscribed to our notification handle.
    {
      om = ble_hs_mbuf_from_flat(notification, sizeof(notification));
      rc = ble_gatts_notify_custom(conn_handle, notification_handle, om);
      //ESP_LOGI(BLE_TAG,"rc=%d\n", rc);

      if (rc != 0)
      {
        ESP_LOGE(BLE_TAG,"Error notifying; rc\n");
      }
    }
    else
    {
      ESP_LOGI(BLE_TAG,"No one subscribed to notifications\n");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}
#endif /* ENABLE_BLE */

float GetTaskHighWaterMarkPercent( TaskHandle_t task_handle, uint32_t stack_allotment )
{
  UBaseType_t uxHighWaterMark;
  uint32_t diff;
  float result;

  uxHighWaterMark = uxTaskGetStackHighWaterMark( task_handle );

  diff = stack_allotment - uxHighWaterMark;

  result = ( (float)diff / (float)stack_allotment ) * 100.0;

  return result;
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
