/*******************************************************************************
 * @file        user_roles.h
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#ifndef USER_ROLES_H
#define USER_ROLES_H

#include <esp_http_server.h>

// User roles
typedef enum {
    ROLE_USER,
    ROLE_ADMIN,
    ROLE_UNKNOWN
} UserRole;

UserRole authenticateUser(httpd_req_t *req);

#endif  // USER_ROLES_H

/********************************* END OF FILE ********************************/
/******************************************************************************/
