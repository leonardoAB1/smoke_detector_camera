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
#include <stdarg.h>
#include "settings.h"

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

#if ENABLE_BT

#include "../bt_utils/bt_utils.h"

/**
 * @brief Tag for Bluetooth-related log messages
 */
extern const char *BT_TAG;

/**
 * @brief Logging function for Bluetooth output
 * @param fmt Format string for the log message
 * @param args Variable argument list
 */
void bt_log_output(const char *fmt, va_list args);

#endif /* ENABLE_BT */

#if ENABLE_BLE

#include "../ble_utils/ble_utils.h"

/**
 * @brief Tag for BLE-related log messages
 */
extern const char *BLE_TAG;

/**
 * @brief Logging function for Bluetooth Low Energy output
 * @param fmt Format string for the log message
 * @param args Variable argument list
 */
void ble_log_output(const char *fmt, va_list args);

#endif /* ENABLE_BLE */

#endif  // LOGGING_UTILS_H

/********************************* END OF FILE ********************************/
/******************************************************************************/
