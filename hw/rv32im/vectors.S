/* start.S */

.section .text.start
.globl _start
.type _start, @function

_start:
    # Setup the stack
    la sp, __stack_top

    # Zero out BSS
    la t0, __bss_start
    la t1, __bss_end
zero_bss:
    bgeu t0, t1, zero_bss_end
    sw zero, 0(t0)
    addi t0, t0, 4
    j zero_bss
zero_bss_end:

    # Call main, and drop to exit
    call main
    call _exit
hang:
    j hang
    
