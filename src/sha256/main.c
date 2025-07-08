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

benchmark_hash_t benchmark_main(void)
{
    benchmark_hash_t benchmark_hash_ret = 0;

#if HASH_TEST
    hash_result_t benchmark_hash;
    hash_ctx_t benchmark_hash_ctx;
    hash_init(&benchmark_hash_ctx);
#endif  // HASH_TEST

    SHA256_CTX ctx;                 // SHA256 context
    uint8_t hash[DIGEST_BYTES];     // the output of SHA256 will be stored here

    printf("Hashing %zu bytes of data\r\n", strlen((char*) data));

    sha256_init(&ctx);
    sha256_update(&ctx, data, strlen((char*) data));
    sha256_final(&ctx, hash);

    print_hash(hash);

#if HASH_TEST
    hash_update(&benchmark_hash_ctx, &hash, sizeof(hash));
    hash_final(benchmark_hash, &benchmark_hash_ctx);
    benchmark_hash_ret = hash_get_lowest32bits(benchmark_hash);
#endif  // HASH_TEST

    return benchmark_hash_ret;
}
