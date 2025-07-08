/* +++Date last modified: 05-Jul-1997 */

/*
**  CUBIC.C - Solve a cubic polynomial
**  public domain by Ross Cottrell
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "snipmath.h"
#include "common.h"

void SolveCubic(double a,
                double b,
                double c,
                double d,
                uint32_t *solutions,
                double *x)
{
    double a1 = b / a, a2 = c / a, a3 = d / a;
    double Q = (a1 * a1 - 3.0 * a2) / 9.0;
    double R = (2.0 * a1 * a1 * a1 - 9.0 * a1 * a2 + 27.0 * a3) / 54.0;
    double R2_Q3 = R * R - Q * Q * Q;

    double theta;

    if (R2_Q3 <= 0)
    {
        *solutions = 3;
        theta = acos(R / sqrt(Q * Q * Q));
        x[0] = -2.0 * sqrt(Q) * cos(theta / 3.0) - a1 / 3.0;
        x[1] = -2.0 * sqrt(Q) * cos((theta + 2.0 * PI) / 3.0) - a1 / 3.0;
        x[2] = -2.0 * sqrt(Q) * cos((theta + 4.0 * PI) / 3.0) - a1 / 3.0;
    }
    else
    {
        *solutions = 1;
        x[0] = pow(sqrt(R2_Q3) + fabs(R), 1 / 3.0);
        x[0] += Q / x[0];
        x[0] *= (R < 0.0) ? 1 : -1;
        x[0] -= a1 / 3.0;
    }
}

void PrintCubic(double a, double b, double c, double d, uint32_t *solutions, double *x)
{
    printf("Coefficients: a = %+05.1f, b = %+05.1f, c = %+05.1f, d = %+05.1f    ",
           printf_float(a), printf_float(b), printf_float(c), printf_float(d));
    printf("Solutions:");
    for (uint32_t i = 0; i < *solutions; i++)
    {
        printf("  %+08.6f", printf_float(x[i]));
    }
    printf("\r\n");
}

void Cubic(double a, double b, double c, double d, uint32_t *solutions, double *x)
{
    SolveCubic(a, b, c, d, solutions, x);
    PrintCubic(a, b, c, d, solutions, x);
}

#ifdef TEST

int main(void)
{
    double a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
    double a2 = 1.0, b2 = -4.5, c2 = 17.0, d2 = -30.0;
    double x[3];
    uint32_t solutions;

    SolveCubic(a1, b1, c1, d1, &solutions, x);

    /* should get 3 solutions: 2, 6 & 2.5   */

    SolveCubic(a2, b2, c2, d2, &solutions, x);

    /* should get 1 solution: 2.5           */

    return 0;
}

#endif /* TEST */
