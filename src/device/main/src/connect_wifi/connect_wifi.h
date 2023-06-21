/*******************************************************************************
 * @file        connect_wifi.h
 * @brief       Wifi related functions for the Smoke Detector Camera DIY camera
 * @date        27 May 2023
 * @author      Leonardo Acha Boiano
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

#include "../logging/logging_utils.h"

#define MAXIMUM_RETRY 5

/**
 * @brief Connects to Wi-Fi.
 * 
 * @return esp_err_t Returns ESP_OK if the connection is successful, otherwise returns an error code.
 */
esp_err_t connect_wifi(void);

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/