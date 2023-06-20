/*******************************************************************************
 * @file        gpio_interrupts.h
 * @author      Leonardo Acha Boiano
 * @date        20 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#ifndef GPIO_INTERRUPTS_H_
#define GPIO_INTERRUPTS_H_

#include "../camera/camera_pins.h"
#include <stdint.h>
#include "esp_intr_alloc.h"
#include "esp_attr.h"
#include "driver/gpio.h"

// Function declarations for the interrupt service routines
void IRAM_ATTR pir_signal_isr(void* arg);
void IRAM_ATTR ldr_isr(void* arg);
void IRAM_ATTR smoke_sensor_isr(void* arg);

// Getter functions for sensors' state
uint8_t getPirState(void);
uint8_t getLdrState(void);
uint8_t getSmokeSensorState(void);

#endif /* GPIO_INTERRUPTS_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
