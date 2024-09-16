#include <math.h>
#include <stdint.h>

static double computeKernel(double *x, double v0, double v1, double v2, double v3, double v4, double v5)
{
    double k = pow(v0 - x[0], 2.0) + pow(v1 - x[1], 2.0) + pow(v2 - x[2], 2.0) + pow(v3 - x[3], 2.0) +
               pow(v4 - x[4], 2.0) + pow(v5 - x[5], 2.0);

    return exp(-0.001 * k);
}

double score(double *input)
{
    double kernels[3] = {0};
    kernels[0] = computeKernel(input, 389.255, 4.231, 111.702, 4.664, 1009.682, 1.313);
    kernels[1] = computeKernel(input, 389.628, 4.266, 107.383, 4.079, 1009.527, 1.312);
    kernels[2] = computeKernel(input, 394.161, 4.313, 108.048, 3.952, 1009.667, 1.319);
    double score = -1.766 + (0.839 * kernels[0]) + (0.113 * kernels[1]) + (0.848 * kernels[2]);

    return score;
}
