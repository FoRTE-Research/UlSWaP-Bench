#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdarg.h>

#if (!defined(PICOLIBC_FLOAT_PRINTF_SCANF) || (CHECKSUM_TEST))
#undef printf_float
#define printf_float(_num) (_num)
#endif  // PICOLIBC_FLOAT_PRINTF_SCANF

#ifndef HOST_TEST
#define HOST_TEST 0
#endif

#if NO_PRINT
#define printf(...)
#elif CHECKSUM_TEST
void printf_checksum(const char *fmt, ...);
#define printf(fmt, ...) printf_checksum(fmt, ##__VA_ARGS__)
#endif  // NO_PRINT

#define TRUE  1
#define FALSE 0

void my_srand(uint32_t new_seed);
uint32_t my_rand(void);
uint32_t get_benchmark_checksum(void);
// void test_checksum(void);        // Uncomment to test printf_checksum
#endif  // COMMON_H
