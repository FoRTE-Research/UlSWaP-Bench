#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "fourier.h"
#include "common.h"

const uint32_t MAXSIZE = 256;       // small 4096, 8192 inverse, 512 for memory-limited systems
const uint32_t MAXWAVES = 4;        // large has 8
const uint32_t MAX_NUMS_PER_LINE = 8;

static float realin[256];
static float imagin[256];
static float realout[256];
static float imagout[256];
static float Coeff[4];
static float Amp[4];


void printf_float_array(const char* name, float *array, uint32_t size)
{
    printf("%s:\r\n", name);
    for (uint32_t i = 0, p = 0; i < size; i++)
    {
        printf("%+011.4f     ", printf_float(array[i]));
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

int benchmark_main(void)
{
    uint32_t i, j;
    float    *RealIn;
    float    *ImagIn;
    float    *RealOut;
    float    *ImagOut;
    float    *coeff;
    float    *amp;

    my_srand(1);

    // Statically allocate
    RealIn  = realin;
    ImagIn  = imagin;
    RealOut = realout;
    ImagOut = imagout;
    coeff   = Coeff;
    amp     = Amp;

    /* Makes MAXWAVES waves of random amplitude and period */
    for (i = 0; i < MAXWAVES; i++)
    {
        coeff[i] = my_rand() % 1000;
        amp[i]   = my_rand() % 1000;
    }
    for (i = 0; i < MAXSIZE; i++)
    {
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

    printf("Regular FFT\r\n");
    printf("-----------\r\n");
    fft_float(MAXSIZE, FALSE, RealIn, ImagIn, RealOut, ImagOut);
    printf_float_array("RealOut", RealOut, MAXSIZE);
    printf_float_array("ImagOut", ImagOut, MAXSIZE);

    printf("Inverse FFT\r\n");
    printf("-----------\r\n");
    fft_float(MAXSIZE, TRUE, RealIn, ImagIn, RealOut, ImagOut);
    printf_float_array("RealOut", RealOut, MAXSIZE);
    printf_float_array("ImagOut", ImagOut, MAXSIZE);

    return 0;
}
