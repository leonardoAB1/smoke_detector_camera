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

#ifdef ENABLE_BLE
#include "../ble_utils/ble_utils.h"
#endif

// Define the stack depth and priority for the Admin Control Task
#define TASK_MOTOR_ADMIN_CONTROL_STACK_DEPTH configMINIMAL_STACK_SIZE 
#define TASK_MOTOR_ADMIN_CONTROL_PRIORITY tskIDLE_PRIORITY+10
#define TASK_MOTOR_ADMIN_CONTROL_CORE 1

// Define the stack depth and priority for the Default Control Task
#define TASK_MOTOR_DEFAULT_CONTROL_STACK_DEPTH configMINIMAL_STACK_SIZE 
#define TASK_MOTOR_DEFAULT_CONTROL_PRIORITY tskIDLE_PRIORITY+10
#define TASK_MOTOR_DEFAULT_CONTROL_CORE 1

#if ENABLE_BLE
// Define the stack depth and priority for the BLE Notification Task
#define TASK_BLE_NOTIFICATION_STACK_DEPTH 1024*5 
#define TASK_BLE_NOTIFICATION_PRIORITY tskIDLE_PRIORITY+11
#define TASK_BLE_NOTIFICATION_CORE 0
#endif /* ENABLE_BLE */

#define MOTOR_ANGLES_QUEUE_ITEM_NUMBER 10  

#define ALPHA 0.1 // Smoothing factor for the EWMA
#define THRESHOLD 0.9 // Threshold for comparing with angle

//Struct to pass angles to Queue
typedef struct
{
    uint8_t angle1;
    uint8_t angle2;
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
	uint8_t TaskCore;					     /*< Task core (0 or 1)           */
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

#ifdef ENABLE_BLE
/**
 * @brief Task responsible for sending notifications through Bluetooth Low Energy (BLE).
 * 
 * This task handles the transmission of notifications using BLE technology. It is designed to run
 * continuously and send notifications to connected BLE devices.
 * 
 * @param pvParameters Pointer to the task parameters (if any).
 */
void BLENotificationTask(void *pvParameters) __attribute__((noreturn));
#endif /* ENABLE_BLE */
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
