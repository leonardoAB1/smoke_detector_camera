/*******************************************************************************
 * @file        logging_utils.h
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include "../logging/logging_utils.h"

const char *CAMERA_TAG = "Camera";
const char *WEBSERVER_TAG = "Webserver";
const char *ADMIN_TAG = "Admin";
const char *WIFI_TAG = "Connect Wifi";
const char *TASK_LOG_TAG = "Task Log";
const char *INTERRUPT_LOG_TAG = "Interrupt Log";
const char *GPIO_TAG = "GPIO Log";
const char *GPIO_STATES_TAG = "GPIO States Log";
const char *MOTOR_TAG = "Motor Log";

#if ENABLE_BT
const char *BT_TAG = "Bluetooth Log";

void bt_log_output(const char *fmt, va_list args)
{
    char buffer[BT_TX_BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    bt_printf("%s", buffer); //setup serial output communicate by bt
    // ALWAYS Write to stdout
    printf("%s", buffer);
}
#endif /* ENABLE_BT */


#if ENABLE_BLE
const char *BLE_TAG = "BLE-Server";

void ble_log_output(const char *fmt, va_list args)
{
    char buffer[MAX_NOTIFICATION_LEN];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    ble_printf("%s", buffer); //setup serial output communicate by ble
}
#endif /* ENABLE_BLE */

/********************************* END OF FILE ********************************/
/******************************************************************************/
