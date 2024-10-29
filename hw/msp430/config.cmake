function (set_msp430_config)
    message(STATUS "Setting MSP430 configuration")

    set(PICOLIBC_ROOT $ENV{HOME}/picolibc/build-msp430)
    set(MSP430_ROOT $ENV{HOME}/ti/msp430-gcc)

    set(LIB_PATH ${PICOLIBC_ROOT}/build-msp430)
    set(CC_PATH ${MSP430_ROOT}/bin)

    set(CMAKE_C_COMPILER ${CC_PATH}/msp430-elf-gcc PARENT_SCOPE)
    set(CMAKE_ASM_COMPILER ${CC_PATH}/msp430-elf-gcc PARENT_SCOPE)

    set(GENERAL_FLAGS "-Wall;-fno-builtin;-ffreestanding;-fno-optimize-sibling-calls;-fno-builtin-fma;-ffp-contract=off;-ffunction-sections")
    set(MSP430_FLAGS "-mlarge;-mdata-region=upper;-mmcu=msp430fr5994;-mhwmult=none;-specs=${PICOLIBC_ROOT}/picolibc.specs;-T${ARCH_DIR}/memmap.ld;-DCUSTOM_ARCH_STARTUP;-Wl,--gc-sections")

    set(ARCH_OBJDUMP "${CC_PATH}/msp430-elf-objdump" PARENT_SCOPE)

#    set(ARCH_LINK_DIRS "${MSP430_ROOT}/include" PARENT_SCOPE)
    set(ARCH_INC_DIRS "${MSP430_ROOT}/include" PARENT_SCOPE)

    set(ARCH_FLAGS "${GENERAL_FLAGS};${MSP430_FLAGS}" PARENT_SCOPE)
    set(ARCH_LINK_FLAGS "${GENERAL_FLAGS};${MSP430_FLAGS}" PARENT_SCOPE)
    set(ARCH_SOURCES "${ARCH_DIR}/supportFuncs.c;${ARCH_DIR}/vectors.S" PARENT_SCOPE)
endfunction()
