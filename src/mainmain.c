#include <stdio.h>
#include <stdint.h>
#include "common.h"

benchmark_hash_t benchmark_main(void);
extern void hexstring(uint32_t num);

int main(void)
{
#ifdef CUSTOM_ARCH_STARTUP
    run_arch_startup();
#endif  // CUSTOM_ARCH_STARTUP

    uint32_t hash = benchmark_main();
    printf("Benchmark execution complete.\r\n");

#if HASH_TEST
    hexstring(hash);
#else  // HASH_TEST
    (void)hash;  // Avoid unused variable warning
#endif  // HASH_TEST

#ifdef CUSTOM_ARCH_FINISH
    run_arch_finish();
#endif  // CUSTOM_ARCH_FINISH

    return 0;
}
