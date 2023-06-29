/*******************************************************************************
 * @file        task_utils.h
 * @author      Leonardo Acha Boiano
 * @date        13 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#ifndef _TASKS_H_
#define _TASKS_H_

#include "stdio.h"
#include "esp_task_wdt.h"
#include "../motor_control/motor_control.h"
#include "../logging/logging_utils.h"

// Define the stack depth and priority for the DecrementTask
#define TASK_MOTOR_ADMIN_CONTROL_STACK_DEPTH 2048
#define TASK_MOTOR_ADMIN_CONTROL_PRIORITY tskIDLE_PRIORITY+10

// Define the stack depth and priority for the Default Control Task
#define TASK_MOTOR_DEFAULT_CONTROL_STACK_DEPTH 2048
#define TASK_MOTOR_DEFAULT_CONTROL_PRIORITY tskIDLE_PRIORITY+10

#define MOTOR_ANGLES_QUEUE_ITEM_NUMBER 10  

//Struct to pass angles to Queue
typedef struct
{
    int angle1;
    int angle2;
} MotorAngles_t;

//Declare task params structure
typedef struct
{
    MotorAngles_t* motorAnglesQueue;
} TaskParams_t;

/**
 * @brief Task configuration structure used to create a task configuration table.
 * Note: this is for dynamic memory allocation. We create all the tasks up front
 * dynamically and then never allocate memory again after initialization.
 */
typedef struct
{
	TaskFunction_t const TaskCodePtr;		 /*< Pointer to the task function */
	const char *const TaskName;				 /*< String task name             */
	const configSTACK_DEPTH_TYPE StackDepth; /*< Stack depth                  */
	const void *const ParametersPtr;		 /*< Parameter Pointer            */
	UBaseType_t TaskPriority;				 /*< Task Priority                */
	TaskHandle_t *const TaskHandle;			 /*< Pointer to task handle       */
} TaskInitParams_t;

/**
 * @brief Initializes the tasks and creates the task table.
 * 
 */
void initialize_tasks(void);

/**
 * @brief Task  allows admins to control the motors freely through a REST API command.
 *        The motor angle is updated as requested and no longer follows the default pattern.
 * @note Admin user permissions are required to execute this function.
 * 
 * @param pvParameters Pointer to the task parameters.
 */
void MotorAdminControlTask(void* pvParameters) __attribute__((noreturn));

/**
 * @brief Task moves motor from HOME(MOTOR_1 and MOTOR_2 at 0) at a default pattern.
 * 
 * @param pvParameters Pointer to the task parameters.
 */
void MotorDefaultControlTask(void* pvParameters) __attribute__((noreturn));

extern Motor motor1;
extern Motor motor2;

/**
 * @param task_handle: The task handle name
 * @param stack_allotment:  How much stack space did you allocate to it when you created it
 *
 * Returns: float with the % of stacke used
 * Example:   printf("Stack Used %04.1f%%\r\n", GetTaskHighWaterMarkPercent(xTask1, 2048) );
 *Notes:
 */
float GetTaskHighWaterMarkPercent( TaskHandle_t task_handle, uint32_t stack_allotment );

#endif /* _TASKS_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
