/* +++Date last modified: 05-Jul-1997 */

/*
**  BITCNTS.C - Test program for bit counting functions
**
**  public domain by Bob Stout & Auke Reitsma
*/

#include <stdio.h>
#include <stdlib.h>
#include "bitops.h"
#include "../bareBench.h"

#define FUNCS      8
#define RNG_SEED   0x0C728394
#define ITERATIONS 1125000

void my_srand(uint32_t new_seed);
uint32_t my_rand(void);

static int CDECL bit_shifter(uint32_t x);

int main(void)
{
    uint32_t i, j, n, seed;

    my_srand(RNG_SEED);

    static int (*CDECL pBitCntFunc[FUNCS])(uint32_t) = {
        bit_count,
        bitcount,
        ntbl_bitcnt,
        ntbl_bitcount,
        btbl_bitcnt,
        BW_btbl_bitcount,
        AR_btbl_bitcount,
        bit_shifter};
    static char *text[FUNCS] = {
        "Optimized 1 bit/loop counter",
        "Ratko's mystery algorithm",
        "Recursive bit count by nybbles",
        "Non-recursive bit count by nybbles",
        "Recursive bit count by bytes",
        "Non-recursive bit count by bytes (BW)",
        "Non-recursive bit count by bytes (AR)",
        "Shift and count bits"};

    puts("Bit counter algorithm benchmark\r\n");

    for (i = 0; i < FUNCS; i++)
    {
        for (j = n = 0, seed = my_rand(); j < ITERATIONS; j++, seed += 13)
            n += pBitCntFunc[i](seed);

        printf("%-38s> Bits: %u\r\n", text[i], n);
    }

    return 0;
}

static int CDECL bit_shifter(uint32_t x)
{
    int i, n;

    for (i = n = 0; x && (i < (sizeof(uint32_t) * CHAR_BIT)); ++i, x >>= 1)
        n += (int)(x & 1L);
    return n;
}
