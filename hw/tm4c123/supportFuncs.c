#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* System Control */
#define SYSCTL_RCGCUART_R (*((volatile uint32_t *)0x400FE618))
#define SYSCTL_RCGCGPIO_R (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_PRUART_R   (*((volatile uint32_t *)0x400FEA18))
#define SYSCTL_PRGPIO_R   (*((volatile uint32_t *)0x400FEA08))

/* RCGCGPIO/PRGPIO bit positions */
#define GPIO_PORTA_CLK (1 << 0)
#define GPIO_PORTF_CLK (1 << 5)

/* GPIO Port A */
#define GPIO_PORTA_BASE 0x40004000
#define GPIO_PORTA_AFSEL_R (*((volatile uint32_t *)(GPIO_PORTA_BASE + 0x420)))
#define GPIO_PORTA_DEN_R   (*((volatile uint32_t *)(GPIO_PORTA_BASE + 0x51C)))
#define GPIO_PORTA_PCTL_R  (*((volatile uint32_t *)(GPIO_PORTA_BASE + 0x52C)))

// PA2
#define GPIO_PORTA_DIR_R   (*((volatile uint32_t *)(GPIO_PORTA_BASE + 0x400)))
#define GPIO_PORTA_DATA_R  (*((volatile uint32_t *)(GPIO_PORTA_BASE + 0x3FC)))

/* UART0 */
#define UART0_BASE 0x4000C000
#define UART_DR_R   (*((volatile uint32_t *)(UART0_BASE + 0x000)))
#define UART_FR_R   (*((volatile uint32_t *)(UART0_BASE + 0x018)))
#define UART_IBRD_R (*((volatile uint32_t *)(UART0_BASE + 0x024)))
#define UART_FBRD_R (*((volatile uint32_t *)(UART0_BASE + 0x028)))
#define UART_LCRH_R (*((volatile uint32_t *)(UART0_BASE + 0x02C)))
#define UART_CTL_R  (*((volatile uint32_t *)(UART0_BASE + 0x030)))

#define UART_ENABLED ((PRINT == 1) || (CHECKSUM_TEST == 1))
#if UART_ENABLED
/* Raw PIOSC. RCC's USESYSDIV is 0 out of reset, so no divider is applied and
   the UART is clocked straight off the 16 MHz precision oscillator. */
#define SYS_CLOCK_HZ 16000000u

/* Baud rate divisors, per TM4C123 datasheet 14.3.2:
     BRD = SYS_CLOCK_HZ / (16 * BAUD_RATE)
   UARTIBRD takes the integer part and UARTFBRD the fraction scaled by 64 and
   rounded. Working in 64ths of a divisor up front keeps this to integer
   arithmetic and lets a fraction that rounds up to 64/64 carry into the integer
   part on its own, instead of overflowing the 6-bit UARTFBRD field.
   64/16 == 4, hence the *4.
*/
// Defined in config.env, which is also read by the test script.
#ifndef BAUD_RATE
#error "BAUD_RATE must be defined by the build (see hw/tm4c123/config.env)"
#endif
#define UART_BRD_64 (((SYS_CLOCK_HZ * 4u) + (BAUD_RATE / 2u)) / BAUD_RATE)
#define UART_IBRD   (UART_BRD_64 / 64u)
#define UART_FBRD   (UART_BRD_64 % 64u)

_Static_assert(UART_IBRD >= 1u && UART_IBRD <= 65535u,
               "BAUD_RATE is out of range for SYS_CLOCK_HZ");

#endif  // UART_ENABLED

/* UART flags */
#define UART_FR_TXFF (1 << 5)

/* GPIO Port F base */
#define GPIO_PORTF_BASE 0x40025000

/* GPIO registers (offsets) */
#define GPIO_PORTF_DIR_R    (*((volatile uint32_t *)(GPIO_PORTF_BASE + 0x400)))
#define GPIO_PORTF_DEN_R    (*((volatile uint32_t *)(0x4002551C)))
#define GPIO_PORTF_DATA_R   (*((volatile uint32_t *)(GPIO_PORTF_BASE + 0x3FC)))

/* Output pins */
#define LED_BLUE (1u << 2)
#define START_STOP_MARKER (1u << 2)  // PA2

/* lm4flash releases reset the instant flashing finishes, so output starts
   before the host has finished opening the port. Hold off a little. This runs
   before the start marker is asserted, so it sits outside the measured
   region. ~250 ms at 16 MHz. */
#define UART_SETTLE_LOOPS 400000u

/* Even with that delay, a fixed 8-byte chunk -- one TX FIFO load -- is
   reliably discarded from the head of the stream. The loss is quantized by
   byte count, not by time: dropping the line rate from 115200 to 9600 stretches
   those 8 bytes from 0.7 ms to 8.3 ms of wire time and still loses exactly 8,
   so it is a FIFO flush somewhere between here and the host tty rather than a
   receiver overrun or a window where nobody was listening. Rather than chase
   which side flushes, emit an expendable preamble for it to eat. The harness
   discards everything through UART_SYNC_MARKER before comparing. */
#define UART_PREAMBLE_FILL  32
#define UART_SYNC_MARKER    "<<<BENCHMARK_START>>>"

