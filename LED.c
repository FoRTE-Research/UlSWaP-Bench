#define GPIO_BASE 0x40004C00
#define BIT_MASK 0b0000000000000001

void initLED();
void LED();

void initLED()  {
    short *base;
    base = GPIO_BASE + 0x4;

    *base |= 0x1;
}

void LED()  {
    unsigned short *base;
    unsigned short value = 0;
    base = GPIO_BASE + 0x2;

    value = *base & BIT_MASK;

    value ^= 0x1;

    *base = (*base & ~BIT_MASK)  | (value & BIT_MASK);

    
}
