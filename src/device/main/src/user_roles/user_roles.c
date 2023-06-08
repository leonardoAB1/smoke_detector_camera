/*******************************************************************************
 * @file        user_roles.c
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include "../user_roles/user_roles.h"

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

/********************************* END OF FILE ********************************/
/******************************************************************************/
