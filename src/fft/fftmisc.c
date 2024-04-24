/*============================================================================

    fftmisc.c  -  Don Cross <dcross@intersrv.com>

    http://www.intersrv.com/~dcross/fft.html

    Helper routines for Fast Fourier Transform implementation.
    Contains common code for fft_float() and fft_double().

    See also:
        fourierf.c
        fourierd.c
        ..\include\fourier.h

    Revision history:

1998 September 19 [Don Cross]
    Improved the efficiency of IsPowerOfTwo().
    Updated coding standards.

============================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "fourier.h"

#define TRUE 1
#define FALSE 0

#define BITS_PER_WORD (sizeof(uint32_t) * 8)

int32_t IsPowerOfTwo(uint32_t x)
{
    if (x < 2)
        return FALSE;

    if (x & (x - 1)) // Thanks to 'byang' for this cute trick!
        return FALSE;

    return TRUE;
}

uint32_t NumberOfBitsNeeded(uint32_t PowerOfTwo)
{
    uint32_t i;

    if (PowerOfTwo < 2)
    {
        fprintf(
            stderr,
            ">>> Error in fftmisc.c: argument %d to NumberOfBitsNeeded is too small.\r\n",
            PowerOfTwo);

        exit(1);
    }

    for (i = 0;; i++)
    {
        if (PowerOfTwo & (1 << i))
            return i;
    }
}

uint32_t ReverseBits(uint32_t index, uint32_t NumBits)
{
    uint32_t i, rev;

    for (i = rev = 0; i < NumBits; i++)
    {
        rev = (rev << 1) | (index & 1);
        index >>= 1;
    }

    return rev;
}

double Index_to_frequency(uint32_t NumSamples, uint32_t Index)
{
    if (Index >= NumSamples)
        return 0.0;
    else if (Index <= NumSamples / 2)
        return (double)Index / (double)NumSamples;

    return -(double)(NumSamples - Index) / (double)NumSamples;
}

/*--- end of file fftmisc.c---*/
