#include <msp430.h>

.section .resetvec,"ax"
.word rst

.extern _start

.section .rstjmp,"ax"
.global rst
rst:
  MOV #WDTPW|WDTHOLD, &WDTCTL
  jmp _start

.global _exit
_exit:
  jmp .
