#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "svm.h"
#include "data.h"
#include "common.h"

// RMS and crest factor (peak/RMS) for one channel
void featurize(double *rms, double *crest, const int16_t *series, size_t len)
{
    int16_t peak = 0;
    *rms = 0;
    for (int16_t i = len - 1; i >= 0; i--)
    {
        if (abs(series[i]) > peak)
        {
            peak = abs(series[i]);
        }
        *rms += (int32_t)series[i] * (int32_t)series[i];
    }
    *rms /= len;
    *rms = sqrt(*rms);
    *crest = peak / *rms;
}

benchmark_hash_t benchmark_main(void)
{
    benchmark_hash_t benchmark_hash_ret = 0;

    volatile double noprint_output;
    double features[6];
    double pred_featurized;

    featurize(&features[0], &features[1], acc_x3, sizeof(acc_x3) / sizeof(acc_x3[0]));
    featurize(&features[2], &features[3], acc_y3, sizeof(acc_y3) / sizeof(acc_y3[0]));
    featurize(&features[4], &features[5], acc_z3, sizeof(acc_z3) / sizeof(acc_z3[0]));
    pred_featurized = score(features);
    printf("Featurized prediction: %f\r\n", printf_float(pred_featurized));
    if (pred_featurized > 0)
    {
        printf("OK\r\n");
    }
    else
    {
        printf("NOK\r\n");
    }

    featurize(&features[0], &features[1], acc_x2, sizeof(acc_x2) / sizeof(acc_x2[0]));
    featurize(&features[2], &features[3], acc_y2, sizeof(acc_y2) / sizeof(acc_y2[0]));
    featurize(&features[4], &features[5], acc_z2, sizeof(acc_z2) / sizeof(acc_z2[0]));
    pred_featurized = score(features);
    printf("Featurized prediction: %f\r\n", printf_float(pred_featurized));
    if (pred_featurized > 0)
    {
        printf("OK\r\n");
    }
    else
    {
        printf("NOK\r\n");
    }

    featurize(&features[0], &features[1], acc_x1, sizeof(acc_x1) / sizeof(acc_x1[0]));
    featurize(&features[2], &features[3], acc_y1, sizeof(acc_y1) / sizeof(acc_y1[0]));
    featurize(&features[4], &features[5], acc_z1, sizeof(acc_z1) / sizeof(acc_z1[0]));
    pred_featurized = score(features);
    printf("Featurized prediction: %f\r\n", printf_float(pred_featurized));
    if (pred_featurized > 0)
    {
        printf("OK\r\n");
    }
    else
    {
        printf("NOK\r\n");
    }

    featurize(&features[0], &features[1], acc_x0, sizeof(acc_x0) / sizeof(acc_x0[0]));
    featurize(&features[2], &features[3], acc_y0, sizeof(acc_y0) / sizeof(acc_y0[0]));
    featurize(&features[4], &features[5], acc_z0, sizeof(acc_z0) / sizeof(acc_z0[0]));
    pred_featurized = score(features);
    printf("Featurized prediction: %f\r\n", printf_float(pred_featurized));
    if (pred_featurized > 0)
    {
        printf("OK\r\n");
    }
    else
    {
        printf("NOK\r\n");
    }

    noprint_output = pred_featurized;
    (void)noprint_output;

    return benchmark_hash_ret;
}
