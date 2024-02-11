#include <stdio.h>
#include <stdint.h>

void riscv_putchar_uart(char);

static int riscv_putchar(char c, FILE *file)
{
	(void) file;
	riscv_putchar_uart(c);
	return c;
}

// Required by picolibc: https://github.com/picolibc/picolibc/blob/main/doc/os.md
static FILE __stdio = FDEV_SETUP_STREAM(riscv_putchar,
                                        NULL,
                                        NULL,
                                        _FDEV_SETUP_WRITE);
FILE *const stdout = &__stdio; __strong_reference(stdout, stdin); __strong_reference(stdin, stderr);
