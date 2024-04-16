#include <stdint.h>

static uint32_t _myrand_seed = 1234;

void my_srand(uint32_t new_seed)
{
    _myrand_seed = new_seed;
}

uint32_t my_rand(void)
{
    _myrand_seed = (uint32_t)(1103515245 * _myrand_seed + 12345);
    return _myrand_seed;
}
