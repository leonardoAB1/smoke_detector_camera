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
#include "../gpio_utils/gpio_utils.h"

#define MOTOR_1 1
#define MOTOR_2 2

#define MOTOR_FREQUENCY 50

/**
 * @brief Moves the specified motor to a custom angle.
 * 
 * @param motor The motor to move (MOTOR_1 or MOTOR_2).
 * @param angle The desired angle for the motor (0 to 180 for MOTOR_1, 0 to 90 for MOTOR_2).
 * 
 * @note This function allows admins to control the motors freely through a REST API command.
 *       The motor angle is updated as requested and no longer follows the default pattern.
 * @note Admin user permissions are required to execute this function.
 */
void move_motor_custom(int motor, int angle);

/**
 * @brief Moves the specified motor to the default angle.
 * 
 * @param motor The motor to move (MOTOR_1 or MOTOR_2).
 * 
 * @note This function returns the specified motor to the default angle as per the default pattern.
 *       The default pattern consists of motor1 moving from 0 to 180 degrees, with each degree taking 2 seconds,
 *       and motor2 moving from 0 to 90 degrees, with each degree taking 3 seconds.
 * @note Admin user permissions are required to stop and restart this function.
 */
void move_motor_default(int motor);

#endif /* MOTOR_CONTROL_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
