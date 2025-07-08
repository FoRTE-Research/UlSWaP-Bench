#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Enable both ECB and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DECB=1
#define CBC 1
#define ECB 0
#define AES128 1

#define INPUT_IMPLEMENTATION
#include "text_input_1k.h"
#include "common.h"
#include "aes.h"

#define TOTAL_ENCRYPTION_SIZE 65536 // 64 KiB
#define ITERATIONS (TOTAL_ENCRYPTION_SIZE / INPUT_SIZE)
#define BYTES_TO_PRINT 128
#define BYTES_PER_LINE 32

uint8_t g_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
uint8_t g_iv[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

void cbc_encrypt(void);
void cbc_decrypt(void);

static void print_char_array(const uint8_t *arr, uint32_t len, const char* label)
{
    if (label)
    {
        printf("%s:\r\n", label);
    }
    for (int i = 0; i < len; i++)
    {
        printf("%02X ", arr[i]);
        if ((i + 1) % BYTES_PER_LINE == 0)
        {
            printf("\r\n");
        }
    }
    printf("\r\n");
}

int benchmark_main(void)
{
#if HOST_TEST
    char input_copy[INPUT_SIZE + 1];
    memcpy(input_copy, test_data, INPUT_SIZE);
    input_copy[INPUT_SIZE] = '\0';  // Null-terminate the string for printing
#endif  // HOST_TEST

    printf("Mode: CBC\r\n");
    print_char_array(g_key, 16, "Key");
    print_char_array(g_iv, 16, "IV");
    printf("\r\n");

    cbc_encrypt();
    printf("\r\n");
    cbc_decrypt();

#if HOST_TEST
    int success = memcmp(input_copy, test_data, INPUT_SIZE) == 0;
    printf("\r\nDecryption %s\r\n", success ? "succeeded" : "failed");
#endif  // HOST_TEST

    return 0;
}

void increment_iv(void)
{
    for (int i = 15; i >= 0; i--)
    {
        if (++g_iv[i] != 0)
        {
            break; // No overflow, exit the loop
        }
    }
}

void decrement_iv(void)
{
    for (int i = 15; i >= 0; i--)
    {
        if (--g_iv[i] != 0xFF)
        {
            break; // No underflow, exit the loop
        }
    }
}


void cbc_encrypt(void)
{
    printf("Encrypting %u bytes %u times (%u bytes total)\r\n", INPUT_SIZE, ITERATIONS, TOTAL_ENCRYPTION_SIZE);
    printf("Plaintext (first %u bytes):\r\n", BYTES_TO_PRINT);
    print_char_array(test_data, BYTES_TO_PRINT, NULL);
    printf("Plaintext (final %u bytes):\r\n", BYTES_TO_PRINT);
    print_char_array((test_data + INPUT_SIZE - BYTES_TO_PRINT), BYTES_TO_PRINT, NULL);

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, g_key, g_iv);
    for (uint32_t i = 0; i < ITERATIONS; i++)
    {
        AES_CBC_encrypt_buffer(&ctx, test_data, INPUT_SIZE);
        increment_iv();
        AES_ctx_set_iv(&ctx, g_iv);
    }

    printf("Output ciphertext (first %u bytes):\r\n", BYTES_TO_PRINT);
    print_char_array(test_data, BYTES_TO_PRINT, NULL);
    printf("Output ciphertext (final %u bytes):\r\n", BYTES_TO_PRINT);
    print_char_array(test_data + INPUT_SIZE - BYTES_TO_PRINT, BYTES_TO_PRINT, NULL);
}


void cbc_decrypt(void)
{
    printf("Decrypting %u bytes %u times (%u bytes total)\r\n", INPUT_SIZE, ITERATIONS, TOTAL_ENCRYPTION_SIZE);

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, g_key, g_iv);
    for (uint32_t i = 0; i < ITERATIONS; i++)
    {
        decrement_iv();
        AES_ctx_set_iv(&ctx, g_iv);
        AES_CBC_decrypt_buffer(&ctx, test_data, INPUT_SIZE);
    }

    printf("Decrypted plaintext (first %u bytes):\r\n", BYTES_TO_PRINT);
    print_char_array(test_data, BYTES_TO_PRINT, NULL);
    printf("Decrypted plaintext (final %u bytes):\r\n", BYTES_TO_PRINT);
    print_char_array((test_data + INPUT_SIZE - BYTES_TO_PRINT), BYTES_TO_PRINT, NULL);
}
