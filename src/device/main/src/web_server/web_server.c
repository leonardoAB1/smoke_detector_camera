/*******************************************************************************
 * @file        web_server.c
 * @brief       Web Server Implementation
 * @details     This file contains the implementation of a web server for an ESP32-CAM development board.
 *              It provides functions to start the web server and register URI handlers.
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 *
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include "../web_server/web_server.h"

// Function to start the web server
void start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    //config.core_id = 0;  // Set the core ID to 0 (core 0)
    config.max_uri_handlers = 20;
    //config.task_priority    = tskIDLE_PRIORITY+1;
    //config.uri_match_fn     = httpd_uri_match_wildcard;

    // Start the HTTP server
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Set URI handlers
        httpd_uri_t image_uri = {
            .uri = "/image",
            .method = HTTP_GET,
            .handler = image_httpd_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &image_uri);

        httpd_uri_t image64_uri = {
            .uri = "/image64",
            .method = HTTP_GET,
            .handler = image_base64_httpd_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &image64_uri);

        httpd_uri_t status_uri = {
            .uri = "/status",
            .method = HTTP_GET,
            .handler = status_httpd_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &status_uri);

        httpd_uri_t admin_uri = {
            .uri = "/admin",
            .method = HTTP_POST,
            .handler = admin_httpd_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &admin_uri);

        httpd_uri_t motor_01_set_uri = {
            .uri = "/motor_01/set",
            .method = HTTP_POST,
            .handler = handle_set_motor_angle, //needs motor number as a header
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &motor_01_set_uri);

        httpd_uri_t motor_01_get_uri = {
            .uri = "/motor_01/get",
            .method = HTTP_GET,
            .handler = handle_get_motor_angle, //needs motor number as a header
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &motor_01_get_uri);

        httpd_uri_t motor_02_set_uri = {
        .uri = "/motor_02/set",
        .method = HTTP_POST,
        .handler = handle_set_motor_angle,
        .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &motor_02_set_uri);

        httpd_uri_t motor_02_get_uri = {
            .uri = "/motor_02/get",
            .method = HTTP_GET,
            .handler = handle_get_motor_angle,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &motor_02_get_uri);

        httpd_uri_t ldr_set_uri = {
            .uri = "/ldr/set",
            .method = HTTP_POST,
            .handler = handle_set_ldr,  //for testing purposes
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &ldr_set_uri);

        httpd_uri_t ldr_get_uri = {
            .uri = "/ldr/get",
            .method = HTTP_GET,
            .handler = handle_get_ldr,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &ldr_get_uri);
        
        httpd_uri_t led_set_uri = {
            .uri = "/led/set",
            .method = HTTP_POST,
            .handler = handle_set_led, 
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &led_set_uri);

        httpd_uri_t led_get_uri = {
            .uri = "/led/get",
            .method = HTTP_GET,
            .handler = handle_get_led,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &led_get_uri);
        
        httpd_uri_t smoke_sensor_set_uri = {
            .uri = "/smoke_sensor/set",
            .method = HTTP_POST,
            .handler = handle_set_smoke_sensor, //for testing purposes
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &smoke_sensor_set_uri);

        httpd_uri_t smoke_sensor_get_uri = {
            .uri = "/smoke_sensor/get",
            .method = HTTP_GET,
            .handler = handle_get_smoke_sensor,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &smoke_sensor_get_uri);

        ESP_LOGI(WEBSERVER_TAG, "HTTP server started");
    }
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
