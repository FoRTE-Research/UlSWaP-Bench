#include <msp430.h>
#include <stdio.h>
#include <stddef.h>

static int sendByte(char byte, FILE *file);

// Basic clock startup for 8MHz (max freq without FRAM wait states)
void initClocks(void){
  // Startup clock system with max DCO setting ~8MHz
  CSCTL0_H = CSKEY_H;                     // Unlock CS registers
  CSCTL1 = DCOFSEL_3 | DCORSEL;           // Set DCO to 8MHz
  CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers
  CSCTL0_H = 0;                           // Lock CS registers
}

// Basic UART init for output
void initUart(void){
  // Configure USCI_A0 GPIOs
  P2SEL1 |= (BIT0 | BIT1);
  P2SEL0 &= ~(BIT0 | BIT1);       // USCI_A0 UART operation

  // Configure USCI_A0 for UART mode
  UCA0CTLW0 = UCSWRST;                    // Put eUSCI in reset
  UCA0CTLW0 |= UCSSEL__SMCLK;             // CLK = SMCLK
  // Baud Rate calculation
  // 8000000/(16*9600) = 52.083
  // Fractional portion = 0.083
  // User's Guide Table 21-4: UCBRSx = 0x04
  // UCBRFx = int ( (52.083-52)*16) = 1
  UCA0BRW = 52;                           // 8000000/16/9600
  UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
  UCA0CTLW0 &= ~UCSWRST;                  // Initialize eUSCI
  UCA0IE |= UCRXIE;                       // Enable USCI_A0 RX interrupt
}

// UART tx interrupt handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=EUSCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(EUSCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            while(!(UCA0IFG&UCTXIFG));
            __no_operation();
            break;
        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;
        default: break;
    }
}

void run_arch_startup(){

  // Stop watchdog
  WDTCTL = WDTPW | WDTHOLD;

  // Peripheral init
  initClocks();
  initUart();

  // Disable GPIO high-z
  PM5CTL0 &= ~LOCKLPM5;

  // Enable interrupts
  __bis_SR_register(GIE);
}

void print_hexstring(uint32_t num){
  for (uint32_t i = sizeof(num) * 8; i > 0; i -= 4){
    uint8_t nibble = (num >> (i - 4)) & 0xF;
    if (nibble > 9){
      sendByte(nibble + 0x37, NULL);
    }else{
      sendByte(nibble + 0x30, NULL);
    }
  }
}

void hexstring(uint32_t num){
  print_hexstring(num);
  sendByte('\r', NULL);
  sendByte('\n', NULL);
}

void hexstrings(uint32_t num){
  print_hexstring(num);
  sendByte(' ', NULL);
}

// Set up handles for picolibc stdout
static int sendByte(char byte, FILE *file){
  (void) file;
  while(!(UCA0IFG&UCTXIFG));
  UCA0TXBUF = byte;
  return byte;
}

static FILE __stdio = FDEV_SETUP_STREAM(sendByte, NULL, NULL, _FDEV_SETUP_WRITE);
FILE *const stdout = &__stdio;
__strong_reference(stdout, stdin);
__strong_reference(stdin, stderr);

