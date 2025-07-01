#include <stdio.h>

#include "common.h"
#include "c6h6_model.h" // emlearn generated model
#include "data.h"

uint32_t benchmark_main(void)
{
    uint32_t ret = 0;

    volatile float noprint_output;
    float out;
    int i, j;
    for (i = TESTCASES - 1; i >= 0; i--)
    {
        out = c6h6_model_predict(testFeatures[i], 7);
        for (j = 0; j < 6; j++)
        {
            printf("%hu, ", testFeatures[i][j]);
        }
        printf("%hu: ", testFeatures[i][6]);
        printf("%f\r\n", printf_float(out));
    }

    noprint_output = out;
    (void)noprint_output;

    return ret;
}
