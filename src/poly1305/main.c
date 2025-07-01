#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "common.h"
#include "../text_input_16k.h"
#include "poly1305.h"

// Test vectors from RFC 7539 -> https://tools.ietf.org/html/rfc7539#section-2.5.2
// Expected hash: a8:06:1d:c1:30:51:36:c6:c2:2b:8b:af:0c:01:27:a9
// Uncomment to verify
// const uint8_t test_data[] = "Cryptographic Forum Research Group";

// From RFC 7539
const uint8_t key [32] = { 0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33,
                           0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8,
                           0x01, 0x03, 0x80, 0x8a, 0xfb, 0x0d, 0xb2, 0xfd,
                           0x4a, 0xbf, 0xf6, 0xaf, 0x41, 0x49, 0xf5, 0x1b };

struct poly1305_context ctx;

void print_hash(uint8_t *hash)
{
    for (int i = 0; i < 16; i++)
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

    uint8_t hash[16];

    printf("Hashing %zu bytes of data\r\n", strlen((char*) test_data));

    Poly1305_Init(&ctx, key);
    Poly1305_Update(&ctx, test_data, strlen((char*) test_data));
    Poly1305_Final(&ctx, hash);

    print_hash(hash);

#if HASH_TEST
    hash_update(&benchmark_hash_ctx, &hash, sizeof(hash));
    hash_final(benchmark_hash, &benchmark_hash_ctx);
    benchmark_hash_ret = hash_get_lowest32bits(benchmark_hash);
#endif  // HASH_TEST

    return benchmark_hash_ret;
}
