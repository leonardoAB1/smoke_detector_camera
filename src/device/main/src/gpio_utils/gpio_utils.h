/*******************************************************************************
 * @file        gpio_utils.h
 * @author      Leonardo Acha Boiano
 * @date        11 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#ifndef GPIO_UTILS_H_
#define GPIO_UTILS_H_
#include "../camera/camera_pins.h"
#include "../motor_control/motor_control.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define HIGH    1
#define LOW     0

esp_err_t init_gpio(void);

#endif /* GPIO_UTILS_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
