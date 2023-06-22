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
#include "../logging/logging_utils.h"
#include "../gpio_utils/gpio_utils.h"

/**
 * @brief Interrupt configuration structure used to initialize interrupts.
 */
typedef struct
{
    gpio_num_t gpioNum;                /*< GPIO number for the interrupt pin */
    gpio_isr_t isrHandler;             /*< Pointer to the interrupt service routine */
    void *userData;                    /*< User data to be passed to the ISR */
    gpio_int_type_t interruptType;     /*< Interrupt type (e.g., GPIO_INTR_ANYEDGE) */
} InterruptInitParams_t;

/**
 * @brief Initializes the interrupts.
 */
esp_err_t init_isr(void);

// Function declarations for the interrupt service routines
void pir_signal_isr(void* arg);
void ldr_isr(void* arg);
void smoke_sensor_isr(void* arg);

// Getter functions for sensors' state
uint8_t getPirState(void);
uint8_t getLdrState(void);
uint8_t getSmokeSensorState(void);

#endif /* GPIO_INTERRUPTS_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
