/**
 * Copyright (c) 2012-2016 Moodstocks SAS
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "buf.h"
#include "conf.h"

#define JPEC_BUFFER_INIT_SIZ 65536
#define OUTPUT_BUF_SIZE 7058
uint8_t output_buf[OUTPUT_BUF_SIZE];

jpec_buffer_t g_jpec_buffer;

jpec_buffer_t *jpec_buffer_new2(int32_t siz)
{
    jpec_buffer_t *b = &g_jpec_buffer;
#if (HOST_TEST && NEW_INPUT_IMAGE)
    b->stream = siz > 0 ? malloc(siz) : NULL;
#else
    b->stream = output_buf;
#endif  // HOST_TEST && NEW_INPUT_IMAGE
    b->siz = OUTPUT_BUF_SIZE;
    b->len = 0;

    return b;
}

void jpec_buffer_write_byte(jpec_buffer_t *b, int32_t val)
{
    assert(b);

#if HOST_TEST
    if (b->siz == b->len)
    {
#if NEW_INPUT_IMAGE
        int32_t nsiz = (b->siz > 0) ? 2 * b->siz : JPEC_BUFFER_INIT_SIZ;
        void *tmp = realloc(b->stream, nsiz);
        printf("Increasing output buffer size to %d\n", nsiz);
        b->stream = (uint8_t *)tmp;
        b->siz = nsiz;
#else
        printf("Insufficient space for output image. Increase OUTPUT_BUF_SIZE.\r\n");
        exit(1);
#endif  // NEW_INPUT_IMAGE
    }
#endif  // HOST_TEST

    b->stream[b->len++] = (uint8_t)val;
}

void jpec_buffer_write_2bytes(jpec_buffer_t *b, int32_t val)
{
    assert(b);
    jpec_buffer_write_byte(b, (val >> 8) & 0xFF);
    jpec_buffer_write_byte(b, val & 0xFF);
}
