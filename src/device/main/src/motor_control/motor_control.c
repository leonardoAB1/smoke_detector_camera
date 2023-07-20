/*******************************************************************************
 * @file        motor_control.c
 * @author      Leonardo Acha Boiano
 * @date        12 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *              Based on servo driver from the ESP-IoT-Solution library.
 *
 *******************************************************************************/
#include "../motor_control/motor_control.h"

Motor create_motor(ledc_mode_t speed_mode, uint8_t channel) {
    Motor motor;
    motor.speed_mode = speed_mode;
    motor.channel = channel;
    motor.angle = 0.0f;
    motor.is_active = true;
    return motor;
}

//Global motor strucures
Motor motor1;
Motor motor2;

void initialize_motors()
{
    motor1 = create_motor(MOTOR_1_SPEED_MODE, MOTOR_1_CHANNEL);
    motor2 = create_motor(MOTOR_2_SPEED_MODE, MOTOR_2_CHANNEL);
}

void start_motor_movement(Motor *motor) {
    motor->is_active = true;
}

void stop_motor_movement(Motor *motor) {
    motor->is_active = false;
}

esp_err_t move_motor(const Motor *motor, float angle) {
    MOTOR_CHECK(motor->speed_mode < LEDC_SPEED_MODE_MAX, "LEDC speed mode invalid", ESP_ERR_INVALID_ARG);
    MOTOR_CHECK(motor->channel < LEDC_CHANNEL_MAX, "LEDC channel number too large", ESP_ERR_INVALID_ARG);
    MOTOR_CHECK(angle >= 0.0f, "Angle can't be negative", ESP_ERR_INVALID_ARG);

    esp_err_t result;
    uint32_t duty = calculate_duty(angle);
    result = ledc_set_duty(motor->speed_mode, (ledc_channel_t)motor->channel, duty);
    result |= ledc_update_duty(motor->speed_mode, (ledc_channel_t)motor->channel);

    MOTOR_CHECK(ESP_OK == result, "write servo angle failed", ESP_FAIL);

    //uint32_t current_duty = ledc_get_duty(motor->speed_mode, (ledc_channel_t)motor->channel);
    //ESP_LOGI(MOTOR_TAG, "Actual motor duty cycle for speed mode %d and channel %d: %lu Angle: %.2f Duty: %lu", motor->speed_mode, motor->channel, (unsigned long)current_duty, motor->angle, (unsigned long)duty);

    return ESP_OK;
}

uint32_t calculate_duty(float angle) {
    uint32_t duty  = (uint32_t)(SERVO_WIDTH_MIN_US + ((SERVO_WIDTH_MAX_US - SERVO_WIDTH_MIN_US) * angle) / SERVO_MAX_ANGLE);
    return duty;
}

// Motor getter
float get_motor_angle(const Motor *motor)
{
    return motor->angle;
}

// Motor setter
void set_motor_angle(Motor *motor, float angle)
{
    motor->angle = angle;
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
