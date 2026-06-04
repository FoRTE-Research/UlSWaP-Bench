function(set_spike_config)
    message(STATUS "Setting spike configuration")

    set(CMAKE_SYSTEM_NAME Generic)
    set(SUBARCH "rv32im")
    set(ABI "ilp32")

    set(PICOLIBC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/picolibc)
    set(PICOLIBC_INSTALL_DIR ${PICOLIBC_ROOT}/install/spike-${SUBARCH})
    set(LIB_PATH ${PICOLIBC_INSTALL_DIR}/lib)
    set(TOOLCHAIN_PATH /usr/bin)

    execute_process(
        COMMAND riscv64-unknown-elf-gcc -print-libgcc-file-name
        OUTPUT_VARIABLE LIBGCC_FILE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    get_filename_component(RT_LIB_DIR "${LIBGCC_FILE}" DIRECTORY)
    set(RT_LIB_DIR ${RT_LIB_DIR}/../../${SUBARCH}/${ABI})

    set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/clang-15 PARENT_SCOPE)
    set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}/clang-15 PARENT_SCOPE)

    set(GENERAL_FLAGS "-Wall;-fno-builtin;-ffreestanding;-fno-optimize-sibling-calls;-fno-builtin-fma;-ffp-contract=off;-ffunction-sections;-Os")
    set(RISCV_FLAGS "--target=riscv32-unknown-elf;-mno-relax;-march=${SUBARCH};-mabi=${ABI};-DPICOLIBC_FLOAT_PRINTF_SCANF")

    set(ARCH_LINK_DIRS "${LIB_PATH};${RT_LIB_DIR}" PARENT_SCOPE)
    set(ARCH_INC_DIRS "${PICOLIBC_INSTALL_DIR}/include" PARENT_SCOPE)
    set(ARCH_LIBS "libgcc.a" PARENT_SCOPE)

    # Concatenate all flags into a single list
    set(ARCH_FLAGS "${GENERAL_FLAGS};${RISCV_FLAGS};${PICOLIBC_FLAGS}" PARENT_SCOPE)
    set(ARCH_SOURCES "${ARCH_DIR}/vectors.S;${ARCH_DIR}/supportFuncs.c" PARENT_SCOPE)

    set(ARCH_LINK_FLAGS "-fuse-ld=lld;--target=riscv32-unknown-elf;-T${ARCH_DIR}/memmap.ld;-Wl,--gc-sections;-nostdlib;-nodefaultlibs" PARENT_SCOPE)

    set(ARCH_OBJDUMP ${TOOLCHAIN_PATH}/llvm-objdump-15 PARENT_SCOPE)
endfunction()
