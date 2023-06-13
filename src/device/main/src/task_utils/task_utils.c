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

TaskHandle_t motorTask;    // Define motorTask globally
BaseType_t result;         // Define result globally
TaskParams_t taskParams;   // Define taskParams globally

void initialize_tasks(void)
{
	TaskInitParams_t const TaskInitParameters[] = {
		// Pointer to the Task function, Task String Name, The task stack depth, Parameter Pointer, Task priority, Task Handle
		{(TaskFunction_t)MotorControlTask, "motor_control_task", configMINIMAL_STACK_SIZE, NULL, 5, &motorTask}
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

void MotorControlTask(void *pvParameters)
{
	//Handles

	while (1)
	{		
        //
	}
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
