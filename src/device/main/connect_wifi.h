/*******************************************************************************
 * @file        connect_wifi.h
 * @author      Leonardo Acha Boiano
 * @date        27 May 2023
 * @brief       Wifi related functions of the Smoke Detector Camera DIY camera
 * 
 * @note        This code is written in C and is used on a ESP32-CAM development board.
 *
 *******************************************************************************/

#ifndef CONNECT_WIFI_H_
#define CONNECT_WIFI_H_

#include <esp_system.h>
#include <nvs_flash.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "driver/gpio.h"
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <lwip/netdb.h>

extern int wifi_connect_status;

void connect_wifi(void);

#endif