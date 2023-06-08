/*******************************************************************************
 * @file        connect_wifi.h
 * @author      Leonardo Acha Boiano
 * @date        27 May 2023
 * @brief       Wifi related functions of the Smoke Detector Camera DIY camera
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#ifndef CONNECT_WIFI_H_
#define CONNECT_WIFI_H_

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <lwip/netdb.h>
#include "cJSON.h"

extern int wifi_connect_status;

esp_err_t connect_wifi(void);

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
