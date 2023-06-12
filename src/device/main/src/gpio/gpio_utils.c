/*******************************************************************************
 * @file        gpio_utils.c
 * @author      Leonardo Acha Boiano
 * @date        11 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#include "../gpio/gpio_utils.h"

void init_gpio(void) {
    // Initialize GPIO pins for servos as PWM outputs
    // Example:
    // gpio_set_direction(GPIO_SERVO1, GPIO_MODE_OUTPUT);
    // gpio_pad_select_gpio(GPIO_SERVO1);
    // gpio_set_level(GPIO_SERVO1, 0);
    
    // Initialize GPIO pins for LEDs as digital outputs
    
    // Initialize GPIO pins for LDR as digital input
    
    // Initialize GPIO pins for smoke sensor as digital input
    
    // Initialize GPIO pins for PIR signal as digital input
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
