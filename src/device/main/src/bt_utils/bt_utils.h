/*******************************************************************************
 * @file        bt_utils.h
 * @author      Leonardo Acha Boiano
 * @date        20 Jul 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#ifndef BT_UTILS_H_
#define BT_UTILS_H_

#include "settings.h"

#if ENABLE_BT

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"

#include "time.h"
#include "sys/time.h"

#include "../logging/logging_utils.h"

#define SPP_SERVER_NAME "SPP_SERVER"
#define DEVICE_NAME "SMC_BT"
#define SPP_SHOW_DATA 0
#define SPP_SHOW_SPEED 1
#define SPP_SHOW_MODE SPP_SHOW_DATA /*Choose show mode: show data or speed*/

#define CONFIG_CLASSIC_BT_ENABLED 1
#define CONFIG_BT_SPP_ENABLED 1

#define BT_TX_BUFFER_SIZE 256

/**
 * @brief Prints formatted data through Bluetooth.
 *
 * This function sends formatted data through Bluetooth using the Serial Port Profile (SPP).
 * It behaves similar to the standard `printf` function, but instead of printing to the console,
 * it sends the formatted data to the connected Bluetooth device.
 *
 * Before calling this function, ensure that a valid Bluetooth connection is established and
 * the `connection_handle` variable is properly set. If the connection handle is `NULL`, this
 * function will log an error and return without sending any data.
 *
 * @param format The format string containing placeholders for the data to be printed.
 * @param ...    Variable arguments to be formatted and printed according to the format string.
 *               These are the values that will replace the placeholders in the format string.
 *
 * @note The maximum length of the formatted data that can be sent over Bluetooth is determined
 *       by the BT_TX_BUFFER_SIZE constant defined in the bt_utils.h file. If the formatted data
 *       exceeds this size, it may be truncated.
 */
void bt_printf(const char *format, ...);

/**
 * @brief Initialize the Bluetooth functionality.
 * 
 * This function initializes the Bluetooth controller, bluedroid stack, registers
 * callback functions, and sets up parameters for secure pairing and legacy pairing.
 * 
 */
void init_bluetooth(esp_err_t ret);

/**
 * @brief Bluetooth GAP event callback.
 * 
 * This function is a callback to handle different events related to the Bluetooth
 * Generic Access Profile (GAP). It deals with authentication, pin requests, mode changes, etc.
 * 
 * @param event The Bluetooth GAP event type.
 * @param param The pointer to the GAP event parameters.
 */
void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);

/**
 * @brief Bluetooth SPP event callback.
 * 
 * This function is a callback to handle various events related to the Serial Port Profile (SPP)
 * in Bluetooth communication. Depending on the event type, it performs different actions,
 * such as initializing SPP, handling data received, and managing connection status.
 * 
 * @param event The Bluetooth SPP event type.
 * @param param The pointer to the SPP event parameters.
 */
static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

/**
 * @brief Print data transfer speed.
 * 
 * This function calculates the data transfer speed based on the time intervals between data receptions
 * and prints the result in kilobits per second.
 */
static void print_speed(void);

/**
 * @brief Convert Bluetooth device address to a string format.
 * 
 * This function takes a Bluetooth device address (BDA) represented as an array of bytes (uint8_t data type)
 * and converts it to a string format "XX:XX:XX:XX:XX:XX", where each "XX" represents a hexadecimal value of
 * one byte of the Bluetooth address.
 * 
 * @param bda  Pointer to the uint8_t array containing the Bluetooth device address.
 * @param str  Pointer to the character array where the resulting string will be stored.
 * @param size The size of the character array (str). It should be at least 18 characters (17 characters for the address + 1 for the null-terminator).
 * @return char* Pointer to the resulting string (str). It returns NULL if any of the parameters is invalid.
 */
static char *bda2str(uint8_t *bda, char *str, size_t size);

#endif /* ENABLE_BT */

#endif /* BT_UTILS_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
