/*******************************************************************************
 * @file        connect_wifi.c
 * @author      Leonardo Acha Boiano
 * @date        27 May 2023
 * @brief       Wifi related functions of the Smoke Detector Camera DIY camera
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include "connect_wifi.h"
#include "config.h"

int wifi_connect_status = 0;
static const char *TAG = "Connect_WiFi";
int s_retry_num = 0;

#define MAXIMUM_RETRY 5

/* FreeRTOS event group to signal when we are connected */
EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

/**
 * @brief Event handler function for WiFi events
 * 
 * @param arg Argument passed to the event handler
 * @param event_base Base of the event
 * @param event_id ID of the event
 * @param event_data Data associated with the event
 */
static esp_err_t wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
	  ESP_LOGI(TAG, "Wi-Fi client started");
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
	  ESP_LOGI(TAG, "Disconnected from Wi-Fi");
        if (s_retry_num < MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "Retrying to connect to AP (%d/%d)", s_retry_num, MAXIMUM_RETRY);
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        wifi_connect_status = 0;
        ESP_LOGE(TAG, "Connect to the AP fail");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        wifi_connect_status = 1;
    }
   return ESP_OK;
}

/**
 * @brief Function to connect to Wi-Fi
 */
esp_err_t connect_wifi(void)
{
    // Create event group
    s_wifi_event_group = xEventGroupCreate();

    // Initialize TCP/IP adapter
    ESP_ERROR_CHECK(esp_netif_init());

    // Initialize Wi-Fi
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    // Initialize Wi-Fi with default configuration
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Configure Wi-Fi with SSID and password
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    // Start Wi-Fi
    ESP_LOGI(TAG, "Starting Wi-Fi connection...");
    ESP_ERROR_CHECK(esp_wifi_start());

    // Wait until either the connection is successful or the maximum retries are reached
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    // Check the connection status
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "Connected to Wi-Fi");
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGE(TAG, "Failed to connect to Wi-Fi");
    }
    else
    {
        ESP_LOGE(TAG, "Unexpected event");
    }

    // Clean up the event group
    vEventGroupDelete(s_wifi_event_group);

    return ESP_OK;
}
