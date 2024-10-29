#include <stdio.h>
#include <stdint.h>

void riscv_putchar_uart(char);
void print_uint_uart(uint32_t);

static int riscv_putchar(char c, FILE *file)
{
    (void)file;
    riscv_putchar_uart(c);
    return c;
}

// Required by picolibc: https://github.com/picolibc/picolibc/blob/main/doc/os.md
static FILE __stdio = FDEV_SETUP_STREAM(riscv_putchar,
                                        NULL,
                                        NULL,
                                        _FDEV_SETUP_WRITE);
FILE *const stdout = &__stdio;
__strong_reference(stdout, stdin);
__strong_reference(stdin, stderr);

void print_hexstring(uint32_t num)
{
    for (uint32_t i = sizeof(num) * 8; i > 0; i -= 4)
    {
        uint8_t nibble = (num >> (i - 4)) & 0xF;
        if (nibble > 9)
        {
            riscv_putchar_uart(nibble + 0x37);
        }
        else
        {
            riscv_putchar_uart(nibble + 0x30);
        }
    }
}

void hexstring(uint32_t num)
{
    print_hexstring(num);
    riscv_putchar_uart('\r');
    riscv_putchar_uart('\n');
}

void hexstrings(uint32_t num)
{
    print_hexstring(num);
    riscv_putchar_uart(' ');
}

void uart_send(uint32_t c)
{
    riscv_putchar_uart(c);
}

__clock_t clock(void)
{
    return 1234;
}
