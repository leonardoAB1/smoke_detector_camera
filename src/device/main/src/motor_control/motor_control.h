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

#define MOTOR_01 1
#define MOTOR_02 2

void move_motor_custom(int motor, int angle);
void move_motor_default(int motor);

#endif /* MOTOR_CONTROL_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
