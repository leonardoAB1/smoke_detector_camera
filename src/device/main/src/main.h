/*******************************************************************************
 * @file        main.h
 * @author      Leonardo Acha Boiano
 * @date        18 Jul 2023
 * @brief       Main code of the Smoke Detector Camera Device
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include "settings.h"

#include <esp_system.h>
#include <nvs_flash.h>

#if ENABLE_BT
#include "bt_utils/bt_utils.h"
#endif /* ENABLE_BT */

#if ENABLE_BLE
#include "ble_utils/ble_utils.h"
#endif /* ENABLE_BLE */

#include "connect_wifi/connect_wifi.h"
#include "camera/camera_utils.h"
#include "camera/camera_pins.h"
#include "gpio_utils/gpio_utils.h"
#include "motor_control/motor_control.h"
#include "web_server/web_server.c"
#include "http_handlers/http_handlers.h"
#include "motor_control/motor_control.h"
#include "task_utils/task_utils.h"
#include "gpio_interrupts/gpio_interrupts.h"
#include "logging/logging_utils.h"

// Global variable to store the authenticated user's role
extern UserRole authenticatedUserRole;

// Boundary for multipart/x-mixed-replace content type
#define PART_BOUNDARY "123456789000000000000987654321"
// Boundary for multipart/form-data content type
#define BOUNDARY "------------------------123456789000000000000987654321"

// Function prototypes
esp_err_t nvs_flash_init_custom(esp_err_t ret);

#endif /* MAIN_H */

/********************************* END OF FILE ********************************/
/******************************************************************************/
