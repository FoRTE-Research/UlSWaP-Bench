#include "../bareBench.h"

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void PUT32 ( unsigned int, unsigned int);


int regression ( void );


#define THUL_UART_BASE 0xE0000000
//------------------------------------------------------------------------
void uart_putc ( unsigned int c )
{
    PUT32(THUL_UART_BASE+0x0,c);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x0D);
    uart_putc(0x0A);
}
//------------------------------------------------------------------------
int main ( void )
{
    regression();
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------

