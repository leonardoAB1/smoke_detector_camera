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

	TaskInitParams_t const TaskInitParameters[] = {
		// Pointer to the Task function, Task String Name, The task stack depth, Parameter Pointer, Task priority, Task Handle
		{(TaskFunction_t)MotorAdminControlTask, "motor_admin_control_task", TASK_MOTOR_ADMIN_CONTROL_STACK_DEPTH, NULL, TASK_MOTOR_ADMIN_CONTROL_PRIORITY, &motorAdminTask},
		{(TaskFunction_t)MotorDefaultControlTask, "motor_default_control_task", TASK_MOTOR_DEFAULT_CONTROL_STACK_DEPTH, NULL, TASK_MOTOR_DEFAULT_CONTROL_PRIORITY, &motorDefaultTask}
    };

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
                             1  // Set the core ID to 1 (core 1)
                             );
		configASSERT(result == pdPASS); // Make sure the task was created successfully
	}	
    ESP_LOGI(TASK_LOG_TAG, "Motor Tasks Initialized");
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
        // Reset the watchdog timer
        //esp_task_wdt_reset();

        MotorAngles_t angles;
        if (xQueueReceive((QueueHandle_t)params->motorAnglesQueue, &angles, portMAX_DELAY) == pdTRUE)
        {
            // Acquire motor1Mutex before accessing MOTOR_1
            xSemaphoreTake(motor1Mutex, portMAX_DELAY);
            move_motor(MOTOR_1, getDutyCycleFromAngle(angles.angle1));
            xSemaphoreGive(motor1Mutex);

            // Acquire motor2Mutex before accessing MOTOR_2
            xSemaphoreTake(motor2Mutex, portMAX_DELAY);
            move_motor(MOTOR_2, getDutyCycleFromAngle(angles.angle2));
            xSemaphoreGive(motor2Mutex);
        }

        // Delay between iterations to control the task execution rate
        vTaskDelay(pdMS_TO_TICKS(100));  // Delay for 100 milliseconds
    }
}

void MotorDefaultControlTask(void *pvParameters)
{
    while (1)
    {
        // Reset the watchdog timer
        //esp_task_wdt_reset();

        // Acquire motor1Mutex before accessing MOTOR_1
        xSemaphoreTake(motor1Mutex, portMAX_DELAY);
        move_motor(MOTOR_1, getDutyCycleFromAngle(0));
        xSemaphoreGive(motor1Mutex);

        vTaskDelete(NULL);

        // Acquire motor2Mutex before accessing MOTOR_2
        xSemaphoreTake(motor2Mutex, portMAX_DELAY);
        move_motor(MOTOR_2, getDutyCycleFromAngle(0));
        xSemaphoreGive(motor2Mutex);

        // Delay between iterations to control the task execution rate
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1 second
    }
}


/********************************* END OF FILE ********************************/
/******************************************************************************/
