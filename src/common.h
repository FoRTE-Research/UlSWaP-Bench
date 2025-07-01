#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#ifndef PICOLIBC_FLOAT_PRINTF_SCANF
#define printf_float(_num) _num
#endif  // PICOLIBC_FLOAT_PRINTF_SCANF

#if NO_PRINT
#define printf(...)
#endif  // NO_PRINT

#ifndef HOST_TEST
#define HOST_TEST 0
#endif

#if HASH_TEST
#include "hash.h"
#endif  // HASH_TEST

typedef uint32_t benchmark_hash_t;


#define TRUE  1
#define FALSE 0

void my_srand(uint32_t new_seed);
uint32_t my_rand(void);

#endif  // COMMON_H
