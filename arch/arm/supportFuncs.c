#define THUL_UART_BASE 0xE0000000

void putchar(char);
void exit(int);

//unsigned int heapCurrent = 0x40000000;
extern unsigned int end;
extern unsigned int heapCurrent;

void _fini() {}
void __exidx_start() {}
void __exidx_end() {}

void __errno() {}

void PUT32 ( unsigned int, unsigned int);

int _isatty()
{
  return 0;
}
int _fstat()
{
  return 0;
}


void uart_putc ( unsigned int c )
{
    PUT32(THUL_UART_BASE+0x0,c);
}

void uart_send ( unsigned int c )
{
    PUT32(THUL_UART_BASE+0x0,c);
}

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

void puts(char *string)
{
    int index = 0;
    while(string[index] != '\0')
    {
        putchar(string[index]);
        ++index;
    }
}

void * _sbrk(int increment)
{
    heapCurrent += increment;
    
    if(heapCurrent >= end)
    {
        exit(1);
    }
    
    return (void *)heapCurrent;
}

int rand(void)
{
    return 7;
}

int srand(void)
{
    return 11;
}

// Default behavior is for GCC to send printf output here
int _write(int fd, const unsigned char *buf, int count)
{
    int cnt;
    for(cnt = 0; cnt < count; ++cnt)
    {
        putchar(*buf);
	++buf;
    }

    return cnt;
}

void _close()
{
    return;
}

void _read()
{
    return;
}

void _lseek()
{
    return;
}
