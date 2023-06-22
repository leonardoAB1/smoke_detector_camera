/*******************************************************************************
 * @file        gpio_state.h
 * @author      Leonardo Acha Boiano
 * @date        22 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#ifndef GPIO_STATE_H_
#define GPIO_STATE_H_

#include "../gpio_interrupts/gpio_interrupts.h"
#include "../gpio_utils/gpio_utils.h"
#include "../logging/logging_utils.h"
#include <stdint.h>

// Function to check the GPIO's current state
void checkGPIOState(uint8_t currentState, volatile uint8_t* stateVariable);

// Getter functions for sensors' state
uint8_t getPirState(void);
uint8_t getLdrState(void);
uint8_t getSmokeSensorState(void);

// Setter functions for sensors' state
void setPirState(uint8_t state);
void setLdrState(uint8_t state);
void setSmokeSensorState(uint8_t state);

// Getter functions for led's state
uint8_t getLedState(void);

// Setter functions for led's state
void setLedState(uint8_t state);

#endif /* GPIO_STATE_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
