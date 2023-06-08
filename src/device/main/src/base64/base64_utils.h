/*******************************************************************************
 * @file        base64_utils.h
 * @brief       Header file for base64 encoding utility functions
 * @details     Provides a function for base64 encoding of data
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

/**
 * @brief   Base64 encodes a given source data.
 *
 * @param   src         Pointer to the source data buffer.
 * @param   src_len     Length of the source data.
 * @param   dst         Pointer to the destination buffer for the encoded data.
 * @param   dst_len     Pointer to a variable storing the length of the destination buffer. 
 *                      After encoding, this variable will be updated with the actual encoded length.
 *
 * @return  0 on success, -1 if the destination buffer is too small.
 */
int base64_encode(const unsigned char *src, size_t src_len, char *dst, size_t *dst_len);

#endif  // BASE64_UTILS_H

/********************************* END OF FILE ********************************/
/******************************************************************************/
