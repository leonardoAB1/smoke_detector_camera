/*******************************************************************************
 * @file        http_handlers.h
 * @brief       Header file containing declarations of HTTP request handlers.
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#ifndef HTTP_HANDLERS_H
#define HTTP_HANDLERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_log.h>
#include <esp_camera.h>
#include <cJSON.h>
#include <esp_http_server.h>

#include "../logging/logging_utils.h"
#include "../user_roles/user_roles.h"
#include "../base64/base64_utils.h"

/**
 * @brief       HTTP request handler for getting a single image in base64 encoding.
 * @param[in]   req The HTTP request object.
 * @return      An esp_err_t indicating the success or failure of the operation.
 */
esp_err_t image_base64_httpd_handler(httpd_req_t *req);

/**
 * @brief       HTTP request handler for getting a single image.
 * @param[in]   req The HTTP request object.
 * @return      An esp_err_t indicating the success or failure of the operation.
 */
esp_err_t image_httpd_handler(httpd_req_t *req);

/**
 * @brief       HTTP request handler for getting the camera status.
 * @param[in]   req The HTTP request object.
 * @return      An esp_err_t indicating the success or failure of the operation.
 */
esp_err_t status_httpd_handler(httpd_req_t *req);

/**
 * @brief       HTTP request handler for admin functionality.
 * @param[in]   req The HTTP request object.
 * @return      An esp_err_t indicating the success or failure of the operation.
 */
esp_err_t admin_httpd_handler(httpd_req_t *req);

/**
 * @brief       The authenticated user role.
 * @note        This variable is defined in main.c.
 */
extern UserRole authenticatedUserRole;

#endif  // HTTP_HANDLERS_H

/********************************* END OF FILE ********************************/
/******************************************************************************/
