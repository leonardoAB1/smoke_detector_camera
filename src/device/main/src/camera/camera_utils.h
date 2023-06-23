/*******************************************************************************
 * @file        camera_utils.h
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#ifndef CAMERA_UTILS_H
#define CAMERA_UTILS_H

#include "esp_camera.h"
#include "../camera/camera_pins.h"
#include "../user_roles/user_roles.h"

// Frequency of XCLK pin of the camera
#define CONFIG_XCLK_FREQ 15000000

esp_err_t init_camera(void);

#endif  // CAMERA_UTILS_H

/********************************* END OF FILE ********************************/
/******************************************************************************/
