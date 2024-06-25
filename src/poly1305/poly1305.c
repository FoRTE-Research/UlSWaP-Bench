/*
 * Copyright 2015-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "poly1305.h"

size_t Poly1305_ctx_size(void)
{
    return sizeof(struct poly1305_context);
}

/* pick 32-bit uint32_teger in little endian order */
static uint32_t U8TOuint32_t(const uint8_t *p)
{
    return (((uint32_t)(p[0] & 0xff)) |
            ((uint32_t)(p[1] & 0xff) << 8) |
            ((uint32_t)(p[2] & 0xff) << 16) |
            ((uint32_t)(p[3] & 0xff) << 24));
}

/*
 * Implementations can be classified by amount of significant bits in
 * words making up the multi-precision value, or in other words radix
 * or base of numerical representation, e.g. base 2^64, base 2^32,
 * base 2^26. Complementary characteristic is how wide is the result of
 * multiplication of pair of digits, e.g. it would take 128 bits to
 * accommodate multiplication result in base 2^64 case. These are used
 * interchangeably. To describe implementation that is. But interface
 * is designed to isolate this so that low-level primitives implemented
 * in assembly can be self-contained/self-coherent.
 */

/*
 * Even though there is __int128 reference implementation targeting
 * 64-bit platforms provided below, it's not obvious that it's optimal
 * choice for every one of them. Depending on instruction set overall
 * amount of instructions can be comparable to one in __int64
 * implementation. Amount of multiplication instructions would be lower,
 * but not necessarily overall. And in out-of-order execution context,
 * it is the latter that can be crucial...
 *
 * On related note. Poly1305 author, D. J. Bernstein, discusses and
 * provides floating-point implementations of the algorithm in question.
 * It made a lot of sense by the time of introduction, because most
 * then-modern processors didn't have pipelined integer multiplier.
 * [Not to mention that some had non-constant timing for integer
 * multiplications.] Floating-point instructions on the other hand could
 * be issued every cycle, which allowed to achieve better performance.
 * Nowadays, with SIMD and/or out-or-order execution, shared or
 * even emulated FPU, it's more complicated, and floating-point
 * implementation is not necessarily optimal choice in every situation,
 * rather contrary...
 *
 *                                              <appro@openssl.org>
 */


/*
 * poly1305_blocks processes a multiple of POLY1305_BLOCK_SIZE blocks
 * of |inp| no longer than |len|. Behaviour for |len| not divisible by
 * block size is unspecified in general case, even though in reference
 * implementation the trailing chunk is simply ignored. Per algorithm
 * specification, every input block, complete or last partial, is to be
 * padded with a bit past most significant byte. The latter kind is then
 * padded with zeros till block size. This last partial block padding
 * is caller(*)'s responsibility, and because of this the last partial
 * block is always processed with separate call with |len| set to
 * POLY1305_BLOCK_SIZE and |padbit| to 0. In all other cases |padbit|
 * should be set to 1 to perform implicit padding with 128th bit.
 * poly1305_blocks does not actually check for this constraint though,
 * it's caller(*)'s responsibility to comply.
 *
 * (*)  In the context "caller" is not application code, but higher
 *      level Poly1305_* from this very module, so that quirks are
 *      handled locally.
 */
static void
poly1305_blocks(void *ctx, const uint8_t *inp, size_t len, uint32_t padbit);

/*
 * Type-agnostic "rip-off" from constant_time.h
 */
#define CONSTANT_TIME_CARRY(a, b) ( \
    (a ^ ((a ^ b) | ((a - b) ^ b))) >> (sizeof(a) * 8 - 1))


typedef struct
{
    uint32_t h[5];
    uint32_t r[4];
} poly1305_internal;

/* store a 32-bit uint32_teger in little endian */
static void uint32_tTO8(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)((v) & 0xff);
    p[1] = (uint8_t)((v >> 8) & 0xff);
    p[2] = (uint8_t)((v >> 16) & 0xff);
    p[3] = (uint8_t)((v >> 24) & 0xff);
}

static void poly1305_init(void *ctx, const uint8_t key[16])
{
    poly1305_internal *st = (poly1305_internal *)ctx;

    /* h = 0 */
    st->h[0] = 0;
    st->h[1] = 0;
    st->h[2] = 0;
    st->h[3] = 0;
    st->h[4] = 0;

    /* r &= 0xffffffc0ffffffc0ffffffc0fffffff */
    st->r[0] = U8TOuint32_t(&key[0]) & 0x0fffffff;
    st->r[1] = U8TOuint32_t(&key[4]) & 0x0ffffffc;
    st->r[2] = U8TOuint32_t(&key[8]) & 0x0ffffffc;
    st->r[3] = U8TOuint32_t(&key[12]) & 0x0ffffffc;
}

