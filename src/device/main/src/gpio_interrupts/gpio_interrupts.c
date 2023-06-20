/*******************************************************************************
 * @file        gpio_interrupts.c
 * @author      Leonardo Acha Boiano
 * @date        20 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include "../gpio_interrupts/gpio_interrupts.h" // Create this header file in the same directory

volatile uint8_t ldrState = 0;  // Variable to store the LDR state
volatile uint8_t pirState = 0;
volatile uint8_t smokeState = 0;

// Function to check the GPIO's current state
void checkGPIOState(uint8_t currentState, volatile uint8_t* stateVariable)
{
    switch (currentState)
    {
        case 0:
            *stateVariable = 0; // GPIO state changed from 1 to 0
            break;
        case 1:
            *stateVariable = 1; // GPIO state changed from 0 to 1
            break;
    }
}

// ISR for GPIO_PIR_SIGNAL pin
void IRAM_ATTR pir_signal_isr(void* arg)
{
    checkGPIOState(gpio_get_level(GPIO_PIR_SIGNAL), &pirState);
}

// ISR for GPIO_LDR pin
void IRAM_ATTR ldr_isr(void* arg)
{
    checkGPIOState(gpio_get_level(GPIO_LDR), &ldrState);
}

// ISR for GPIO_SMOKE_SENSOR pin
void IRAM_ATTR smoke_sensor_isr(void* arg)
{
    checkGPIOState(gpio_get_level(GPIO_SMOKE_SENSOR), &smokeState);
}

// Getter functions for sensors' state
uint8_t getPirState(void)
{
    return pirState;
}

uint8_t getLdrState(void)
{
    return ldrState;
}

uint8_t getSmokeSensorState(void)
{
    return smokeState;
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
