#include <stdio.h>
#include <stdint.h>

#define IO_DISPLAY 0x10000000
#define IO_HALT 0x100000

 __attribute__((noinline))
int riscv_putchar(char ch, FILE *file) {
    (void)file;
    uint8_t *addr = (uint8_t*)IO_DISPLAY;
    *addr = ch;
    return 0;
}

 __attribute__((noinline))
void _exit() {
    uint32_t *addr = (uint32_t*)IO_HALT;
    *addr = 0x5555;
}

static FILE __stdio = FDEV_SETUP_STREAM(riscv_putchar,
                                        NULL,
                                        NULL,
                                        _FDEV_SETUP_WRITE);
FILE *const stdout = &__stdio;
FILE *const stderr = &__stdio;
FILE *const stdin  = &__stdio;

//__strong_reference(stdout, stdin);
//__strong_reference(stdin, stderr);

void print_hexstring(uint32_t num)
{
    for (uint32_t i = sizeof(num) * 8; i > 0; i -= 4)
    {
        uint8_t nibble = (num >> (i - 4)) & 0xF;
        if (nibble > 9)
        {
            riscv_putchar(nibble + 0x37, NULL);
        }
        else
        {
            riscv_putchar(nibble + 0x30, NULL);
        }
    }
}

void hexstring(uint32_t num)
{
    print_hexstring(num);
    riscv_putchar('\r', NULL);
    riscv_putchar('\n', NULL);
}

void hexstrings(uint32_t num)
{
    print_hexstring(num);
    riscv_putchar(' ', NULL);
}

void uart_send(uint32_t c) {
    riscv_putchar(c, NULL);
}

