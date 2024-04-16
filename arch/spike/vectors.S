.section .text
.globl _start
_start:
    # lui gp, %hi(IO_BASE)
    lui sp, %hi(_stacktop)
    addi sp, sp, %lo(_stacktop)
    call main
    j _exit


.globl _exit
_exit:
    slli a0, a0, 1
    ori a0, a0, 1

    la t0, tohost
    sw a0, 0(t0)

    1: j 1b


.section ".tohost","aw",@progbits
.align 6
.globl tohost
tohost: .dword 0
.align 6
.globl fromhost
fromhost: .dword 0
