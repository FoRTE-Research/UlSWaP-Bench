
function(set_riscv_config)
    message(STATUS "Setting RISC-V configuration")

    set(CMAKE_SYSTEM_NAME Generic)

    set(FRANKEN_LLVM_ROOT $ENV{HOME}/llvm-project)
    set(PICOLIBC_ROOT $ENV{HOME}/picolibc)

    set(LIB_PATH ${PICOLIBC_ROOT}/franken-install/picolibc/rv32imafdc-unknown-elf)
    set(CC_PATH ${FRANKEN_LLVM_ROOT}/build/bin)

    set(CMAKE_C_COMPILER ${CC_PATH}/clang PARENT_SCOPE)
    set(CMAKE_ASM_COMPILER ${CC_PATH}/clang PARENT_SCOPE)

    set(GENERAL_FLAGS "-O3;-Wall;-DDEBUG=0;-DVERILATOR=0;-fno-builtin;-ffreestanding;-fomit-frame-pointer;-fno-optimize-sibling-calls;-fno-builtin-fma;-ffp-contract=off")
    set(RISCV_FLAGS "--target=riscv32-unknown-elf;-mno-relax;-march=rv32i_ysll_ymul_ydiv;-DPICOLIBC_FLOAT_PRINTF_SCANF")

    set(ARCH_LINK_DIRS "${LIB_PATH}/lib;${FRANKEN_LLVM_ROOT}/build-rt/lib/linux" PARENT_SCOPE)
    set(ARCH_INC_DIRS "${LIB_PATH}/include" PARENT_SCOPE)

    # Concatenate all flags into a single list
    set(ARCH_FLAGS "${GENERAL_FLAGS};${RISCV_FLAGS};${PICOLIBC_FLAGS}" PARENT_SCOPE)
    set(ARCH_SOURCES "${ARCH_DIR}/vectors.S;${ARCH_DIR}/putget.S;${ARCH_DIR}/supportFuncs.c" PARENT_SCOPE)

    set(ARCH_LINK_FLAGS "-fuse-ld=lld;--target=riscv32-unknown-elf;-T${ARCH_DIR}/memmap.ld" PARENT_SCOPE)

endfunction()
