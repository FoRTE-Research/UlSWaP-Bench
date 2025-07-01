#include <stdio.h>
#include <string.h>

#define INPUT_IMPLEMENTATION
#include "text_input_64k.h"
#include "sha256.h"
#include "common.h"

#define DIGEST_BYTES (256/8)


void print_hash(uint8_t *hash)
{
    for (int i = 0; i < DIGEST_BYTES; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\r\n");
}

benchmark_hash_t benchmark_main(void)
{
    benchmark_hash_t benchmark_hash_ret = 0;

    SHA256_CTX ctx;                 // SHA256 context
    uint8_t hash[DIGEST_BYTES];     // the output of SHA256 will be stored here

    test_data[INPUT_SIZE - 1] = '\0'; // ensure null termination
    printf("Computing the SHA256 hash of %u bytes of data\r\n", INPUT_SIZE);

    sha256_init(&ctx);
    sha256_update(&ctx, test_data, INPUT_SIZE);
    sha256_final(&ctx, hash);

    print_hash(hash);

    return benchmark_hash_ret;
}
