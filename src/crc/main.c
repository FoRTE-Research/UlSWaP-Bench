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

#define INPUT_IMPLEMENTATION
#include "text_input_64k.h"
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


benchmark_hash_t benchmark_main(void)
{
    benchmark_hash_t benchmark_hash_ret = 0;

#if HASH_TEST
    hash_result_t benchmark_hash;
    hash_ctx_t benchmark_hash_ctx;
    hash_init(&benchmark_hash_ctx);
#endif  // HASH_TEST

    crcInit();
    volatile uint32_t noprint_output;
    test_data[INPUT_SIZE - 1] = '\0'; // ensure null termination

    printf("Computing the 32-bit CRC of a %u byte message\r\n", INPUT_SIZE);
    uint32_t crc = crcFast(test_data, INPUT_SIZE);
    printf("%08X\r\n", crc);
    noprint_output = crc;
    (void)noprint_output;

#if HASH_TEST
    hash_update(&benchmark_hash_ctx, &crc, sizeof(crc));
    hash_final(benchmark_hash, &benchmark_hash_ctx);
    benchmark_hash_ret = hash_get_lowest32bits(benchmark_hash);
#endif  // HASH_TEST

    return benchmark_hash_ret;
}
