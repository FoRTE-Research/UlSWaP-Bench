/* +++Date last modified: 05-Jul-1997 */

/*
**  Bit counter by Ratko Tomic
*/

#include "bitops.h"

uint32_t bitcount(uint32_t i)
{
    i = ((i & 0xAAAAAAAAL) >> 1) + (i & 0x55555555L);
    i = ((i & 0xCCCCCCCCL) >> 2) + (i & 0x33333333L);
    i = ((i & 0xF0F0F0F0L) >> 4) + (i & 0x0F0F0F0FL);
    i = ((i & 0xFF00FF00L) >> 8) + (i & 0x00FF00FFL);
    i = ((i & 0xFFFF0000L) >> 16) + (i & 0x0000FFFFL);

    return (uint32_t)i;
}

#ifdef TEST

#include <stdlib.h>
#include "snip_str.h" /* For plural_text() macro    */

main(int argc, char *argv[])
{
    uint32_t n;

    while (--argc)
    {
        uint32_t i;

        n = atol(*++argv);
        i = bitcount(n);
        printf("%ld contains %d bit%s set\n", n, i, plural_text(i));
    }

    return 0;
}

#endif /* TEST */
