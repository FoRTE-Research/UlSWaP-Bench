/** need to choose which SHA implementation to run **/
// #define gladman_sha
#define saddi_sha
//#define mbedtls_sha

#include <stdio.h>
#include <string.h>
#include "input.h"

#ifdef gladman_sha
#include <memory.h>
#include <ctype.h>
#include "gladman/sha2.h"
#endif
#ifdef saddi_sha
#include "sha256.h"
#include <stdlib.h>
#endif
#ifdef mbedtls_sha
#include "mbedtls/sha256.h"
#endif

#define DIGEST_BYTES (256/8)

/** Globals (test inputs) **/
// unsigned char data[] = "abc"; // Data you want to hash
// Used to verify the hash function
unsigned char check_sha256[] = { 0xd2, 0x8b, 0x2e, 0xb4, 0x92, 0xf8, 0xbc, 0xc0,
                                 0x2b, 0xd1, 0xfb, 0x77, 0x59, 0x50, 0x2e, 0xcf,
                                 0x1c, 0xd2, 0xbb, 0x1a, 0x98, 0x06, 0x59, 0xa3,
                                 0x64, 0xb8, 0xba, 0x4a, 0xdb, 0xf0, 0x49, 0x39 };
uint8_t hash[DIGEST_BYTES]; // the output of SHA256 will be stored here
// size_t len = sizeof(data);

/** contexts **/
#ifdef gladman_sha
sha256_ctx cx[1];
#endif
#ifdef saddi_sha
SHA256_CTX ctx;
#endif
#ifdef mbedtls_sha
mbedtls_sha256_context ctx;
#endif

/** Call initialization functions for different SHA implementations **/
void init_sha()
{
#ifdef gladman_sha
    sha256_begin(cx);
#endif
#ifdef saddi_sha
    sha256_init(&ctx);
#endif
#ifdef mbedtls_aes
    mbedtls_sha256_init(&ctx);
#endif
}

void test_sha256()
{
#ifdef gladman_sha
    sha256(hash, data, len, cx);
#endif
#ifdef saddi_sha
    sha256_update(&ctx, data, strlen((char*) data));
    // sha256_update(&ctx, data, sizeof(data));
    sha256_final(&ctx, hash);
#endif
#ifdef mbedtls_sha
    mbedtls_sha256(data, len, hash, 0, ctx);
#endif
// hash now contains the output of SHA-256
}

int check_result()
{
   return memcmp((uint8_t*) hash, (uint8_t*) check_sha256, DIGEST_BYTES);
}

void print_hash()
{
    for (int i = 0; i < DIGEST_BYTES; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\r\n");
}

int main(int argc, char *argv[])
{
    /** initialize SHA **/
    init_sha();
    // printf("%s\r\n", hash);

    /** test SHA-256 **/
    test_sha256();
    print_hash();

    /** Check the result to see whether SHA algorithm is correctly working or not **/
   volatile unsigned int verify = check_result();
    printf("%s\r\n", verify ? "FAIL" : "PASS");

#if defined msp432p401r || defined msp430fr5994
    volatile unsigned int elapsed = getElapsedTime();
#endif

    // while (1);
    return 0;

}
