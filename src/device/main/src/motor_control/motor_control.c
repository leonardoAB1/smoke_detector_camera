/*******************************************************************************
 * @file        motor_control.c
 * @author      Leonardo Acha Boiano
 * @date        12 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/
#include "../motor_control/motor_control.h"

// Global variables for motor angles
volatile float motor1_angle = 0.0f;
volatile float motor2_angle = 0.0f;

// Declare a global flag variable
volatile bool stop_motors = false;

void move_motor(int motor, int duty) {
    // Implement the code to control the specified motor with the given angle
    //ledc_set_duty(LEDC_LOW_SPEED_MODE, motor, duty);
    //ledc_update_duty(LEDC_LOW_SPEED_MODE, motor); 
    vTaskDelay(2000/portTICK_PERIOD_MS);
    //ledc_stop(LEDC_LOW_SPEED_MODE, motor, 0);

    // Update the current angle based on the motor
    if (motor == MOTOR_1)
    {
        motor1_angle = (duty / 255.0f) * 180.0f;
    }
    else if (motor == MOTOR_2)
    {
        motor2_angle = (duty / 255.0f) * 180.0f;
    }
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
        // Check if stop_motors flag is set
        if (stop_motors) {
            break; // Exit the loop if stop_motors is true
        }
        
        int duty = getDutyCycleFromAngle(angle);
        move_motor(MOTOR_1, duty);
        move_motor(MOTOR_2, duty);
    }
    
    for (int angle = 180; angle >= 0; angle--) {
        // Check if stop_motors flag is set
        if (stop_motors) {
            break; // Exit the loop if stop_motors is true
        }
        
        int duty = getDutyCycleFromAngle(angle);
        move_motor(MOTOR_1, duty);
        move_motor(MOTOR_2, duty);
    }
}

void stop_motors_default() {
    stop_motors = true; // Set the stop_motors flag to true
    //ledc_stop(LEDC_LOW_SPEED_MODE, MOTOR_1, 0);
    //ledc_stop(LEDC_LOW_SPEED_MODE, MOTOR_2, 0);
}

// Motor 1 getter
float get_motor1_angle(void)
{
    return motor1_angle;
}

// Motor 2 getter
float get_motor2_angle(void)
{
    return motor2_angle;
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