static void
poly1305_blocks(void *ctx, const uint8_t *inp, size_t len, uint32_t padbit)
{
    poly1305_internal *st = (poly1305_internal *)ctx;
    uint32_t r0, r1, r2, r3;
    uint32_t s1, s2, s3;
    uint32_t h0, h1, h2, h3, h4, c;
    uint64_t d0, d1, d2, d3;

    r0 = st->r[0];
    r1 = st->r[1];
    r2 = st->r[2];
    r3 = st->r[3];

    s1 = r1 + (r1 >> 2);
    s2 = r2 + (r2 >> 2);
    s3 = r3 + (r3 >> 2);

    h0 = st->h[0];
    h1 = st->h[1];
    h2 = st->h[2];
    h3 = st->h[3];
    h4 = st->h[4];

    while (len >= POLY1305_BLOCK_SIZE)
    {
        /* h += m[i] */
        h0 = (uint32_t)(d0 = (uint64_t)h0 + U8TOuint32_t(inp + 0));
        h1 = (uint32_t)(d1 = (uint64_t)h1 + (d0 >> 32) + U8TOuint32_t(inp + 4));
        h2 = (uint32_t)(d2 = (uint64_t)h2 + (d1 >> 32) + U8TOuint32_t(inp + 8));
        h3 = (uint32_t)(d3 = (uint64_t)h3 + (d2 >> 32) + U8TOuint32_t(inp + 12));
        h4 += (uint32_t)(d3 >> 32) + padbit;

        /* h *= r "%" p, where "%" stands for "partial remainder" */
        d0 = ((uint64_t)h0 * r0) +
             ((uint64_t)h1 * s3) +
             ((uint64_t)h2 * s2) +
             ((uint64_t)h3 * s1);
        d1 = ((uint64_t)h0 * r1) +
             ((uint64_t)h1 * r0) +
             ((uint64_t)h2 * s3) +
             ((uint64_t)h3 * s2) +
             (h4 * s1);
        d2 = ((uint64_t)h0 * r2) +
             ((uint64_t)h1 * r1) +
             ((uint64_t)h2 * r0) +
             ((uint64_t)h3 * s3) +
             (h4 * s2);
        d3 = ((uint64_t)h0 * r3) +
             ((uint64_t)h1 * r2) +
             ((uint64_t)h2 * r1) +
             ((uint64_t)h3 * r0) +
             (h4 * s3);
        h4 = (h4 * r0);

        /* last reduction step: */
        /* a) h4:h0 = h4<<128 + d3<<96 + d2<<64 + d1<<32 + d0 */
        h0 = (uint32_t)d0;
        h1 = (uint32_t)(d1 += d0 >> 32);
        h2 = (uint32_t)(d2 += d1 >> 32);
        h3 = (uint32_t)(d3 += d2 >> 32);
        h4 += (uint32_t)(d3 >> 32);
        /* b) (h4:h0 += (h4:h0>>130) * 5) %= 2^130 */
        c = (h4 >> 2) + (h4 & ~3U);
        h4 &= 3;
        h0 += c;
        h1 += (c = CONSTANT_TIME_CARRY(h0, c));
        h2 += (c = CONSTANT_TIME_CARRY(h1, c));
        h3 += (c = CONSTANT_TIME_CARRY(h2, c));
        h4 += CONSTANT_TIME_CARRY(h3, c);
        /*
         * Occasional overflows to 3rd bit of h4 are taken care of
         * "naturally". If after this point we end up at the top of
         * this loop, then the overflow bit will be accounted for
         * in next iteration. If we end up in poly1305_emit, then
         * comparison to modulus below will still count as "carry
         * into 131st bit", so that properly reduced value will be
         * picked in conditional move.
         */

        inp += POLY1305_BLOCK_SIZE;
        len -= POLY1305_BLOCK_SIZE;
    }

    st->h[0] = h0;
    st->h[1] = h1;
    st->h[2] = h2;
    st->h[3] = h3;
    st->h[4] = h4;
}

