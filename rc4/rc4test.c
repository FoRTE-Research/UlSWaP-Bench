#include "testdata.h"
#include <stdint.h>

void hexstring(uint32_t d);
void hexstrings(uint32_t d);

uint8_t ciphertext[TESTDATALEN];
uint8_t plaintext[TESTDATALEN];

/*        This code illustrates a sample implementation
 *                 of the Arcfour algorithm
 *         Copyright (c) April 29, 1997 Kalle Kaukonen.
 *                    All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that this copyright
 * notice and disclaimer are retained.
 *
 * THIS SOFTWARE IS PROVIDED BY KALLE KAUKONEN AND CONTRIBUTORS ``AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL KALLE
 * KAUKONEN OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

typedef struct
{
    uint32_t x;
    uint32_t y;
    uint8_t state[256];
} ArcfourContext;

void arcfour_init(ArcfourContext *ctx, const uint8_t *key, uint32_t key_len);
uint32_t arcfour_byte(ArcfourContext *ctx);
void arcfour_encrypt(ArcfourContext *ctx, uint8_t *dest, const uint8_t *src, uint32_t len);

void arcfour_init(ArcfourContext *ctx, const uint8_t *key, uint32_t key_len)
{
    uint32_t t, u;
    uint32_t keyindex;
    uint32_t stateindex;
    uint8_t *state;
    uint32_t counter;

    state = ctx->state;
    ctx->x = 0;
    ctx->y = 0;
    for (counter = 0; counter < 256; counter++)
    {
        state[counter] = counter;
    }
    keyindex = 0;
    stateindex = 0;
    for (counter = 0; counter < 256; counter++)
    {
        t = state[counter];
        stateindex = (stateindex + key[keyindex] + t) & 0xff;
        u = state[stateindex];
        state[stateindex] = t;
        state[counter] = u;
        if (++keyindex >= key_len)
        {
            keyindex = 0;
        }
    }
}

uint32_t arcfour_byte(ArcfourContext *ctx)
{
    uint32_t x;
    uint32_t y;
    uint32_t sx, sy;
    uint8_t *state;

    state = ctx->state;
    x = (ctx->x + 1) & 0xff;
    sx = state[x];
    y = (sx + ctx->y) & 0xff;
    sy = state[y];
    ctx->x = x;
    ctx->y = y;
    state[y] = sx;
    state[x] = sy;
    return state[(sx + sy) & 0xff];
}

void arcfour_encrypt(ArcfourContext *ctx, uint8_t *dest, const uint8_t *src, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; i++)
    {
        dest[i] = src[i] ^ arcfour_byte(ctx);
    }
}

int main(void)
{
    uint32_t errors = 0;
    uint32_t ra;

    uint8_t mykey[] = {
        0x29, 0x04, 0x19, 0x72,
        0xfb, 0x42, 0xba, 0x5f,
        0xc7, 0x12, 0x77, 0x12,
        0xf1, 0x38, 0x29, 0xc9};

    ArcfourContext mycontext;

    hexstring(0x12345678);

    /* Encryption */
    arcfour_init(&mycontext, mykey, 16);
    arcfour_encrypt(&mycontext, ciphertext, testdata, TESTDATALEN);

    /* Decryption */
    arcfour_init(&mycontext, mykey, 16);
    arcfour_encrypt(&mycontext, plaintext, ciphertext, TESTDATALEN);

    for (ra = 0; ra < TESTDATALEN; ra++)
    {
        if (plaintext[ra] != testdata[ra])
        {
            errors++;
            hexstrings(ra);
            hexstrings(plaintext[ra]);
            hexstring(testdata[ra]);
        }
        if (errors > 20)
        {
            break;
        }
    }

    hexstring(errors);
    if (errors)
    {
        hexstring(0xBADBAD99);
        return (1);
    }

    hexstring(0x12345678);
    return (0);
}
