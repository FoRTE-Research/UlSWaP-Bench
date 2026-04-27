function (set_arm7_config)
    message(STATUS "Setting Arm v7-M configuration")

    set(PICOLIBC_ROOT $ENV{HOME}/picolibc/gcc-arm7)

    set(CMAKE_C_COMPILER arm-none-eabi-gcc PARENT_SCOPE)
    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc PARENT_SCOPE)
    
    set(GENERAL_FLAGS "-nostartfiles;-nostdlib;-ffreestanding;-mthumb;-march=armv7-m;-O2;-T${ARCH_DIR}/memmap.ld")

    set(ARCH_FLAGS "${GENERAL_FLAGS}" PARENT_SCOPE)
    set(ARCH_LINK_FLAGS "${GENERAL_FLAGS}" PARENT_SCOPE)
    set(ARCH_SOURCES "${ARCH_DIR}/supportFuncs.c;${ARCH_DIR}/vectors.S" PARENT_SCOPE)
    
    set(ARCH_INC_DIRS "${PICOLIBC_ROOT}/include" PARENT_SCOPE)
    set(ARCH_LINK_DIRS "/usr/lib/gcc/arm-none-eabi/13.2.1/thumb/v6-m/nofp;${PICOLIBC_ROOT}/lib" PARENT_SCOPE)
    set(ARCH_LIBS "gcc" PARENT_SCOPE)
    
    set(ARCH_OBJDUMP arm-none-eabi-objdump PARENT_SCOPE)
    
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/hex)
    set(ARCH_POST_COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_BINARY_DIR}/bin/__BENCHMARK__.elf ${PROJECT_BINARY_DIR}/hex/__BENCHMARK__.bin PARENT_SCOPE)
endfunction()
