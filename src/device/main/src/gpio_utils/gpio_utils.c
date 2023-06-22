/*******************************************************************************
 * @file        gpio_utils.c
 * @author      Leonardo Acha Boiano
 * @date        11 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#include "../gpio_utils/gpio_utils.h"

esp_err_t init_gpio(void) {
    // Initialize GPIO pins for servos as PWM outputs
    // Motor1
    ledc_channel_config_t channelConfigMotor1 = {0};
    channelConfigMotor1.gpio_num     = GPIO_SERVO_1;
    channelConfigMotor1.speed_mode   = LEDC_HIGH_SPEED_MODE;
    channelConfigMotor1.channel      = LEDC_CHANNEL_0;
    channelConfigMotor1.intr_type    = LEDC_INTR_DISABLE;
    channelConfigMotor1.timer_sel    = LEDC_TIMER_1;
    channelConfigMotor1.duty         = 0; //Initial Duty Cycle
    ledc_channel_config(&channelConfigMotor1);
    
    // Motor2
    ledc_channel_config_t channelConfigMotor2 = {0};
    channelConfigMotor2.gpio_num     = GPIO_SERVO_2;
    channelConfigMotor2.speed_mode   = LEDC_HIGH_SPEED_MODE;
    channelConfigMotor2.channel      = LEDC_CHANNEL_1;
    channelConfigMotor2.intr_type    = LEDC_INTR_DISABLE;
    channelConfigMotor2.timer_sel    = LEDC_TIMER_1;
    channelConfigMotor2.duty         = 0; //Initial Duty Cycle
    ledc_channel_config(&channelConfigMotor2);

    //Initialize Timer
    ledc_timer_config_t timerConfig = {0};
    timerConfig.speed_mode          = LEDC_HIGH_SPEED_MODE;
    timerConfig.duty_resolution     = PWM_RESOLUTION;
    timerConfig.timer_num           = LEDC_TIMER_1;
    timerConfig.freq_hz             = PWM_FREQUENCY; //50 Hz
    ledc_timer_config(&timerConfig);

    // Initialize normal GPIO pins for LEDs as digital output
    gpio_reset_pin(GPIO_LEDs);
    gpio_set_direction(GPIO_LEDs, GPIO_MODE_OUTPUT);
   
    return ESP_OK;
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
