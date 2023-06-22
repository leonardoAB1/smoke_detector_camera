/*******************************************************************************
 * @file        gpio_state.c
 * @author      Leonardo Acha Boiano
 * @date        22 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#include "../gpio_state/gpio_state.h"

volatile uint8_t ledState = 0;    // Variable to store the LED state
volatile uint8_t ldrState = 0;    // Variable to store the LDR state
volatile uint8_t pirState = 0;    // Variable to store the PIR state
volatile uint8_t smokeState = 0;  // Variable to store the smoke sensor state

// Function to check the GPIO's current state
void checkGPIOState(uint8_t currentState, volatile uint8_t* stateVariable) {
    switch (currentState) {
        case 0:
            *stateVariable = 0;  // GPIO state changed from 1 to 0
            break;
        case 1:
            *stateVariable = 1;  // GPIO state changed from 0 to 1
            break;
    }
}

// Getter functions for LEDs' state
uint8_t getLedState(void) {
    if (ledState != 0 && ledState != 1) {
        // Log an error or handle the error condition appropriately
        ESP_LOGE(GPIO_STATES_TAG, "Invalid LED state: %d", ledState);
    }
    return ledState;
}

// Setter functions for LEDs' state
void setLedState(uint8_t state) {
    if (state != 0 && state != 1) {
        // Log an error or handle the error condition appropriately
        ESP_LOGE(GPIO_TAG, "Invalid LED state: %d", state);
        return;
    }
    ledState = state;
}

// Getter functions for sensors' state
uint8_t getPirState(void) {
    if (pirState != 0 && pirState != 1) {
        // Log an error or handle the error condition appropriately
        ESP_LOGE(GPIO_STATES_TAG, "Invalid PIR state: %d", pirState);
    }
    return pirState;
}

uint8_t getLdrState(void) {
    if (ldrState != 0 && ldrState != 1) {
        // Log an error or handle the error condition appropriately
        ESP_LOGE(GPIO_STATES_TAG, "Invalid LDR state: %d", ldrState);
    }
    return ldrState;
}

uint8_t getSmokeSensorState(void) {
    if (smokeState != 0 && smokeState != 1) {
        // Log an error or handle the error condition appropriately
        ESP_LOGE(GPIO_STATES_TAG, "Invalid smoke sensor state: %d", smokeState);
    }
    return smokeState;
}

// Setter functions for sensors' state
void setPirState(uint8_t state) {
    if (state != 0 && state != 1) {
        // Log an error or handle the error condition appropriately
        ESP_LOGE(INTERRUPT_LOG_TAG, "Invalid PIR state: %d", state);
        return;
    }
    pirState = state;
}

void setLdrState(uint8_t state) {
    if (state != 0 && state != 1) {
        // Log an error or handle the error condition appropriately
        ESP_LOGE(INTERRUPT_LOG_TAG, "Invalid LDR state: %d", state);
        return;
    }
    ldrState = state;

    // Update the LED state based on the LDR state
    if (ldrState == 0) {
        gpio_set_level(GPIO_LEDs, LOW);
    } else {
        gpio_set_level(GPIO_LEDs, HIGH);
    }
}

void setSmokeSensorState(uint8_t state) {
    if (state != 0 && state != 1) {
        // Log an error or handle the error condition appropriately
        ESP_LOGE(INTERRUPT_LOG_TAG, "Invalid smoke sensor state: %d", state);
        return;
    }
    smokeState = state;
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
