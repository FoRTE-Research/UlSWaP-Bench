#include <stdint.h>
#include <common.h>

static uint32_t _myrand_seed = 1234;

void my_srand(uint32_t new_seed)
{
    _myrand_seed = new_seed;
}

uint32_t my_rand(void)
{
    _myrand_seed = (_myrand_seed >> 1) ^
                   (uint32_t)(-(uint32_t)(_myrand_seed & UINT32_C(1)) & UINT32_C(0xD0000001));
    return _myrand_seed;
}
