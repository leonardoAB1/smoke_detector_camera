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

// Tag for logging purposes
static const char *TAG = "esp32-cam Webserver";

// Boundary for multipart/x-mixed-replace content type
#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

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
        .pin_sccb_sda = CAM_PIN_SIOD,
        .pin_sccb_scl = CAM_PIN_SIOC,
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

// HTTP request handler for streaming MJPEG
esp_err_t jpg_stream_httpd_handler(httpd_req_t *req){
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len;
    uint8_t * _jpg_buf;
    char * part_buf[64];
    static int64_t last_frame = 0;

    // Initialize last_frame if it's the first frame
    if(!last_frame) {
        last_frame = esp_timer_get_time();
    }

    // Set response content type to multipart/x-mixed-replace
    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK){
        return res;
    }

    // Continuously stream MJPEG frames
    while(true){
        // Capture frame from camera
        fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "Camera capture failed");
            res = ESP_FAIL;
            break;
        }

        // Check frame format and convert to JPEG if needed
        if(fb->format != PIXFORMAT_JPEG){
            bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
            if(!jpeg_converted){
                ESP_LOGE(TAG, "JPEG compression failed");
                esp_camera_fb_return(fb);
                res = ESP_FAIL;
            }
        } else {
            _jpg_buf_len = fb->len;
            _jpg_buf = fb->buf;
        }

        // Send MJPEG frame as a response chunk
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }
        if(res == ESP_OK){
            size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        }

        // Free the JPEG buffer if it was allocated
        if(fb->format != PIXFORMAT_JPEG){
            free(_jpg_buf);
        }
        esp_camera_fb_return(fb);

        // Check for any errors and calculate frame time
        if(res != ESP_OK){
            break;
        }
        int64_t fr_end = esp_timer_get_time();
        int64_t frame_time = fr_end - last_frame;
        last_frame = fr_end;
        frame_time /= 1000;

        // Log MJPEG frame information
        ESP_LOGI(TAG, "MJPG: %luKB %lums (%.1ffps)",
            (uint32_t)(_jpg_buf_len/1024),
            (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time);
    }

    last_frame = 0;
    return res;
}

// URI handler for HTTP GET requests
httpd_uri_t uri_get = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = jpg_stream_httpd_handler,
    .user_ctx = NULL
};

// Function to set up the HTTP server
httpd_handle_t setup_server(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t stream_httpd  = NULL;

    // Start the HTTP server
    if (httpd_start(&stream_httpd , &config) == ESP_OK)
    {
        // Register the URI handler for HTTP GET requests
        httpd_register_uri_handler(stream_httpd , &uri_get);
    }

    return stream_httpd;
}

// Entry point of the application
void app_main()
{
    esp_err_t err;

    // Initialize NVS (Non-Volatile Storage)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    // Connect to Wi-Fi
    connect_wifi();

    if (wifi_connect_status)
    {
        // Initialize the camera
        err = init_camera();
        if (err != ESP_OK)
        {
            printf("err: %s\n", esp_err_to_name(err));
            return;
        }

        // Set up the HTTP server
        setup_server();

        ESP_LOGI(TAG, "ESP32 CAM Web Server is up and running\n");
    }
    else {
        ESP_LOGI(TAG, "Failed to connect with Wi-Fi, check your network credentials\n");
    }
}
