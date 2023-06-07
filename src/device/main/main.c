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
#include <stdint.h>
#include <string.h>

// Tag for logging purposes
static const char *TAG = "esp32-cam Webserver";

// Boundary for multipart/x-mixed-replace content type
#define PART_BOUNDARY "123456789000000000000987654321"
// Boundary for multipart/form-data content type
#define BOUNDARY "------------------------123456789000000000000987654321"

// Frequency of XCLK pin of the camera
#define CONFIG_XCLK_FREQ 20000000 

// User roles
typedef enum {
    ROLE_USER,
    ROLE_ADMIN,
    ROLE_UNKNOWN
} UserRole;

// Global variable to store the authenticated user's role
UserRole authenticatedUserRole;

// Function to authenticate the user and extract the role from the request headers
UserRole authenticateUser(httpd_req_t *req)
{
    // Extract the value of the X-User-Role header
    char roleHeader[32];
    size_t headerLen = sizeof(roleHeader);
    if (httpd_req_get_hdr_value_str(req, "X-User-Role", roleHeader, headerLen) != ESP_OK) {
        // User role header not found
        return ROLE_UNKNOWN;
    }

    // Check if the role is "admin"
    if (strcmp(roleHeader, "admin") == 0) {
        return ROLE_ADMIN;
    }

    return ROLE_USER;
}

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

// Base64 encoding table
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Function to perform base64 encoding
int base64_encode(const unsigned char *src, size_t src_len, char *dst, size_t *dst_len)
{
    size_t i, j;
    size_t encoded_len = (src_len + 2) / 3 * 4;

    if (*dst_len < encoded_len) {
        *dst_len = encoded_len;
        return -1; // Destination buffer too small
    }

    for (i = 0, j = 0; i < src_len; i += 3, j += 4) {
        uint32_t octet_a = i < src_len ? src[i] : 0;
        uint32_t octet_b = i + 1 < src_len ? src[i + 1] : 0;
        uint32_t octet_c = i + 2 < src_len ? src[i + 2] : 0;

        dst[j] = base64_table[(octet_a >> 2) & 0x3F];
        dst[j + 1] = base64_table[((octet_a & 0x3) << 4) | ((octet_b >> 4) & 0xF)];
        dst[j + 2] = i + 1 < src_len ? base64_table[((octet_b & 0xF) << 2) | ((octet_c >> 6) & 0x3F)] : '=';
        dst[j + 3] = i + 2 < src_len ? base64_table[octet_c & 0x3F] : '=';
    }

    *dst_len = encoded_len;
    return 0;
}

// HTTP request handler for getting a single image base64 encoded
esp_err_t image_base64_httpd_handler(httpd_req_t *req)
{
    // Authenticate the user
    authenticatedUserRole = authenticateUser(req);

    if (authenticatedUserRole != ROLE_USER && authenticatedUserRole != ROLE_ADMIN) {
        httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, "Unauthorized");
        return ESP_OK;
    }

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Internal Server Error");
        return ESP_FAIL;
    }

    // Set the content type header
    httpd_resp_set_type(req, "image/jpeg");

    // Calculate the size of the base64-encoded data
    size_t encoded_len = (fb->len + 2) / 3 * 4;

    char *encoded_data = (char *)malloc(encoded_len);
    if (!encoded_data) {
        ESP_LOGE(TAG, "Memory allocation failed");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Internal Server Error");
        esp_camera_fb_return(fb);
        return ESP_FAIL;
    }

    // Encode the image data using base64
    size_t encoded_data_len = encoded_len;
    int ret = base64_encode((const unsigned char *)fb->buf, fb->len, encoded_data, &encoded_data_len);
    if (ret != 0) {
        ESP_LOGE(TAG, "Base64 encoding failed. Error code: %d", ret);
        ESP_LOGE(TAG, "Encoded data length: %d", encoded_data_len);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Internal Server Error");
        free(encoded_data);
        esp_camera_fb_return(fb);
        return ESP_FAIL;
    }

    // Send the encoded image data as the response
    httpd_resp_send(req, encoded_data, encoded_data_len);

    // Cleanup
    free(encoded_data);
    esp_camera_fb_return(fb);

    return ESP_OK;
}

// HTTP request handler for getting a single image
esp_err_t image_httpd_handler(httpd_req_t *req)
{
    // Authenticate the user
    authenticatedUserRole = authenticateUser(req);

    if (authenticatedUserRole != ROLE_USER && authenticatedUserRole != ROLE_ADMIN) {
        httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, "Unauthorized");
        return ESP_OK;
    }

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Internal Server Error");
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
    // Authenticate the user
    authenticatedUserRole = authenticateUser(req);

    if (authenticatedUserRole != ROLE_USER && authenticatedUserRole != ROLE_ADMIN) {
        httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, "Unauthorized");
        return ESP_OK;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "status", "online");

    char *response = cJSON_Print(root);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, strlen(response));

    cJSON_Delete(root);
    free(response);

    return ESP_OK;
}

// HTTP request handler for admin functionality
esp_err_t admin_httpd_handler(httpd_req_t *req)
{
    // Authenticate the user
    authenticatedUserRole = authenticateUser(req);

    if (authenticatedUserRole != ROLE_ADMIN) {
        httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, "Unauthorized");
        return ESP_OK;
    }

    // Check if the request method is POST
    if (req->method != HTTP_POST) {
        httpd_resp_send_err(req, HTTPD_405_METHOD_NOT_ALLOWED, "Method Not Allowed");
        return ESP_OK;
    }

    // Get the content length
    char content_length_str[16];
    if (httpd_req_get_hdr_value_str(req, "Content-Length", content_length_str, sizeof(content_length_str)) != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Bad Request");
        return ESP_OK;
    }

    size_t content_len = atoi(content_length_str);

    // Read the request content data
    char *content = malloc(content_len + 1);
    if (!content) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Internal Server Error");
        return ESP_OK;
    }

    int ret = httpd_req_recv(req, content, content_len);
    if (ret <= 0) {
        free(content);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Bad Request");
        return ESP_OK;
    }

    content[ret] = '\0'; // Null-terminate the content

    // Parse the JSON content
    cJSON *json = cJSON_Parse(content);
    if (!json) {
        free(content);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
        return ESP_OK;
    }

    // Get the value from the JSON object
    cJSON *valueObj = cJSON_GetObjectItem(json, "value");
    if (!valueObj || !cJSON_IsNumber(valueObj)) {
        cJSON_Delete(json);
        free(content);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
        return ESP_OK;
    }

    int value = valueObj->valueint;

    // Log the updated value
    ESP_LOGI(TAG, "Value updated: %d", value);

    // Process the value (e.g., perform admin action)
    // ...

    // Cleanup
    cJSON_Delete(json);
    free(content);

    // Send a response
    httpd_resp_set_type(req, "text/plain");
    httpd_resp_send(req, "Value updated successfully", -1);

    return ESP_OK;
}

// Function to start the web server
static void start_webserver(void)
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

        ESP_LOGI(TAG, "HTTP server started");
    }
}

// Entry point of the application
void app_main(void)
{
    // Initialize NVS (Non-Volatile Storage) to store WiFi configuration
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Connect to WiFi
    connect_wifi();

    // Initialize the camera
    ESP_ERROR_CHECK(init_camera());

    // Start the web server
    start_webserver();
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
