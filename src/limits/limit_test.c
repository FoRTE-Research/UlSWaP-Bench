#include <stdint.h>

extern void hexstring(unsigned int d);

extern uint16_t ult(uint16_t a, uint16_t b);
extern uint16_t slt(int16_t a, int16_t b);

extern uint16_t ulte(uint16_t a, uint16_t b);
extern uint16_t slte(int16_t a, int16_t b);

extern uint16_t ugt(uint16_t a, uint16_t b);
extern uint16_t sgt(int16_t a, int16_t b);

extern uint16_t ugte(uint16_t a, uint16_t b);
extern uint16_t sgte(int16_t a, int16_t b);

uint16_t limit_test(void)
{
    uint16_t ra;
    int16_t sa;

    for (ra = 0;; ra++)
        if (ult(ra, 7) == 0)
            break;
    hexstring(ra);
    if (ra != 7)
        return (1);

    for (ra = 0xF000;; ra++)
        if (ult(ra, 0xF007) == 0)
            break;
    hexstring(ra);
    if (ra != 0xF007)
        return (1);

    for (sa = -7;; sa++)
        if (slt(sa, 7) == 0)
            break;
    hexstring(sa);
    if (sa != 7)
        return (1);

    for (sa = -17;; sa++)
        if (slt(sa, -7) == 0)
            break;
    hexstring(sa);
    if (sa != -7)
        return (1);

    for (ra = 0;; ra++)
        if (ulte(ra, 7) == 0)
            break;
    hexstring(ra);
    if (ra != 8)
        return (1);

    for (ra = 0xF000;; ra++)
        if (ulte(ra, 0xF007) == 0)
            break;
    hexstring(ra);
    if (ra != 0xF008)
        return (1);

    for (ra = 0xF000;; ra++)
        if (ulte(ra, 7) == 0)
            break;
    hexstring(ra);
    if (ra != 0xF000)
        return (1);

    for (sa = -7;; sa++)
        if (slte(sa, 7) == 0)
            break;
    hexstring(sa);
    if (sa != 8)
        return (1);

    for (sa = -17;; sa++)
        if (slte(sa, -7) == 0)
            break;
    hexstring(sa);
    if (sa != -6)
        return (1);

    for (ra = 0;; ra++)
        if (ugt(ra, 7))
            break;
    hexstring(ra);
    if (ra != 8)
        return (1);

    for (ra = 0xF000;; ra++)
        if (ugt(ra, 0xF007))
            break;
    hexstring(ra);
    if (ra != 0xF008)
        return (1);

    for (ra = 0;; ra++)
        if (ugte(ra, 7))
            break;
    hexstring(ra);
    if (ra != 7)
        return (1);

    for (ra = 0xF000;; ra++)
        if (ugte(ra, 0xF007))
            break;
    hexstring(ra);
    if (ra != 0xF007)
        return (1);

    return (0);
}

int benchmark_main(void)
{
    hexstring(0x1234);

    if (limit_test())
        hexstring(0xBAD);

    hexstring(0x1234);
    return (0);
}
