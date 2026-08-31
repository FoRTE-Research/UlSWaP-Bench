# BENCHMARKS_ROOT is referenced by paths in config.env, so define it first.
get_filename_component(BENCHMARKS_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(CONFIG_ENV_FILE "${CMAKE_CURRENT_LIST_DIR}/config.env")
include(${CMAKE_CURRENT_LIST_DIR}/../read_config.cmake)

function (set_hw_config)
    message(STATUS "Setting tm4c123 configuration")
    message(STATUS "Using toolchain: ${CC}")

    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR arm)

    set(CMAKE_C_COMPILER ${CC} PARENT_SCOPE)
    set(CMAKE_ASM_COMPILER ${CC} PARENT_SCOPE)

    set(GENERAL_FLAGS "-Wextra;-Wshadow;-Wconversion;-Wundef;-Wstrict-prototypes;-ffreestanding;-fno-optimize-sibling-calls;-fno-builtin-fma;-ffp-contract=off;")
    set(ARM_FLAGS "-mcpu=${CPU};-march=${SUBARCH};-mthumb;-mfloat-abi=${FLOAT_ABI};--specs=${PICOLIBC_INSTALL_DIR}/specs/picolibc.specs;-DCUSTOM_ARCH_STARTUP;-DCUSTOM_ARCH_FINISH")

    # Only a build that prints over the UART needs a baud rate
    if (PRINT OR CHECKSUM_TEST)
        message(STATUS "Enabling UART at a baud rate of ${BAUD_RATE}")
        list(APPEND ARM_FLAGS "-DBAUD_RATE=${BAUD_RATE}")
    endif()

    set(ARCH_OBJDUMP "${OBJDUMP}" PARENT_SCOPE)

    set(ARCH_LINK_DIRS "${PICOLIBC_INSTALL_DIR}/lib" PARENT_SCOPE)
    set(ARCH_INC_DIRS "${PICOLIBC_INSTALL_DIR}/include" PARENT_SCOPE)
    set(ARCH_LIBS "-lc;-lgcc" PARENT_SCOPE)

    set(ARCH_FLAGS "${GENERAL_FLAGS};${ARM_FLAGS}" PARENT_SCOPE)
    set(ARCH_LINK_FLAGS "${GENERAL_FLAGS};${ARM_FLAGS};-T${CMAKE_CURRENT_FUNCTION_LIST_DIR}/memmap.lds;-nostartfiles" PARENT_SCOPE)

    set(ARCH_SOURCES "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/supportFuncs.c;${CMAKE_CURRENT_FUNCTION_LIST_DIR}/tm4c123gh6pm_startup_ccs_gcc.c" PARENT_SCOPE)

    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
    set(ARCH_POST_COMMAND ${OBJCOPY} -O binary ${PROJECT_BINARY_DIR}/elf/__BENCHMARK__.elf ${PROJECT_BINARY_DIR}/bin/__BENCHMARK__.bin PARENT_SCOPE)
endfunction()
