/*******************************************************************************
 * @file        main.c
 * @author      Leonardo Acha Boiano
 * @date        27 May 2023
 * @brief       Main code of the Smoke Detector Camera Device
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include "main.h"

// Global variable to store the authenticated user's role
UserRole authenticatedUserRole;

// Entry point of the application
void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    // Initialize NVS (Non-Volatile Storage)
    nvs_flash_init_custom(ret);

    #if ENABLE_BT
    init_bluetooth(ret);
    // Set the custom log output function
    esp_log_set_vprintf(bt_log_output);
    #endif /* ENABLE_BT */
    
    #if ENABLE_BLE
    // Initialize Bluetooth Low Energy
    startBLE();
    snprintf(notification, 
            sizeof(notification), "Initial Notification");
    // Set the custom log output function
    esp_log_set_vprintf(ble_log_output);
    #endif /* ENABLE_BLE */

    // Initialize the camera
    ESP_ERROR_CHECK(init_camera());

    // Initialize GPIO pins
    init_gpio();

    //Init interrupt gpios
    init_isr();

    //Init motor structures
    initialize_motors();

    //Initialize FreeRTOS Tasks
    initialize_tasks();

    // Connect to WiFi
    connect_wifi();

    // Start the web server
    start_webserver();
}

esp_err_t nvs_flash_init_custom(esp_err_t ret){
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    return ESP_OK;
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
