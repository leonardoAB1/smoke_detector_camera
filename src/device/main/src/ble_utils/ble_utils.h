/*******************************************************************************
 * @file        ble_utils.h
 * @author      Leonardo Acha Boiano
 * @date        18 Jul 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#ifndef BLE_UTILS_H_
#define BLE_UTILS_H_

#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "host/ble_uuid.h"
#include "console/console.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include <stdarg.h>

#include "../ble_utils/bleprph.h"
#include "../logging/logging_utils.h"

#define MAX_NOTIFICATION_LEN 1024

extern uint16_t notification_handle;
extern uint16_t conn_handle;
extern bool notify_state;
extern char notification[MAX_NOTIFICATION_LEN];

/**
 * @brief Start the BLE operation and advertise the device.
 */
void startBLE();

/**
 * @brief Stop the BLE operation and deallocate resources.
 */
void stopBLE();

/**
 * @brief Send a printf-style formatted message over BLE.
 *
 * This function works similarly to printf. It accepts a format string and a variable number of arguments,
 * formats them, and sends the formatted message as a BLE notification.
 *
 * @param format The format string for the message.
 * @param ... The variable arguments to be formatted.
 */
void ble_printf(const char *format, ...);

/**
 * @brief Initialize the BLE storage configuration.
 */
void ble_store_config_init(void);

/**
 * @brief Handles GAP events for the peripheral device.
 *
 * This function is a callback that handles various GAP events like connection, disconnection,
 * and subscription changes.
 *
 * @param event The type of event being signalled.
 * @param arg Application-specified argument; unused by ble_utils.
 * @return 0 if the application successfully handled the event; nonzero on failure.
 */
static int bleprph_gap_event(struct ble_gap_event *event, void *arg);

/**
 * @brief Callback function to handle access to the custom characteristic.
 *
 * This function is called whenever the custom characteristic is accessed by a user,
 * either for reading or writing its value.
 *
 * @param conn_handle The connection handle of the user.
 * @param attr_handle The attribute handle of the characteristic.
 * @param ctxt Context information about the access request.
 * @param arg Application-specified argument; unused by ble_utils.
 * @return 0 on success, or an error code if the access request is invalid or fails.
 */
static int gatt_svr_chr_access(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *ctxt,
                               void *arg);

/**
 * @brief Callback function to handle writes to the custom characteristic.
 *
 * This function is called whenever a user writes to the custom characteristic.
 *
 * @param om Pointer to the mbuf containing the written data.
 * @param min_len The minimum allowed length for the written data.
 * @param max_len The maximum allowed length for the written data.
 * @param dst Pointer to the destination buffer to copy the written data.
 * @param len Pointer to store the actual length of the copied data.
 * @return 0 on success, or an error code if the write operation is invalid or fails.
 */
static int gatt_svr_chr_write(struct os_mbuf *om, uint16_t min_len, uint16_t max_len, void *dst, uint16_t *len);

/**
 * @brief Callback function to handle the BLE stack reset event.
 *
 * This function is called when the BLE stack is reset.
 *
 * @param reason The reason for the reset.
 */
static void bleprph_on_reset(int reason);

/**
 * @brief Main task function for the BLE host.
 *
 * This function runs the NimBLE stack.
 *
 * @param param Task parameter; unused by ble_utils.
 */
void bleprph_host_task(void *param);

/**
 * @brief Callback function to handle the BLE stack synchronization event.
 */
static void bleprph_on_sync(void);

/**
 * @brief Callback function to handle GATT service, characteristic, and descriptor registration events.
 *
 * @param ctxt Context information about the registration event.
 * @param arg Application-specified argument; unused by ble_utils.
 */
void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg);

/**
 * @brief Sends a notification to the connected client if subscribed.
 *
 * This function sends a notification to the connected client if the client has subscribed
 * to the notification handle. It checks the `notify_state` variable to ensure that the client
 * has subscribed before attempting to send the notification. If the client is not subscribed,
 * the function will do nothing.
 *
 * The notification data to be sent is assumed to be already set in the global variable "notification".
 * Ensure that the "notification" buffer is properly formatted and contains the data to be sent
 * before calling this function.
 *
 * @note Make sure to set the value of "notify_state" appropriately when handling client subscriptions.
 *       Otherwise, notifications will not be sent even if "notification" contains valid data.
 */
void sendNotification();

#endif /* BLE_UTILS_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
