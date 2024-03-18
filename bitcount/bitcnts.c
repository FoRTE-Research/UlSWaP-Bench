/* +++Date last modified: 05-Jul-1997 */

/*
**  BITCNTS.C - Test program for bit counting functions
**
**  public domain by Bob Stout & Auke Reitsma
*/

#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include <limits.h>
#include <time.h>
#include <float.h>
#include "bitops.h"
#include "../bareBench.h"

#define FUNCS 8
#define RNG_SEED 0x0C728394

void my_srand(uint32_t new_seed);
uint32_t my_rand(void);

static int CDECL bit_shifter(uint32_t x);

int main(void)
{
    clock_t start, stop;
    double ct, cmin = DBL_MAX, cmax = 0;
    int i, cminix = 0, cmaxix = 0;
    uint32_t j, n, seed;
    int iterations;

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

    iterations = 1125000;

    puts("Bit counter algorithm benchmark\r\n");

    for (i = 0; i < FUNCS; i++)
    {
        start = clock();

        for (j = n = 0, seed = my_rand(); j < iterations; j++, seed += 13)
            n += pBitCntFunc[i](seed);

        stop = clock();
        ct = (stop - start) / (double)CLOCKS_PER_SEC;
        if (ct < cmin)
        {
            cmin = ct;
            cminix = i;
        }
        if (ct > cmax)
        {
            cmax = ct;
            cmaxix = i;
        }

        printf("%-38s> Time: %7.3f sec.; Bits: %d\r\n", text[i], printf_float(ct), n);
    }
    printf("\r\nBest  > %s\r\n", text[cminix]);
    printf("Worst > %s\r\n", text[cmaxix]);
    return 0;
}

static int CDECL bit_shifter(uint32_t x)
{
    int i, n;

    for (i = n = 0; x && (i < (sizeof(uint32_t) * CHAR_BIT)); ++i, x >>= 1)
        n += (int)(x & 1L);
    return n;
}
