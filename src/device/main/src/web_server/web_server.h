/*******************************************************************************
 * @file        web_server.h
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

void start_webserver(void);

#endif  // WEB_SERVER_H

/********************************* END OF FILE ********************************/
/******************************************************************************/
