/*******************************************************************************
 * @file        logging_utils.h
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include "../logging/logging_utils.h"

const char *CAMERA_TAG = "esp32-cam Camera";
const char *WEBSERVER_TAG = "esp32-cam Webserver";
const char *ADMIN_TAG = "esp32-cam Admin";
const char *WIFI_TAG = "esp32-cam Connect Wifi";
const char *TASK_LOG_TAG = "esp32-cam Task Log";
const char *INTERRUPT_LOG_TAG = "esp32-cam Interrupt Log";
const char *GPIO_TAG = "esp32-cam GPIO Log";
const char *GPIO_STATES_TAG = "esp32-cam GPIO States Log";
const char *MOTOR_TAG = "esp32-cam Motor Log";

#if ENABLE_BLE
const char *BLE_TAG = "BLE-Server";

void bt_log_output(const char *fmt, va_list args)
{
    char buffer[MAX_NOTIFICATION_LEN];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    ble_printf("%s", buffer); //setup serial output communicate by ble
}

void setup_bt_logging()
{
    // Set the custom log output function
    esp_log_set_vprintf(bt_log_output);
}
#endif /* ENABLE_BLE */

/********************************* END OF FILE ********************************/
/******************************************************************************/
