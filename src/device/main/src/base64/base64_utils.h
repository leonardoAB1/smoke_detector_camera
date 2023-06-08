/*******************************************************************************
 * @file        base64_utils.h
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#ifndef BASE64_UTILS_H
#define BASE64_UTILS_H

#include <stdint.h>
#include <string.h>

int base64_encode(const unsigned char *src, size_t src_len, char *dst, size_t *dst_len);

#endif  // BASE64_UTILS_H

/********************************* END OF FILE ********************************/
/******************************************************************************/
