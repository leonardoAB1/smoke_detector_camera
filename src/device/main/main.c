/*******************************************************************************
 * @file        main.c
 * @author      Leonardo Acha Boiano
 * @date        27 May 2023
 * @brief       Main code of the Smoke Detector Camera Device
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include <esp_system.h>
#include <nvs_flash.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "esp_camera.h"
#include "esp_http_server.h"
#include "esp_timer.h"
#include "camera_pins.h"
#include "connect_wifi.h"

#include "cJSON.h"

// Tag for logging purposes
static const char *TAG = "esp32-cam Webserver";

// Boundary for multipart/x-mixed-replace content type
#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
// Boundary for multipart/form-data content type
#define BOUNDARY "------------------------123456789000000000000987654321"

// Frequency of XCLK pin of the camera
#define CONFIG_XCLK_FREQ 20000000 

// Function to initialize the camera
static esp_err_t init_camera(void)
{
    // Camera configuration structure
    camera_config_t camera_config = {
        // Camera pin configurations
        .pin_pwdn  = CAM_PIN_PWDN,
        .pin_reset = CAM_PIN_RESET,
        .pin_xclk = CAM_PIN_XCLK,
        .pin_sscb_sda = CAM_PIN_SIOD,  // Fixed typo: pin_sscb_sda
        .pin_sscb_scl = CAM_PIN_SIOC,  // Fixed typo: pin_sscb_scl
        .pin_d7 = CAM_PIN_D7,
        .pin_d6 = CAM_PIN_D6,
        .pin_d5 = CAM_PIN_D5,
        .pin_d4 = CAM_PIN_D4,
        .pin_d3 = CAM_PIN_D3,
        .pin_d2 = CAM_PIN_D2,
        .pin_d1 = CAM_PIN_D1,
        .pin_d0 = CAM_PIN_D0,
        .pin_vsync = CAM_PIN_VSYNC,
        .pin_href = CAM_PIN_HREF,
        .pin_pclk = CAM_PIN_PCLK,

        // Camera configuration parameters
        .xclk_freq_hz = CONFIG_XCLK_FREQ,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_VGA,
        .jpeg_quality = 10,
        .fb_count = 1,
        .grab_mode = CAMERA_GRAB_WHEN_EMPTY // Sets when buffers should be filled
    };

    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        return err;
    }
    return ESP_OK;
}

// HTTP request handler for getting a single image
esp_err_t image_httpd_handler(httpd_req_t *req)
{
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    // Set the content type header
    httpd_resp_set_type(req, "image/jpeg");

    // Send the image data
    httpd_resp_send(req, (const char *)fb->buf, fb->len);

    esp_camera_fb_return(fb);
    return ESP_OK;
}

// HTTP request handler for getting the camera status
esp_err_t status_httpd_handler(httpd_req_t *req)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "status", "online");

    char *response = cJSON_Print(root);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, strlen(response));

    cJSON_Delete(root);
    free(response);

    return ESP_OK;
}

// Function to start the web server
httpd_handle_t start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    // Start the httpd server
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Register URI handlers
        httpd_uri_t uri_image = {
            .uri = "/image",
            .method = HTTP_GET,
            .handler = image_httpd_handler,
            .user_ctx = NULL
        };

        httpd_uri_t uri_status = {
            .uri = "/status",
            .method = HTTP_GET,
            .handler = status_httpd_handler,
            .user_ctx = NULL
        };

        // Register the URI handlers
        httpd_register_uri_handler(server, &uri_image);
        httpd_register_uri_handler(server, &uri_status);

        return server;
    }

    ESP_LOGE(TAG, "Error starting server!");
    return NULL;
}

// Entry point of the application
void app_main(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK)
    {
        ESP_ERROR_CHECK(err);
    }

    // Initialize the camera
    err = init_camera();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Camera initialization failed with error 0x%x", err);
        return;
    }

    // Connect to Wi-Fi
    err = connect_wifi();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Wi-Fi connection failed with error %d", err);
        return;
    }

    // Start the web server
    httpd_handle_t server = start_webserver();
    if (server == NULL)
    {
        ESP_LOGE(TAG, "Failed to start web server");
        ESP_LOGI(TAG, "Restarting System...");
        esp_restart();
    }
    else
    {
        ESP_LOGI(TAG, "Camera web server started");
    }
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
