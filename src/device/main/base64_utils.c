/*******************************************************************************
 * @file        base64_utils.c
 * @author      Leonardo Acha Boiano
 * @date        7 Jun 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *
 *******************************************************************************/

#include "base64_utils.h"

// Base64 encoding table
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Function to perform base64 encoding
int base64_encode(const unsigned char *src, size_t src_len, char *dst, size_t *dst_len)
{
    size_t i, j;
    size_t encoded_len = (src_len + 2) / 3 * 4;

    if (*dst_len < encoded_len) {
        *dst_len = encoded_len;
        return -1; // Destination buffer too small
    }

    for (i = 0, j = 0; i < src_len; i += 3, j += 4) {
        uint32_t octet_a = i < src_len ? src[i] : 0;
        uint32_t octet_b = i + 1 < src_len ? src[i + 1] : 0;
        uint32_t octet_c = i + 2 < src_len ? src[i + 2] : 0;

        dst[j] = base64_table[(octet_a >> 2) & 0x3F];
        dst[j + 1] = base64_table[((octet_a & 0x3) << 4) | ((octet_b >> 4) & 0xF)];
        dst[j + 2] = i + 1 < src_len ? base64_table[((octet_b & 0xF) << 2) | ((octet_c >> 6) & 0x3F)] : '=';
        dst[j + 3] = i + 2 < src_len ? base64_table[octet_c & 0x3F] : '=';
    }

    *dst_len = encoded_len;
    return 0;
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
