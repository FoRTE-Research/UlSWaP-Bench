/* +++Date last modified: 05-Jul-1997 */

#include <string.h>
#include <stdint.h>
#include "snipmath.h"

#define BITSPERNUM (sizeof(uint32_t) * 8)
#define TOP2BITS(x) ((x & (3L << (BITSPERNUM - 2))) >> (BITSPERNUM - 2))

/* usqrt:
    ENTRY x: uint32_t
    EXIT  returns floor(sqrt(x) * pow(2, BITSPERNUM/2))

    Since the square root never uses more than half the bits
    of the input, we use the other half of the bits to contain
    extra bits of precision after the binary point.

    EXAMPLE
        suppose BITSPERNUM = 32
        then    usqrt(144) = 786432 = 12 * 65536
                usqrt(32) = 370727 = 5.66 * 65536

    NOTES
        (1) change BITSPERNUM to BITSPERNUM/2 if you do not want
            the answer scaled.  Indeed, if you want n bits of
            precision after the binary point, use BITSPERNUM/2+n.
            The code assumes that BITSPERNUM is even.
        (2) This is really better off being written in assembly.
            The line marked below is really a "arithmetic shift left"
            on the double-long value with r in the upper half
            and x in the lower half.  This operation is typically
            expressible in only one or two assembly instructions.
        (3) Unrolling this loop is probably not a bad idea.

    ALGORITHM
        The calculations are the base-two analogue of the square
        root algorithm we all learned in grammar school.  Since we're
        in base 2, there is only one nontrivial trial multiplier.

        Notice that absolutely no multiplications or divisions are performed.
        This means it'll be fast on a wide range of processors.
*/

void usqrt(uint32_t x, struct int_sqrt *q)
{
    uint32_t a = 0; /* accumulator      */
    uint32_t r = 0; /* remainder        */
    uint32_t e = 0; /* trial product    */

    uint32_t i;

    for (i = 0; i < BITSPERNUM; i++) /* NOTE 1 */
    {
        r = (r << 2) + TOP2BITS(x);
        x <<= 2; /* NOTE 2 */
        a <<= 1;
        e = (a << 1) + 1;
        if (r >= e)
        {
            r -= e;
            a++;
        }
    }
    memcpy(q, &a, sizeof(uint32_t));
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(void)
{
    uint32_t i;
    uint32_t l = 0x3fed0169L;
    struct int_sqrt q;

    for (i = 0; i < 101; ++i)
    {
        usqrt(i, &q);
        printf("sqrt(%3d) = %2d, remainder = %2d\n",
               i, q.sqrt, q.frac);
    }
    usqrt(l, &q);
    printf("\nsqrt(%lX) = %X, remainder = %X\n", l, q.sqrt, q.frac);
    return 0;
}

#endif /* TEST */
