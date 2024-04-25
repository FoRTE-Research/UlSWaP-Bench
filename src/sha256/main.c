#include <stdio.h>
#include <string.h>

#include "input.h"
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

int benchmark_main(int argc, char *argv[])
{
    SHA256_CTX ctx;                 // SHA256 context
    uint8_t hash[DIGEST_BYTES];     // the output of SHA256 will be stored here

    printf("Hashing %zu bytes of data\r\n", strlen((char*) data));

    sha256_init(&ctx);
    sha256_update(&ctx, data, strlen((char*) data));
    sha256_final(&ctx, hash);

    print_hash(hash);

    return 0;
}
