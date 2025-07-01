#include <stdio.h>
#include <math.h>
#include "snipmath.h"
#include "common.h"

#if HASH_TEST
void round_doubles(double arr[], uint32_t len)
{
    const double precision = 100000.0;  // five decimal places
    for (uint32_t i = 0; i < len; i++)
    {
        arr[i] = round(arr[i] * precision) / precision;
    }
}
#endif  // HASH_TEST

benchmark_hash_t benchmark_main(void)
{
    benchmark_hash_t benchmark_hash_ret = 0;

    double a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
    double a2 = 1.0, b2 = -4.5,  c2 = 17.0, d2 = -30.0;
    double a3 = 1.0, b3 = -3.5,  c3 = 22.0, d3 = -31.0;
    double a4 = 1.0, b4 = -13.7, c4 = 1.0,  d4 = -35.0;
    double solutions[3];
    double input_angle;
    double output_angle;
    uint32_t num_solutions;
    uint32_t i;
    struct int_sqrt q;

#if HASH_TEST
    hash_result_t benchmark_hash;
    hash_ctx_t benchmark_hash_ctx;
    hash_init(&benchmark_hash_ctx);
#endif  // HASH_TEST

    printf("********* CUBIC FUNCTIONS ***********\r\n");
    /* should get 3 solutions: 2, 6 & 2.5   */
    Cubic(a1, b1, c1, d1, &num_solutions, solutions);
    /* should get 1 solution: 2.5           */
    Cubic(a2, b2, c2, d2, &num_solutions, solutions);
    Cubic(a3, b3, c3, d3, &num_solutions, solutions);
    Cubic(a4, b4, c4, d4, &num_solutions, solutions);
    /* Now solve some random equations */
    for (a1 = 1; a1 < 7; a1++)
    {
        for (b1 = 8; b1 > 0; b1--)
        {
            for (c1 = 8; c1 < 12; c1 += 0.5)
            {
                for (d1 = -1; d1 > -8; d1--)
                {
                    Cubic(a1, b1, c1, d1, &num_solutions, solutions);
#if HASH_TEST
                    round_doubles(solutions, num_solutions);
                    hash_update(&benchmark_hash_ctx, solutions, num_solutions * sizeof(double));
#endif  // HASH_TEST
                }
            }
        }
    }

    printf("\r\n********* INTEGER SQR ROOTS ***********\r\n");
    for (i = 0; i < 1001; ++i)
    {
        usqrt(i, &q);
        printf("sqrt(%3d) = %2d\r\n", i, q.sqrt);
#if HASH_TEST
        hash_update(&benchmark_hash_ctx, &q, sizeof(q));
#endif  // HASH_TEST
    }

    printf("\r\n********* ANGLE CONVERSION ***********\r\n");
    for (input_angle = 0.0; input_angle <= 360.0; input_angle += 1.0)
    {
        output_angle = deg2rad(input_angle);
        printf("%3.0f degrees = %.12f radians\r\n", printf_float(input_angle), printf_float(output_angle));
#if HASH_TEST
        round_doubles(&output_angle, 1);
        hash_update(&benchmark_hash_ctx, &output_angle, sizeof(output_angle));
#endif  // HASH_TEST
    }
    printf("\r\n");

    for (input_angle = 0.0; input_angle <= (2 * PI + 1e-6); input_angle += (PI / 180))
    {
        output_angle = rad2deg(input_angle);
        printf("%.12f radians = %3.0f degrees\r\n", printf_float(input_angle), printf_float(output_angle));
#if HASH_TEST
        round_doubles(&output_angle, 1);
        hash_update(&benchmark_hash_ctx, &output_angle, sizeof(output_angle));
#endif  // HASH_TEST
    }

#if HASH_TEST
    hash_final(benchmark_hash, &benchmark_hash_ctx);
    benchmark_hash_ret = hash_get_lowest32bits(benchmark_hash);
#endif  // HASH_TEST

    return benchmark_hash_ret;
}
