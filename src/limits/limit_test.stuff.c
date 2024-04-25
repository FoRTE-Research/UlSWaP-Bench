#include <stdint.h>

#include "common.h"

// if compiled together with the test code, the compiler optimizer
// optimizes out the use of these funtions and simply puts the right
// answer in the result without doing the work of the test

uint16_t ult(uint16_t a, uint16_t b)
{
    if (a < b)
        return (1);
    return (0);
}

uint16_t slt(int16_t a, int16_t b)
{
    if (a < b)
        return (1);
    return (0);
}

uint16_t ulte(uint16_t a, uint16_t b)
{
    if (a <= b)
        return (1);
    return (0);
}

uint16_t slte(int16_t a, int16_t b)
{
    if (a <= b)
        return (1);
    return (0);
}

uint16_t ugt(uint16_t a, uint16_t b)
{
    if (a > b)
        return (1);
    return (0);
}

uint16_t sgt(int16_t a, int16_t b)
{
    if (a > b)
        return (1);
    return (0);
}

uint16_t ugte(uint16_t a, uint16_t b)
{
    if (a >= b)
        return (1);
    return (0);
}

uint16_t sgte(int16_t a, int16_t b)
{
    if (a >= b)
        return (1);
    return (0);
}
