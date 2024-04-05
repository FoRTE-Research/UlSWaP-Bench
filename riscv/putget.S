.section .text
.globl riscv_putchar_uart
.globl print_uint_uart

.equ IO_UART_DAT, 8
.equ IO_UART_CNTL, 16

riscv_putchar_uart:
    sw a0, IO_UART_DAT(gp)
    li t0, 1<<9
.L0:
    lw t1, IO_UART_CNTL(gp)
    and t1, t1, t0
    bnez t1, .L0
    ret


.if DEBUG
.macro wait_uart_ready
1:
    lw t1, IO_UART_CNTL(gp)
    andi t1, t1, 1<<9
    bnez t1, 1b
.endm

print_uint_uart:
    srli t1, a0, 24
    sw t1, IO_UART_DAT(gp)
    wait_uart_ready
    srli t1, a0, 16
    sw t1, IO_UART_DAT(gp)
    wait_uart_ready
    srli t1, a0, 8
    sw t1, IO_UART_DAT(gp)
    wait_uart_ready
    srli t1, a0, 0
    sw t1, IO_UART_DAT(gp)
    wait_uart_ready
    ret

.endif  # DEBUG
