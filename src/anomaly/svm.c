#include <math.h>
#include <stdint.h>

static double computeKernel(double *x, double v0, double v1, double v2, double v3, double v4, double v5){
  double k = pow(v0 - x[0], 2.0) + pow(v1 - x[1], 2.0) + pow(v2 - x[2], 2.0) + pow(v3 - x[3], 2.0) + pow(v4 - x[4], 2.0) + pow(v5 - x[5], 2.0);
  return exp(-0.001 * k);
}

// Taylor series (3 terms) approximation of e^x
// Assumes x is scaled by factor of 10^3 for fixed-point math
static int32_t exp_approx(int32_t x){
  return 1000 + x + (x * x) / 2000;
}

static int32_t computeKernelFixedPoint(int32_t *x, int32_t v0, int32_t v1, int32_t v2, int32_t v3, int32_t v4, int32_t v5){
  int32_t k = 0;
  // pow() expects floats, let's stick with regular multiplication
  k += (v0 - x[0]) * (v0 - x[0]);
  k += (v1 - x[1]) * (v1 - x[1]);
  k += (v2 - x[2]) * (v2 - x[2]);
  k += (v3 - x[3]) * (v3 - x[3]);
  k += (v4 - x[4]) * (v4 - x[4]);
  k += (v5 - x[5]) * (v5 - x[5]);
  // exp(-0.001 * k)
  k /= 1000000;
  k *= -1;
  return exp_approx(k);
}


double score(double *input){
  double kernels[3] = {0};
  kernels[0] = computeKernel(input, 389.255, 4.231, 111.702, 4.664, 1009.682, -0.429);
  kernels[1] = computeKernel(input, 389.628, 4.266, 107.383, 4.079, 1009.527, -0.485);
  kernels[2] = computeKernel(input, 394.161, 4.313, 108.048, 3.952, 1009.667, -0.451);
  double score = -1.766 + (0.839 * kernels[0]) + (0.113 * kernels[1]) + (0.848 * kernels[2]);
  return score;
}

int32_t scoreFixedPoint(int32_t *input){
  int32_t kernels[3] = {0};
  kernels[0] = computeKernelFixedPoint(input, 389255, 4231, 111702, 4664, 1009682, -429);
  kernels[1] = computeKernelFixedPoint(input, 389628, 4266, 107383, 4079, 1009527, -485);
  kernels[2] = computeKernelFixedPoint(input, 394161, 4313, 108048, 3952, 1009667, -451);
  // Kernel coefficients scaled by 10^3, kernels scaled by 10^3, need to scale intercept by 10^6
  int32_t score = -1766000 + (839 * kernels[0]) + (113 * kernels[1]) + (848 * kernels[2]);
  return score;
}
