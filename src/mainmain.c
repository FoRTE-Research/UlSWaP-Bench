#include <stdio.h>
#include <stdint.h>
#include "common.h"

<<<<<<< HEAD
int benchmark_main(void);
extern void hexstring(uint32_t num);

#ifndef RUNS
#define RUNS 1
#endif  // RUNS
=======
benchmark_hash_t benchmark_main(void);
extern void hexstring(uint32_t num);
>>>>>>> 0f048bfb4cc4e859c5425f907fe38ac7c0ba165f


int main(void)
{
#ifdef CUSTOM_ARCH_STARTUP
    run_arch_startup();
#endif  // CUSTOM_ARCH_STARTUP

    uint32_t hash = benchmark_main();
    printf("Benchmark execution complete.\r\n");

<<<<<<< HEAD
#if CHECKSUM_TEST
    uint32_t checksum = get_benchmark_checksum();
    hexstring(checksum);
#endif  // CHECKSUM_TEST
=======
#if HASH_TEST
    hexstring(hash);
#else  // HASH_TEST
    (void)hash;  // Avoid unused variable warning
#endif  // HASH_TEST
>>>>>>> 0f048bfb4cc4e859c5425f907fe38ac7c0ba165f

#ifdef CUSTOM_ARCH_FINISH
    run_arch_finish();
#endif  // CUSTOM_ARCH_FINISH

    return 0;
}
