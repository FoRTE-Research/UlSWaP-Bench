/*
 * Copyright 2015-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OSSL_CRYPTO_POLY1305_H
# define OSSL_CRYPTO_POLY1305_H
# pragma once

#include <stddef.h>

#define POLY1305_BLOCK_SIZE  16
#define POLY1305_DIGEST_SIZE 16
#define POLY1305_KEY_SIZE    32

typedef struct poly1305_context POLY1305;

typedef void (*poly1305_blocks_f) (void *ctx, const uint8_t *inp,
                                   size_t len, uint32_t padbit);
typedef void (*poly1305_emit_f) (void *ctx, uint8_t mac[16],
                                 const uint32_t nonce[4]);

struct poly1305_context {
    double opaque[24];  /* large enough to hold internal state, declared
                         * 'double' to ensure at least 64-bit invariant
                         * alignment across all platforms and
                         * configurations */
    uint32_t nonce[4];
    uint8_t data[POLY1305_BLOCK_SIZE];
    size_t num;
    struct {
        poly1305_blocks_f blocks;
        poly1305_emit_f emit;
    } func;
};

size_t Poly1305_ctx_size(void);
void Poly1305_Init(POLY1305 *ctx, const uint8_t key[32]);
void Poly1305_Update(POLY1305 *ctx, const uint8_t *inp, size_t len);
void Poly1305_Final(POLY1305 *ctx, uint8_t mac[16]);

#endif /* OSSL_CRYPTO_POLY1305_H */
