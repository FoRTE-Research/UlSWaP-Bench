#include <stdio.h>
#include "snipmath.h"
#include <math.h>
#include "common.h"

/* The printf's may be removed to isolate just the math calculations */

int benchmark_main(void)
{
    double a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
    double a2 = 1.0, b2 = -4.5,  c2 = 17.0, d2 = -30.0;
    double a3 = 1.0, b3 = -3.5,  c3 = 22.0, d3 = -31.0;
    double a4 = 1.0, b4 = -13.7, c4 = 1.0,  d4 = -35.0;
    double x[3];
    double X;
    uint32_t solutions;
    uint32_t i;
    struct int_sqrt q;

    printf("********* CUBIC FUNCTIONS ***********\r\n");
    /* should get 3 solutions: 2, 6 & 2.5   */
    Cubic(a1, b1, c1, d1, &solutions, x);
    /* should get 1 solution: 2.5           */
    Cubic(a2, b2, c2, d2, &solutions, x);
    Cubic(a3, b3, c3, d3, &solutions, x);
    Cubic(a4, b4, c4, d4, &solutions, x);
    /* Now solve some random equations */
    for (a1 = 1; a1 < 7; a1++)
    {
        for (b1 = 8; b1 > 0; b1--)
        {
            for (c1 = 8; c1 < 12; c1 += 0.5)
            {
                for (d1 = -1; d1 > -8; d1--)
                {
                    Cubic(a1, b1, c1, d1, &solutions, x);
                }
            }
        }
    }

    printf("\r\n********* INTEGER SQR ROOTS ***********\r\n");
    for (i = 0; i < 1001; ++i)
    {
        usqrt(i, &q);
        printf("sqrt(%3d) = %2d\r\n", i, q.sqrt);
    }

    printf("\r\n********* ANGLE CONVERSION ***********\r\n");
    for (X = 0.0; X <= 360.0; X += 1.0)
    {
        printf("%3.0f degrees = %.12f radians\r\n", printf_float(X), printf_float(deg2rad(X)));
    }
    printf("\r\n");

    for (X = 0.0; X <= (2 * PI + 1e-6); X += (PI / 180))
    {
        printf("%.12f radians = %3.0f degrees\r\n", printf_float(X), printf_float(rad2deg(X)));
    }

    printf("\r\n********* REMAINDERS ***********\r\n");
    my_srand(0x12345678);
    i = 0;
    while (i < 500)
    {
        uint32_t dividend = my_rand() & 0xFFFFFF;
        uint32_t divisor = my_rand() & 0xFFF;
        if (divisor != 0)
        {
            uint32_t remainder = dividend % divisor;
            printf("%8u %% %4u = %4u\r\n", dividend, divisor, remainder);
            (void)remainder;
            i++;
        }
    }
    printf("\r\n");

    return 0;
}
