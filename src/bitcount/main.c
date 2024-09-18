/* +++Date last modified: 05-Jul-1997 */

/*
**  BITCNTS.C - Test program for bit counting functions
**
**  public domain by Bob Stout & Auke Reitsma
*/

#include <stdio.h>
#include <stdlib.h>
#include "bitops.h"
#include "common.h"

#define NUM_FUNCS  8
#define RNG_SEED   0x0C728394
#define ITERATIONS 100000

void my_srand(uint32_t new_seed);
uint32_t my_rand(void);

static uint32_t bit_shifter(uint32_t x);

int benchmark_main(void)
{
    uint32_t i, j, num;
    volatile uint32_t set_bits;

    my_srand(RNG_SEED);

    static uint32_t (*pBitCntFunc[NUM_FUNCS])(uint32_t) = {
        bit_count,
        bitcount,
        ntbl_bitcnt,
        ntbl_bitcount,
        btbl_bitcnt,
        BW_btbl_bitcount,
        AR_btbl_bitcount,
        bit_shifter};
    static char *func_names[NUM_FUNCS] = {
        "Optimized 1 bit/loop counter",
        "Ratko's mystery algorithm",
        "Recursive bit count by nybbles",
        "Non-recursive bit count by nybbles",
        "Recursive bit count by bytes",
        "Non-recursive bit count by bytes (BW)",
        "Non-recursive bit count by bytes (AR)",
        "Shift and count bits"};
    (void)func_names;

    printf("Counting total number of 1's in %u random numbers\r\n", ITERATIONS);

    for (i = 0; i < NUM_FUNCS; i++)
    {
        for (j = set_bits = 0, num = my_rand(); j < ITERATIONS; j++, num += 13)
        {
            set_bits += pBitCntFunc[i](num);
        }

        printf("%-38s> Set bits: %u\r\n", func_names[i], set_bits);
    }

    return 0;
}

static uint32_t bit_shifter(uint32_t x)
{
    uint32_t i, n;

    for (i = n = 0; x && (i < (sizeof(uint32_t) * CHAR_BIT)); ++i, x >>= 1)
    {
        n += (uint32_t)(x & 1);
    }

    return n;
}
