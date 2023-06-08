/*******************************************************************************
 * @file        web_server.c
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include "web_server.h"

// Function to start the web server
void start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

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

        ESP_LOGI(WEBSERVER_TAG, "HTTP server started");
    }
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
