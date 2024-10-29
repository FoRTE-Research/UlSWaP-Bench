function(set_spike_config)
    message(STATUS "Setting spike configuration")

    set(CMAKE_SYSTEM_NAME Generic)

    set(FRANKEN_LLVM_ROOT $ENV{HOME}/llvm-project)
    set(PICOLIBC_ROOT $ENV{HOME}/picolibc)

    set(LIB_PATH ${PICOLIBC_ROOT}/franken-install/picolibc/rv32imafdc-unknown-elf)
    set(CC_PATH ${FRANKEN_LLVM_ROOT}/build/bin)

    set(CMAKE_C_COMPILER ${CC_PATH}/clang PARENT_SCOPE)
    set(CMAKE_ASM_COMPILER ${CC_PATH}/clang PARENT_SCOPE)

    set(GENERAL_FLAGS "-Wall;-fno-builtin;-ffreestanding;-fno-optimize-sibling-calls;-fno-builtin-fma;-ffp-contract=off;-ffunction-sections")
    set(RISCV_FLAGS "--target=riscv32-unknown-elf;-mno-relax;-march=rv32i_ysll_ymul_ydiv;-DPICOLIBC_FLOAT_PRINTF_SCANF")

    set(ARCH_LINK_DIRS "${LIB_PATH}/lib;${FRANKEN_LLVM_ROOT}/build-rt/lib/linux" PARENT_SCOPE)
    set(ARCH_INC_DIRS "${LIB_PATH}/include" PARENT_SCOPE)
    set(ARCH_LIBS "clang_rt.builtins-riscv32.a" PARENT_SCOPE)

    # Concatenate all flags into a single list
    set(ARCH_FLAGS "${GENERAL_FLAGS};${RISCV_FLAGS};${PICOLIBC_FLAGS}" PARENT_SCOPE)
    set(ARCH_SOURCES "${ARCH_DIR}/vectors.S;${ARCH_DIR}/supportFuncs.c" PARENT_SCOPE)

    set(ARCH_LINK_FLAGS "-fuse-ld=lld;--target=riscv32-unknown-elf;-T${ARCH_DIR}/memmap.ld;-Wl,--gc-sections" PARENT_SCOPE)

    set(ARCH_OBJDUMP ${CC_PATH}/llvm-objdump PARENT_SCOPE)

endfunction()
