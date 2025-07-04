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

benchmark_hash_t benchmark_main(void)
{
    benchmark_hash_t benchmark_hash_ret = 0;

    volatile uint32_t noprint_output;
    uint32_t i, j, num;
    uint32_t set_bits;

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

    noprint_output = set_bits;
    (void)noprint_output;

    return benchmark_hash_ret;
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
