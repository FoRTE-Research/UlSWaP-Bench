#include <stdio.h>

void hexstring (unsigned int d)
{
    printf("%8X\r\n", d);
}

void hexstrings (unsigned int d)
{
    printf("%8X ", d);
}

void uart_send (unsigned int c)
{
    printf("%c", c);
}
