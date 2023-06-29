/*******************************************************************************
 * @file        motor_control.h
 * @author      Leonardo Acha Boiano
 * @date        12 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include <stdbool.h>
#include <math.h>
#include "driver/ledc.h"
#include "../gpio_utils/gpio_utils.h"

#define MOTOR_1_CHANNEL LEDC_CHANNEL_0
#define MOTOR_2_CHANNEL LEDC_CHANNEL_1

#define MOTOR_1_SPEED_MODE LEDC_HIGH_SPEED_MODE
#define MOTOR_2_SPEED_MODE LEDC_HIGH_SPEED_MODE

#define PWM_FREQUENCY 50
#define PWM_RESOLUTION LEDC_TIMER_13_BIT

#define SERVO_FULL_DUTY ((1 << PWM_RESOLUTION) - 1)  // Maximum duty cycle value

#define SERVO_WIDTH_MIN_US 500
#define SERVO_WIDTH_MAX_US 2500

#define SERVO_MAX_ANGLE 180

#define MOTOR_CHECK(a, str, ret_val) \
    if (!(a)) { \
        ESP_LOGE(MOTOR_TAG, "%s(%d): %s", __FUNCTION__, __LINE__, str); \
        return (ret_val); \
    }

typedef struct {
    ledc_mode_t speed_mode;
    uint8_t channel;
    float angle;
    bool is_active;
} Motor;

Motor create_motor(ledc_mode_t speed_mode, uint8_t channel);

void initialize_motors(void);

void start_motor_movement(Motor *motor);

void stop_motor_movement(Motor *motor);

esp_err_t move_motor(const Motor *motor, float angle);

uint32_t calculate_duty(float angle);

// Motor getter
float get_motor_angle(const Motor *motor);

// Motor setter
void set_motor_angle(Motor *motor, float angle);

#endif /* MOTOR_CONTROL_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
