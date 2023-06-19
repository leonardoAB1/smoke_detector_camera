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
		result = xTaskCreate(TaskInitParameters[TaskCount].TaskCodePtr,
							 TaskInitParameters[TaskCount].TaskName,
							 TaskInitParameters[TaskCount].StackDepth,
							 (void*)TaskInitParameters[TaskCount].ParametersPtr,
							 TaskInitParameters[TaskCount].TaskPriority,
							 TaskInitParameters[TaskCount].TaskHandle);
		configASSERT(result == pdPASS); // Make sure the task was created successfully
	}	
}

void MotorAdminControlTask(void *pvParameters)
{
    TaskParams_t *params = (TaskParams_t *)pvParameters;

    while (1)
    {
        MotorAngles_t angles;
        //angles.angle1 = 45;
        //angles.angle2 = 90;
        //xQueueSend(params->motorAnglesQueue, &angles, portMAX_DELAY);

        // Rest of the task code
    }
}


void MotorDefaultControlTask(void *pvParameters)
{
	// Cast the task parameters
    TaskParams_t *params = (TaskParams_t *)pvParameters;

	while (1)
	{		
        //Run motor behavior
		move_motor(MOTOR_1, getDutyCycleFromAngle(0));
		move_motor(MOTOR_2, getDutyCycleFromAngle(0));

		move_motor(MOTOR_1, getDutyCycleFromAngle(90));
		move_motor(MOTOR_2, getDutyCycleFromAngle(90));

		move_motor(MOTOR_1, getDutyCycleFromAngle(180));
		move_motor(MOTOR_2, getDutyCycleFromAngle(180));
	}
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
