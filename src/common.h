#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <inttypes.h>
#include <stdarg.h>

#undef assert
#define assert(...)

#define ALIGN32 __attribute__((aligned(4)))

#if (!defined(__PICOLIBC__) || (CHECKSUM_TEST == 1))
#undef printf_float
#define printf_float(_num) (_num)
#endif  // __PICOLIBC__

#ifndef HOST_TEST
#define HOST_TEST 0
#endif

#if ((PRINT == 0) && (CHECKSUM_TEST == 0))
#define printf(...)
#endif  // PRINT

#if (CHECKSUM_TEST == 1)
void printf_checksum(const char *fmt, ...);
#define printf(fmt, ...) printf_checksum(fmt, ##__VA_ARGS__)
#endif  // CHECKSUM_TEST

#define TRUE  1
#define FALSE 0

void my_srand(uint32_t new_seed);
uint32_t my_rand(void);
uint32_t get_benchmark_checksum(void);
// void test_checksum(void);        // Uncomment to test printf_checksum
#endif  // COMMON_H
