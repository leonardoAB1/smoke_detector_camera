/*******************************************************************************
 * @file        web_server.h
 * @brief       Web Server Header File
 * @details     This file contains the function declaration for the web server.
 *              It includes the necessary headers and defines the start_webserver function.
 *              The web server is used on an ESP32-CAM development board.
 *              The HTTP server provides functionality to handle image requests, status requests, 
 *              and admin requests.
 *              The URI handlers for these requests are defined in the http_handlers.h file.
 *              The start_webserver function initializes and starts the web server.
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 *
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <esp_http_server.h>
#include "../http_handlers/http_handlers.h"
#include "../motor_control/motor_control.h"

/**
 * @brief Start the web server.
 *
 * This function initializes and starts the web server.
 * It configures the HTTP server and registers URI handlers for different routes.
 * The URI handlers for image, base64 image, status, and admin requests are registered.
 *
 * @return None.
 */
void start_webserver(void);

#endif  // WEB_SERVER_H

/********************************* END OF FILE ********************************/
/******************************************************************************/