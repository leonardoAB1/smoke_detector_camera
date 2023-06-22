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

esp_err_t init_isr(void)
{
    // Define the interrupt initialization parameters
    InterruptInitParams_t const InterruptInitParameters[] = {
        {GPIO_LDR, ldr_isr, NULL, GPIO_INTR_ANYEDGE},
        {GPIO_SMOKE_SENSOR, smoke_sensor_isr, NULL, GPIO_INTR_ANYEDGE},
        {GPIO_PIR_SIGNAL, pir_signal_isr, NULL, GPIO_INTR_ANYEDGE}
        // Add more interrupts here if needed
    };

    for (size_t i = 0; i < sizeof(InterruptInitParameters) / sizeof(InterruptInitParameters[0]); i++)
    {
        gpio_config_t gpioConfig;
        gpioConfig.pin_bit_mask = (1ULL << InterruptInitParameters[i].gpioNum);
        gpioConfig.mode = GPIO_MODE_DEF_INPUT;
        gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
        gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
        gpioConfig.intr_type = InterruptInitParameters[i].interruptType;

        esp_err_t err = gpio_config(&gpioConfig);
        if (err != ESP_OK) {
            ESP_LOGE(INTERRUPT_LOG_TAG, "GPIO config failed for pin %d with error 0x%x", InterruptInitParameters[i].gpioNum, err);
            return err;
        }

        err = gpio_isr_handler_add(InterruptInitParameters[i].gpioNum, InterruptInitParameters[i].isrHandler, InterruptInitParameters[i].userData);
        if (err != ESP_OK) {
            ESP_LOGE(INTERRUPT_LOG_TAG, "GPIO ISR handler add failed for pin %d with error 0x%x", InterruptInitParameters[i].gpioNum, err);
            return err;
        }
    }

    ESP_LOGI(INTERRUPT_LOG_TAG, "Interrupts Initialized");

    return ESP_OK;
}



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
void pir_signal_isr(void* arg)
{
    checkGPIOState(gpio_get_level(GPIO_PIR_SIGNAL), &pirState);
}

// ISR for GPIO_LDR pin
void ldr_isr(void* arg)
{
    checkGPIOState(gpio_get_level(GPIO_LDR), &ldrState);
    switch (ldrState)
    {
        case 0:
            gpio_set_level(GPIO_LEDs, LOW);
            break;
        case 1:
            gpio_set_level(GPIO_LEDs, HIGH);
            break;
    }
}

// ISR for GPIO_SMOKE_SENSOR pin
void smoke_sensor_isr(void* arg)
{
    checkGPIOState(gpio_get_level(GPIO_SMOKE_SENSOR), &smokeState);
}

// Getter functions for sensors' state
uint8_t getPirState(void)
{
    if (pirState != 0 && pirState != 1)
    {
        // Log an error or handle the error condition appropriately
        ESP_LOGE(INTERRUPT_LOG_TAG, "Invalid PIR state: %d", pirState);
    }

    return pirState;
}

uint8_t getLdrState(void)
{
    if (ldrState != 0 && ldrState != 1)
    {
        // Log an error or handle the error condition appropriately
        ESP_LOGE(INTERRUPT_LOG_TAG, "Invalid LDR state: %d", ldrState);
    }

    return ldrState;
}

uint8_t getSmokeSensorState(void)
{
    if (smokeState != 0 && smokeState != 1)
    {
        // Log an error or handle the error condition appropriately
        ESP_LOGE(INTERRUPT_LOG_TAG, "Invalid smoke sensor state: %d", smokeState);
    }
    
    return smokeState;
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
