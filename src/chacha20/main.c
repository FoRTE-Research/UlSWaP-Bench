/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "common.h"

#define INPUT_IMPLEMENTATION
#include "text_input_1k.h"

#define CHACHA20_IMPLEMENTATION
#define CHACHA20_NO_UNDEF
#include "ChaCha20.h"

#define TOTAL_ENCRYPTION_SIZE 65536
#define ITERATIONS (TOTAL_ENCRYPTION_SIZE / INPUT_SIZE)
#define BYTES_TO_PRINT 128
#define BYTES_PER_LINE 32

const key256_t key = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

const nonce96_t nonce = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a,
    0x00, 0x00, 0x00, 0x00};

const uint32_t count = 0x00000001;

uint8_t enc_output[INPUT_SIZE];
uint8_t dec_output[INPUT_SIZE];
ChaCha20_Ctx ctx;


static void print_char_array(const uint8_t *arr, uint32_t len, const char* label)
{
    if (label)
    {
        printf("%s:\r\n", label);
    }
    for (int i = 0; i < len; i++)
    {
        printf("%02X ", arr[i]);
        if (((i+1) < len) && ((i + 1) % BYTES_PER_LINE == 0))
        {
            printf("\r\n");
        }
    }
    printf("\r\n");
}


int check_decrypt()
{
    return memcmp(test_data, dec_output, INPUT_SIZE);
}


int benchmark_main(void)
{
    print_char_array(key, sizeof(key), "Key");
    print_char_array(nonce, sizeof(nonce), "Nonce");
    printf("\r\n");

    printf("Encrypting %u bytes %u times (%u bytes total)\r\n", INPUT_SIZE, ITERATIONS, TOTAL_ENCRYPTION_SIZE);
    printf("Plaintext (first %u bytes):\r\n", BYTES_TO_PRINT);
    print_char_array(test_data, BYTES_TO_PRINT, NULL);
    printf("Plaintext (final %u bytes):\r\n", BYTES_TO_PRINT);
    print_char_array((test_data + INPUT_SIZE - BYTES_TO_PRINT), BYTES_TO_PRINT, NULL);

    ChaCha20_init(&ctx, key, nonce, count);
    memcpy(enc_output, test_data, INPUT_SIZE);
    for (uint32_t i = 0; i < ITERATIONS; i++)
    {
        ChaCha20_xor(&ctx, enc_output, enc_output, INPUT_SIZE);
    }

    printf("\r\nOutput ciphertext after %u iterations (first %u bytes):\r\n", ITERATIONS, BYTES_TO_PRINT);
    print_char_array(enc_output, BYTES_TO_PRINT, NULL);
    printf("Output ciphertext after %u iterations (final %u bytes):\r\n", ITERATIONS, BYTES_TO_PRINT);
    print_char_array(enc_output + INPUT_SIZE - BYTES_TO_PRINT, BYTES_TO_PRINT, NULL);

    printf("\r\nDecrypting %u bytes back %u times (%u bytes total)\r\n", INPUT_SIZE, ITERATIONS, TOTAL_ENCRYPTION_SIZE);
    ChaCha20_init(&ctx, key, nonce, count);
    memcpy(dec_output, enc_output, INPUT_SIZE);
    for (uint32_t i = 0; i < ITERATIONS; i++)
    {
        ChaCha20_xor(&ctx, dec_output, dec_output, INPUT_SIZE);
    }

    printf("Decrypted plaintext after %u iterations (first %u bytes):\r\n", ITERATIONS, BYTES_TO_PRINT);
    print_char_array(dec_output, BYTES_TO_PRINT, NULL);
    printf("Decrypted plaintext after %u iterations (final %u bytes):\r\n", ITERATIONS, BYTES_TO_PRINT);
    print_char_array((dec_output + INPUT_SIZE - BYTES_TO_PRINT), BYTES_TO_PRINT, NULL);
    printf("\r\n");

    return 0;
}
