function (set_rv32im_config)
    message(STATUS "Setting RISC-V 32 IM configuration")

    set(PICOLIBC_ROOT $ENV{HOME}/picolibc/rv32im)

    set(CMAKE_C_COMPILER riscv64-unknown-elf-gcc PARENT_SCOPE)
    set(CMAKE_ASM_COMPILER riscv64-unknown-elf-gcc PARENT_SCOPE)
    
    set(GENERAL_FLAGS "-nostartfiles;-nostdlib;-ffreestanding;-march=rv32im;-mabi=ilp32;-O2;-T${ARCH_DIR}/memmap.ld")

    set(ARCH_FLAGS "${GENERAL_FLAGS}" PARENT_SCOPE)
    set(ARCH_LINK_FLAGS "${GENERAL_FLAGS}" PARENT_SCOPE)
    set(ARCH_SOURCES "${ARCH_DIR}/supportFuncs.c;${ARCH_DIR}/vectors.S" PARENT_SCOPE)
    
    set(ARCH_INC_DIRS "${PICOLIBC_ROOT}/include" PARENT_SCOPE)
    set(ARCH_LINK_DIRS "/usr/lib/gcc/riscv64-unknown-elf/13.2.0/rv32im/ilp32;${PICOLIBC_ROOT}/lib" PARENT_SCOPE)
    set(ARCH_LIBS "gcc" PARENT_SCOPE)
    
    set(ARCH_OBJDUMP riscv64-unknown-elf-objdump PARENT_SCOPE)
    
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/hex)
    set(ARCH_POST_COMMAND riscv64-unknown-elf-objcopy -O binary ${PROJECT_BINARY_DIR}/bin/__BENCHMARK__.elf ${PROJECT_BINARY_DIR}/hex/__BENCHMARK__.bin PARENT_SCOPE)
endfunction()
