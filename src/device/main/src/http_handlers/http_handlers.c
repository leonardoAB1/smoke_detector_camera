/*******************************************************************************
 * @file        http_handlers.c
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include "../http_handlers/http_handlers.h"

// HTTP request handler for getting a single image base64 encoded
esp_err_t image_base64_httpd_handler(httpd_req_t *req)
{
    // Authenticate the user
    UserRole authenticatedUserRole = authenticateUser(req);

    if (authenticatedUserRole != ROLE_USER && authenticatedUserRole != ROLE_ADMIN) {
        httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, "Unauthorized");
        return ESP_OK;
    }

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(CAMERA_TAG, "Camera capture failed");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Internal Server Error");
        return ESP_FAIL;
    }

    // Set the content type header
    httpd_resp_set_type(req, "image/jpeg");

    // Calculate the size of the base64-encoded data
    size_t encoded_len = (fb->len + 2) / 3 * 4;

    char *encoded_data = (char *)malloc(encoded_len);
    if (!encoded_data) {
        ESP_LOGE(CAMERA_TAG, "Memory allocation failed");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Internal Server Error");
        esp_camera_fb_return(fb);
        return ESP_FAIL;
    }

    // Encode the image data using base64
    size_t encoded_data_len = encoded_len;
    int ret = base64_encode((const unsigned char *)fb->buf, fb->len, encoded_data, &encoded_data_len);
    if (ret != 0) {
        ESP_LOGE(CAMERA_TAG, "Base64 encoding failed. Error code: %d", ret);
        ESP_LOGE(CAMERA_TAG, "Encoded data length: %d", encoded_data_len);
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
        ESP_LOGE(CAMERA_TAG, "Camera capture failed");
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
    ESP_LOGI(ADMIN_TAG, "Value updated: %d", value);

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


/********************************* END OF FILE ********************************/
/******************************************************************************/
