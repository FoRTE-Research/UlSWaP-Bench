function (set_arm_config)
    message(STATUS "Setting Arm configuration")

	set(CMAKE_SYSTEM_NAME Generic)
	set(CMAKE_SYSTEM_PROCESSOR arm)

    set(PICOLIBC_ROOT /path/to/picolibc/gcc-arm)
    set(ARM_ROOT $ENV{HOME}/ti/gcc_arm_none_eabi_9_2_1)

    set(CC_PATH ${ARM_ROOT}/bin)

    set(CMAKE_C_COMPILER ${CC_PATH}/arm-none-eabi-gcc-9.2.1 PARENT_SCOPE)
    set(CMAKE_ASM_COMPILER ${CC_PATH}/arm-none-eabi-gcc-9.2.1 PARENT_SCOPE)

    set(GENERAL_FLAGS "-Wall;-Wextra;-Wpedantic;-Wshadow;-Wconversion;-Wpedantic;-Wundef;-Wstrict-prototypes;-ffreestanding;-fno-optimize-sibling-calls;-fno-builtin-fma;-ffp-contract=off;-ffunction-sections")
    set(ARM_FLAGS "-mcpu=cortex-m4;-march=armv7e-m;-mthumb;-mfloat-abi=hard;-mfpu=fpv4-sp-d16;--specs=${PICOLIBC_ROOT}/specs/picolibc.specs")

    set(ARCH_OBJDUMP "${CC_PATH}/arm-none-eabi-objdump" PARENT_SCOPE)

    set(ARCH_LINK_DIRS "${PICOLIBC_ROOT}/lib" PARENT_SCOPE)
    set(ARCH_INC_DIRS "${PICOLIBC_ROOT}/include" PARENT_SCOPE)
	set(ARCH_LIBS "-lc;-ldummyhost;-lgcc" PARENT_SCOPE)

    set(ARCH_FLAGS "${GENERAL_FLAGS};${ARM_FLAGS}" PARENT_SCOPE)
    set(ARCH_LINK_FLAGS "${GENERAL_FLAGS};${ARM_FLAGS};-T${ARCH_DIR}/memmap.lds;-nostartfiles;-Wl,--gc-sections" PARENT_SCOPE)

    set(ARCH_SOURCES "${ARCH_DIR}/supportFuncs.c ${ARCH_DIR}/tm4c123gh6pm_startup_ccs_gcc.c" PARENT_SCOPE)
endfunction()
