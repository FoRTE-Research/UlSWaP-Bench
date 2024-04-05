/*============================================================================

       fourier.h  -  Don Cross <dcross@intersrv.com>

       http://www.intersrv.com/~dcross/fft.html

       Contains definitions for doing Fourier transforms
       and inverse Fourier transforms.

============================================================================*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    **   fft() computes the Fourier transform or inverse transform
    **   of the complex inputs to produce the complex outputs.
    **   The number of samples must be a power of two to do the
    **   recursive decomposition of the FFT algorithm.
    **   See Chapter 12 of "Numerical Recipes in FORTRAN" by
    **   Press, Teukolsky, Vetterling, and Flannery,
    **   Cambridge University Press.
    **
    **   Notes:  If you pass ImaginaryIn = NULL, this function will "pretend"
    **           that it is an array of all zeroes.  This is convenient for
    **           transforming digital samples of real number data without
    **           wasting memory.
    */

    void fft_double(
        uint32_t NumSamples,        /* must be a power of 2 */
        int32_t  InverseTransform,  /* 0=forward FFT, 1=inverse FFT */
        double   *RealIn,           /* array of input's real samples */
        double   *ImaginaryIn,      /* array of input's imag samples */
        double   *RealOut,          /* array of output's reals */
        double   *ImaginaryOut);    /* array of output's imaginaries */

    void fft_float(
        uint32_t NumSamples,         /* must be a power of 2 */
        int32_t  InverseTransform,   /* 0=forward FFT, 1=inverse FFT */
        float    *RealIn,            /* array of input's real samples */
        float    *ImaginaryIn,       /* array of input's imag samples */
        float    *RealOut,           /* array of output's reals */
        float    *ImaginaryOut);     /* array of output's imaginaries */

    int32_t  IsPowerOfTwo(uint32_t x);
    uint32_t NumberOfBitsNeeded(uint32_t PowerOfTwo);
    uint32_t ReverseBits(uint32_t index, uint32_t NumBits);

#define fprintf(...)

/*
**   The following function returns an "abstract frequency" of a
**   given index into a buffer with a given number of frequency samples.
**   Multiply return value by sampling rate to get frequency expressed in Hz.
*/
double Index_to_frequency(uint32_t NumSamples, uint32_t Index);

#ifdef __cplusplus
}
#endif

/*--- end of file fourier.h ---*/
