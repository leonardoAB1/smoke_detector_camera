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
#include "../gpio_utils/gpio_utils.h"

#define MOTOR_1 LEDC_CHANNEL_0
#define MOTOR_2 LEDC_CHANNEL_1

#define PWM_FREQUENCY 50
#define PWM_RESOLUTION LEDC_TIMER_13_BIT

#define SERVO_MS_MIN 0.06 //0.06ms
#define SERVO_MS_MAX 2.0  //2ms

/**
 * @brief Moves the specified motor to a given angle.
 * 
 * @param motor The motor to move (MOTOR_1 or MOTOR_2).
 * @param duty Desired motor duty cycle.
 * 
 * @note This function allows admins to control the servo motors.
 */
void move_motor(int motor, int duty);

/**
 * @brief Calculates the duty cycle based on the given angle.
 * 
 * @param angle The angle (in degrees) to calculate the duty cycle.
 * @return The calculated duty cycle.
 * 
 * @note This function uses the LEDC library to control the duty cycle of the ESP32.
 *       The duty cycle is calculated based on the servo pulse width range defined by
 *       SERVO_MS_MIN and SERVO_MS_MAX, and the desired PWM frequency and resolution.
 */
int getDutyCycleFromAngle(float angle);

#endif /* MOTOR_CONTROL_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
