/**********************************************************************
 *
 * Filename:    main.c
 *
 * Description: A simple test program for the CRC implementations.
 *
 * Notes:       To test a different CRC standard, modify crc.h.
 *
 *
 * Copyright (c) 2000 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/

#include <stdio.h>
#include <string.h>

#include "common.h"
#include "../text_input_16k.h"
#include "crc.h"

void crc_test(void)
{
    uint8_t test[] = "123456789";

    /*
     * Print the check value for the selected CRC algorithm.
     */
    printf("The check value for the %s standard is 0x%X\n", CRC_NAME, CHECK_VALUE);

    /*
     * Compute the CRC of the test message, slowly.
     */
    printf("The crcSlow() of \"123456789\" is 0x%X\n", crcSlow(test, strlen((const char*)test)));
    crcSlow(test, 9);

    /*
     * Compute the CRC of the test message, more efficiently.
     */
    crcInit();
    printf("The crcFast() of \"123456789\" is 0x%X\n", crcFast(test, strlen((const char*)test)));
    crcFast(test, 9);
}


int benchmark_main(void)
{
    crcInit();
    volatile uint32_t noprint_output;

    printf("Computing the CRC of a %zu byte message\r\n", strlen((const char*)test_data));
    uint32_t crc = crcFast(test_data, strlen((const char*)test_data));
    printf("%08X\r\n", crc);
    noprint_output = crc;
    (void)noprint_output;

    return 0;
}