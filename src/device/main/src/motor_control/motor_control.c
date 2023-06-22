/*******************************************************************************
 * @file        motor_control.c
 * @author      Leonardo Acha Boiano
 * @date        12 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#include "../motor_control/motor_control.h"

void move_motor(int motor, int duty) {
    // Implement the code to control the specified motor with the given angle
    ledc_set_duty(LEDC_LOW_SPEED_MODE, motor, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, motor); 
    vTaskDelay(1000/portTICK_PERIOD_MS);
    ledc_stop(LEDC_LOW_SPEED_MODE, motor, 0);
}

int getDutyCycleFromAngle(float angle) {
    // Calculate the pulse width (in milliseconds) corresponding to the given angle
    float dutyMs = (SERVO_MS_MAX - SERVO_MS_MIN) / 180.0 * angle + SERVO_MS_MIN;

    // Calculate the duty cycle value based on the desired PWM frequency and resolution
    // Convert the pulse width from milliseconds to seconds by dividing by 1000
    // Multiply by the PWM frequency to obtain the number of PWM cycles per second
    // Multiply by the total number of steps (2^PWM_RESOLUTION) to get the duty cycle value
    int duty = (int)(dutyMs / (1000.0 / PWM_FREQUENCY) * (pow(2, PWM_RESOLUTION) - 1));

    // Return the calculated duty cycle
    return duty;
}

void move_motors_default() {
    for (int angle = 0; angle <= 180; angle++) {
        int duty = getDutyCycleFromAngle(angle);
        move_motor(MOTOR_1, duty);
        move_motor(MOTOR_2, duty);
    }
    for (int angle = 180; angle >= 0; angle--) {
        int duty = getDutyCycleFromAngle(angle);
        move_motor(MOTOR_1, duty);
        move_motor(MOTOR_2, duty);
    }
}

void stop_motors_default() {
    ledc_stop(LEDC_LOW_SPEED_MODE, MOTOR_1, 0);
    ledc_stop(LEDC_LOW_SPEED_MODE, MOTOR_2, 0);
}


/********************************* END OF FILE ********************************/
/******************************************************************************/
