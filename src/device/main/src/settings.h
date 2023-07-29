/*******************************************************************************
 * @file        settings.h
 * @author      Leonardo Acha Boiano
 * @date        23 Jul 2023
 * @brief       Settings code of the Smoke Detector Camera Device.
 *              Made to activate and deactivate features such as
 *              Bluetooth Classic and Bluetooth Low Energy
 *              during development.
 *              Warning!!: Aproximately 30% of IRAM 
 *              must be free in order to use it any of them.
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

#define ENABLE_BLE 0 // Change this to 1 to enable BLE functionality

#define ENABLE_BT 1 // Change this to 1 to enable BT functionality

#endif /* SETTINGS_H */

/********************************* END OF FILE ********************************/
/******************************************************************************/
