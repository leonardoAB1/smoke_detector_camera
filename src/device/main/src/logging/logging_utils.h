/*******************************************************************************
 * @file        logging_utils.h
 * @brief       Header file containing logging utility functions and definitions
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 ******************************************************************************/

#ifndef LOGGING_UTILS_H
#define LOGGING_UTILS_H

#include <esp_log.h>

/**
 * @brief Tag for camera-related log messages
 */
extern const char *CAMERA_TAG;

/**
 * @brief Tag for webserver-related log messages
 */
extern const char *WEBSERVER_TAG;

/**
 * @brief Tag for admin-related log messages
 */
extern const char *ADMIN_TAG;

/**
 * @brief Tag for wifi-related log messages
 */
extern const char *WIFI_TAG;

/**
 * @brief Tag for task-realted log messages
 */
extern const char *TASK_LOG_TAG;

/**
 * @brief Tag for interrupt-related log messages
 */
extern const char *INTERRUPT_LOG_TAG;

/**
 * @brief Tag for gpio-related log messages
 */
extern const char *GPIO_TAG;

/**
 * @brief Tag for gpio-related log messages
 */
extern const char *GPIO_STATES_TAG;

/**
 * @brief Tag for motor-related log messages
 */
extern const char *MOTOR_TAG;

#endif  // LOGGING_UTILS_H

/********************************* END OF FILE ********************************/
/******************************************************************************/
