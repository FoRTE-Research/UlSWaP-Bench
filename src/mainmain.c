#include <stdio.h>
#include "common.h"

int benchmark_main(void);
extern void hexstring(uint32_t num);

#ifndef RUNS
#define RUNS 1
#endif  // RUNS


int main(void)
{
#ifdef CUSTOM_ARCH_STARTUP
    run_arch_startup();
#endif  // CUSTOM_ARCH_STARTUP

    for (int run = 0; run < RUNS; ++run)
    {
        benchmark_main();
    }

    printf("Benchmark execution complete.\r\n");

#if CHECKSUM_TEST
    uint32_t checksum = get_benchmark_checksum();
    hexstring(checksum);
#endif  // CHECKSUM_TEST

#ifdef CUSTOM_ARCH_FINISH
    run_arch_finish();
#endif  // CUSTOM_ARCH_FINISH

    return 0;
}
