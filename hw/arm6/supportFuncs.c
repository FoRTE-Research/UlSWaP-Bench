#include <stdio.h>
#include <stdint.h>

//__strong_reference(stdout, stdin);
//__strong_reference(stdin, stderr);

//extern int arm32_putchar(char ch, FILE *file);

 __attribute__((noinline))
static int arm32_putchar(char ch, FILE *file) {
    uint8_t *addr = (uint8_t*)0xE0000000;
    *addr = ch;
    return 0;
}


void print_hexstring(uint32_t num)
{
    for (uint32_t i = sizeof(num) * 8; i > 0; i -= 4)
    {
        uint8_t nibble = (num >> (i - 4)) & 0xF;
        if (nibble > 9)
        {
            arm32_putchar(nibble + 0x37, NULL);
        }
        else
        {
            arm32_putchar(nibble + 0x30, NULL);
        }
    }
}

void hexstring(uint32_t num)
{
    print_hexstring(num);
    arm32_putchar('\r', NULL);
    arm32_putchar('\n', NULL);
}

void hexstrings(uint32_t num)
{
    print_hexstring(num);
    arm32_putchar(' ', NULL);
}

void uart_send(uint32_t c) {
    arm32_putchar(c, NULL);
}

static FILE __stdio = FDEV_SETUP_STREAM(arm32_putchar,
                                        NULL,
                                        NULL,
                                        _FDEV_SETUP_WRITE);
FILE *const stdout = &__stdio;
FILE *const stderr = &__stdio;
FILE *const stdin  = &__stdio;