/* A peripheral's registers bus-fault (-> hard fault) until its clock is
   enabled and the peripheral reports ready, so every block must come
   through here before any of its registers are touched. */
static void gpio_clock_enable(uint32_t port_clk)
{
    SYSCTL_RCGCGPIO_R |= port_clk;
    while ((SYSCTL_PRGPIO_R & port_clk) == 0);
}

#if UART_ENABLED

void uart_init(void) 
{
    /* Enable clocks */
    SYSCTL_RCGCUART_R |= (1 << 0); // UART0
    while ((SYSCTL_PRUART_R & (1 << 0)) == 0);

    /* PA0/PA1 are the UART0 pins, so Port A must be clocked too */
    gpio_clock_enable(GPIO_PORTA_CLK);

    /* Keep the UART off while it and its pins are configured */
    UART_CTL_R &= ~(1u << 0);

    /* Baud rate divisors for BAUD_RATE at SYS_CLOCK_HZ */
    UART_IBRD_R = UART_IBRD;
    UART_FBRD_R = UART_FBRD;

    /* 8-bit, no parity, 1 stop bit, FIFO enabled.
       Must be written after the baud rate divisors are set. */
    UART_LCRH_R = (0x3 << 5) | (1 << 4);

    /* Mux PA0/PA1 to UART0 (function 1) before enabling the digital driver, so
       U0Tx is already the pin's signal source the moment it starts driving.
       Setting AFSEL/DEN while PCTL is still 0 leaves the alternate function
       unassigned and can glitch the line low, which the receiver reads as a
       start bit and mis-frames on. */
    GPIO_PORTA_PCTL_R  = (GPIO_PORTA_PCTL_R & ~0x000000FFu) | 0x00000011u;
    GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1);
    GPIO_PORTA_DEN_R   |= (1 << 0) | (1 << 1);

    /* Enable UART0, TX, RX only once the pins are settled */
    UART_CTL_R = (1u << 0) | (1u << 8) | (1u << 9);
}

/* Signature is dictated by picolibc's FILE.put hook */
static int tivac_putchar(char c, FILE *stream)
{
    (void)stream;

    /* Wait until TX FIFO is not full */
    while (UART_FR_R & UART_FR_TXFF);

    UART_DR_R = (uint32_t)(unsigned char)c;

    return 0;
}

/* Sacrificial filler + a marker the harness can resynchronize on. The filler is
   comfortably longer than the 8 bytes that go missing, so the marker itself
   always survives intact. */
static void uart_emit_sync_preamble(void)
{
    for (int i = 0; i < UART_PREAMBLE_FILL; i++)
    {
        tivac_putchar('.', NULL);
    }

    for (const char *p = UART_SYNC_MARKER; *p != '\0'; p++)
    {
        tivac_putchar(*p, NULL);
    }

    tivac_putchar('\r', NULL);
    tivac_putchar('\n', NULL);
}

// Required by picolibc: https://github.com/picolibc/picolibc/blob/main/doc/os.md
static FILE __stdio = FDEV_SETUP_STREAM(tivac_putchar,
                                        NULL,
                                        NULL,
                                        _FDEV_SETUP_WRITE);
FILE *const stdout = &__stdio;
__strong_reference(stdout, stdin);
__strong_reference(stdin, stderr);

#endif  // UART_ENABLED


void _exit(int status)
{
    (void)status;
    while(1);
}

void run_arch_startup(void)
{
#if UART_ENABLED
    uart_init();
    for (volatile uint32_t i = 0; i < UART_SETTLE_LOOPS; i++);
    uart_emit_sync_preamble();
#endif  // UART_ENABLED

    gpio_clock_enable(GPIO_PORTA_CLK);
    gpio_clock_enable(GPIO_PORTF_CLK);

    // Delay to allow time for pins to be observed with a logic analyzer after reset
    for (volatile int i = 0; i < 1000; i++);

    // Turn on blue LED to indicate startup
    GPIO_PORTF_DIR_R |= LED_BLUE;
    GPIO_PORTF_DEN_R |= LED_BLUE;
    GPIO_PORTF_DATA_R |= LED_BLUE;

    // Same for PA2
    GPIO_PORTA_DIR_R |= START_STOP_MARKER;
    GPIO_PORTA_DEN_R |= START_STOP_MARKER;
    GPIO_PORTA_DATA_R |= START_STOP_MARKER;
}

void run_arch_finish(void)
{
    // Turn off blue LED and PA2 to indicate finish
    GPIO_PORTF_DATA_R &= ~LED_BLUE;
    GPIO_PORTA_DATA_R &= ~START_STOP_MARKER;
}


void print_hexstring(uint32_t num)
{
    for (uint32_t i = sizeof(num) * 8; i > 0; i -= 4)
    {
        uint8_t nibble = (num >> (i - 4)) & 0xF;
        if (nibble > 9)
        {
            tivac_putchar(nibble + 0x37, NULL);
        }
        else
        {
            tivac_putchar(nibble + 0x30, NULL);
        }
    }
}

void hexstring(uint32_t num)
{
    print_hexstring(num);
    tivac_putchar('\r', NULL);
    tivac_putchar('\n', NULL);
}
