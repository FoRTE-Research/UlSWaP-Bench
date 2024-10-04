//
// Created by willi on 2022/2/22.
//

#ifndef TINY_BIGNUM_RSA_RSA_TEST_H
#define TINY_BIGNUM_RSA_RSA_TEST_H

#include <stdint.h>
#include "bn.h"

#if HOST_TEST
#define MAX_ROUNDS UINT32_MAX
#else
#define MAX_ROUNDS 16
#endif

void pow_mod_faster(struct bn* a, struct bn* b, struct bn* n, struct bn* res, uint32_t max_rounds);
void rsa1024_encrypt(const char* public_key, uint64_t exponent, const char* plaintext_hex, char* output);
void rsa1024_decrypt(const char* public_key, const char* private_key, const char* cipher, char* output);

#endif //TINY_BIGNUM_RSA_RSA_TEST_H
