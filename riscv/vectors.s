/* vectors.s */

.extern IO_BASE
.extern _stacktop

.global _start
_start:
    lui gp, %hi(IO_BASE)
    lui sp, %hi(_stacktop)
    addi sp, sp, %lo(_stacktop)
    call main
    j _exit

.global _exit
_exit:
.ifdef DEBUG
    call print_uint_uart
.endif  // DEBUG
    j .
.end
