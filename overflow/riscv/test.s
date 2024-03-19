.section .text

.globl atest
atest:
    li   t0, 0
    add  t1, a0, a1
    bgeu t1, a0, .atest_no_carry
    ori  t0, zero, 2
.atest_no_carry:
    slt  t2, t1, a1
    slti t3, a0, 0
    bne  t2, t3, .atest_overflow
.atest_return:
    mv a0, t0
    ret
.atest_overflow:
    ori  t0, t0, 1
    j .atest_return


.globl stest
stest:
    li  t0, 0
    sub t1, a0, a1
    // Mimic ARM's behavior - C flag is set if op1 >= op2
    bltu a0, a1, .stest_no_carry
    ori  t0, zero, 2
.stest_no_carry:
    addi a0, a0, 1
    slt  t2, a1, a0
    slti t3, t1, 0
    bne  t2, t3, .stest_no_overflow
    ori  t0, t0, 1
.stest_no_overflow:
    mv a0, t0
    ret