static void poly1305_emit(void *ctx, uint8_t mac[16],
                          const uint32_t nonce[4])
{
    poly1305_internal *st = (poly1305_internal *)ctx;
    uint32_t h0, h1, h2, h3, h4;
    uint32_t g0, g1, g2, g3, g4;
    uint64_t t;
    uint32_t mask;

    h0 = st->h[0];
    h1 = st->h[1];
    h2 = st->h[2];
    h3 = st->h[3];
    h4 = st->h[4];

    /* compare to modulus by computing h + -p */
    g0 = (uint32_t)(t = (uint64_t)h0 + 5);
    g1 = (uint32_t)(t = (uint64_t)h1 + (t >> 32));
    g2 = (uint32_t)(t = (uint64_t)h2 + (t >> 32));
    g3 = (uint32_t)(t = (uint64_t)h3 + (t >> 32));
    g4 = h4 + (uint32_t)(t >> 32);

    /* if there was carry into 131st bit, h3:h0 = g3:g0 */
    mask = 0 - (g4 >> 2);
    g0 &= mask;
    g1 &= mask;
    g2 &= mask;
    g3 &= mask;
    mask = ~mask;
    h0 = (h0 & mask) | g0;
    h1 = (h1 & mask) | g1;
    h2 = (h2 & mask) | g2;
    h3 = (h3 & mask) | g3;

    /* mac = (h + nonce) % (2^128) */
    h0 = (uint32_t)(t = (uint64_t)h0 + nonce[0]);
    h1 = (uint32_t)(t = (uint64_t)h1 + (t >> 32) + nonce[1]);
    h2 = (uint32_t)(t = (uint64_t)h2 + (t >> 32) + nonce[2]);
    h3 = (uint32_t)(t = (uint64_t)h3 + (t >> 32) + nonce[3]);

    uint32_tTO8(mac + 0, h0);
    uint32_tTO8(mac + 4, h1);
    uint32_tTO8(mac + 8, h2);
    uint32_tTO8(mac + 12, h3);
}

void Poly1305_Init(POLY1305 *ctx, const uint8_t key[32])
{
    ctx->nonce[0] = U8TOuint32_t(&key[16]);
    ctx->nonce[1] = U8TOuint32_t(&key[20]);
    ctx->nonce[2] = U8TOuint32_t(&key[24]);
    ctx->nonce[3] = U8TOuint32_t(&key[28]);

#ifndef POLY1305_ASM
    poly1305_init(ctx->opaque, key);
#else
    /*
     * Unlike reference poly1305_init assembly counterpart is expected
     * to return a value: non-zero if it initializes ctx->func, and zero
     * otherwise. Latter is to simplify assembly in cases when there no
     * multiple code paths to switch between.
     */
    if (!poly1305_init(ctx->opaque, key, &ctx->func))
    {
        ctx->func.blocks = poly1305_blocks;
        ctx->func.emit = poly1305_emit;
    }
#endif

    ctx->num = 0;
}

void Poly1305_Update(POLY1305 *ctx, const uint8_t *inp, size_t len)
{
    size_t rem, num;

    if ((num = ctx->num))
    {
        rem = POLY1305_BLOCK_SIZE - num;
        if (len >= rem)
        {
            memcpy(ctx->data + num, inp, rem);
            poly1305_blocks(ctx->opaque, ctx->data, POLY1305_BLOCK_SIZE, 1);
            inp += rem;
            len -= rem;
        }
        else
        {
            /* Still not enough data to process a block. */
            memcpy(ctx->data + num, inp, len);
            ctx->num = num + len;
            return;
        }
    }

    rem = len % POLY1305_BLOCK_SIZE;
    len -= rem;

    if (len >= POLY1305_BLOCK_SIZE)
    {
        poly1305_blocks(ctx->opaque, inp, len, 1);
        inp += len;
    }

    if (rem)
        memcpy(ctx->data, inp, rem);

    ctx->num = rem;
}

void Poly1305_Final(POLY1305 *ctx, uint8_t mac[16])
{
    size_t num;

    if ((num = ctx->num))
    {
        ctx->data[num++] = 1; /* pad bit */
        while (num < POLY1305_BLOCK_SIZE)
            ctx->data[num++] = 0;
        poly1305_blocks(ctx->opaque, ctx->data, POLY1305_BLOCK_SIZE, 0);
    }

    poly1305_emit(ctx->opaque, mac, ctx->nonce);
}