#include <stdio.h>
#include <stdint.h>

#define SYS_write 64

extern volatile uint64_t tohost;
extern volatile uint64_t fromhost;

static uintptr_t syscall(uintptr_t which, uint64_t arg0, uint64_t arg1, uint64_t arg2)
{
  volatile uint64_t magic_mem[8] __attribute__((aligned(64)));
  magic_mem[0] = which;
  magic_mem[1] = arg0;
  magic_mem[2] = arg1;
  magic_mem[3] = arg2;
  __sync_synchronize();

  tohost = (uintptr_t)magic_mem;
  while (fromhost == 0)
    ;
  fromhost = 0;

  __sync_synchronize();
  return magic_mem[0];
}


static int riscv_putchar_spike(char ch, FILE *file)
{
  static char buf[64] __attribute__((aligned(64)));
  static int buflen = 0;

  buf[buflen++] = ch;

  if (ch == '\n' || buflen == sizeof(buf))
  {
    syscall(SYS_write, 1, (uintptr_t)buf, buflen);
    buflen = 0;
  }

  return 0;
}

// Required by picolibc: https://github.com/picolibc/picolibc/blob/main/doc/os.md
static FILE __stdio = FDEV_SETUP_STREAM(riscv_putchar_spike,
                                        NULL,
                                        NULL,
                                        _FDEV_SETUP_WRITE);
FILE *const stdout = &__stdio; __strong_reference(stdout, stdin); __strong_reference(stdin, stderr);


void hexstring (uint32_t d)
{
    printf("%8X\r\n", d);
}

void hexstrings (uint32_t d)
{
    printf("%8X ", d);
}

void uart_send (uint32_t c)
{
    riscv_putchar_spike(c, NULL);
}

__clock_t clock (void)
{
    return 1234;
}
