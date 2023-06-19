/*******************************************************************************
 * @file        main.c
 * @author      Leonardo Acha Boiano
 * @date        27 May 2023
 * @brief       Main code of the Smoke Detector Camera Device
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include <esp_system.h>
#include <nvs_flash.h>
#include "esp_timer.h"

#include "connect_wifi/connect_wifi.h"
#include "camera/camera_utils.h"
#include "camera/camera_pins.h"
#include "gpio_utils/gpio_utils.h"
#include "motor_control/motor_control.h"
#include "web_server/web_server.c"
#include "http_handlers/http_handlers.h"
#include "task_utils/task_utils.h"

// Boundary for multipart/x-mixed-replace content type
#define PART_BOUNDARY "123456789000000000000987654321"
// Boundary for multipart/form-data content type
#define BOUNDARY "------------------------123456789000000000000987654321"

// Global variable to store the authenticated user's role
UserRole authenticatedUserRole;

// Entry point of the application
void app_main(void)
{
    // Initialize NVS (Non-Volatile Storage) to store WiFi configuration
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize the camera
    ESP_ERROR_CHECK(init_camera());

    // Initialize GPIO pins
    init_gpio();

    //Initialize FreeRTOS Tasks
    initialize_tasks();

    // Connect to WiFi
    connect_wifi();

    // Start the web server
    start_webserver();
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
