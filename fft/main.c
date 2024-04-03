#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "fourier.h"
#include "../bareBench.h"

void my_srand(uint32_t new_seed);
uint32_t my_rand(void);


int invfft = 0;
const unsigned MAXSIZE = 256;	   // small 4096, 8192 inverse, 512 for memory-limited systems
const unsigned MAXWAVES = 4; // large has 8
const unsigned MAX_NUMS_PER_LINE = 16;
static float realin[256];
static float imagin[256];
static float realout[256];
static float imagout[256];
static float Coeff[4];
static float Amp[4];

int old_main();

// main for benchmark purposes that does fft and inverse fft
int main()
{
    invfft = 0;
    // MAXSIZE = 128;
    old_main();
    invfft = 1;
    // MAXSIZE = 256;
    old_main();
    return 0;
}

void printf_float_array(const char* name, float *array, unsigned size)
{
    printf("%s:\r\n", name);
    for (unsigned i = 0, p = 0; i < size; i++)
    {
        printf("%+010.4f     ", printf_float(array[i]));
        if (p == MAX_NUMS_PER_LINE - 1)
        {
            printf("\r\n");
            p = 0;
        }
        else
        {
            p++;
        }
    }
    printf("\r\n");
}

int old_main()
{
    unsigned i, j;
    float *RealIn;
    float *ImagIn;
    float *RealOut;
    float *ImagOut;
    float *coeff;
    float *amp;

    my_srand(1);

    /* RealIn=(float*)malloc(sizeof(float)*MAXSIZE);
     ImagIn=(float*)malloc(sizeof(float)*MAXSIZE);
     RealOut=(float*)malloc(sizeof(float)*MAXSIZE);
     ImagOut=(float*)malloc(sizeof(float)*MAXSIZE);
     coeff=(float*)malloc(sizeof(float)*MAXWAVES);
     amp=(float*)malloc(sizeof(float)*MAXWAVES);
    */

    // Statically allocate
    RealIn = realin;
    ImagIn = imagin;
    RealOut = realout;
    ImagOut = imagout;
    coeff = Coeff;
    amp = Amp;

    /* Makes MAXWAVES waves of random amplitude and period */
    for (i = 0; i < MAXWAVES; i++)
    {
        coeff[i] = my_rand() % 1000;
        amp[i] = my_rand() % 1000;
    }
    for (i = 0; i < MAXSIZE; i++)
    {
        /*   RealIn[i]=my_rand();*/
        RealIn[i] = 0;
        for (j = 0; j < MAXWAVES; j++)
        {
            /* randomly select sin or cos */
            if (my_rand() % 2)
            {
                RealIn[i] += coeff[j] * cos(amp[j] * i);
            }
            else
            {
                RealIn[i] += coeff[j] * sin(amp[j] * i);
            }
            ImagIn[i] = 0;
        }
    }

    printf_float_array("RealIn", RealIn, MAXSIZE);
    printf_float_array("ImagIn", ImagIn, MAXSIZE);

    /* regular*/
    fft_float(MAXSIZE, invfft, RealIn, ImagIn, RealOut, ImagOut);

    printf_float_array("RealOut", RealOut, MAXSIZE);
    printf_float_array("ImagOut", ImagOut, MAXSIZE);

    /* free(RealIn);
     free(ImagIn);
     free(RealOut);
     free(ImagOut);
     free(coeff);
     free(amp);
    */
    return 0;
}
