function (set_msp430fr5994_config)
    message(STATUS "Setting MSP430 configuration")

    set(PICOLIBC_ROOT $ENV{HOME}/Projects/picolibc/gcc-msp430)
    set(MSP430_ROOT /opt/msp430-gcc)

    set(CC_PATH ${MSP430_ROOT}/bin)
    
    set(COMPILER "DEFAULT" CACHE PATH "The compiler to build these benchmarks")
    if (COMPILER STREQUAL "DEFAULT")
        message(STATUS "Using default MSP430 compiler")
        set(CMAKE_C_COMPILER ${CC_PATH}/msp430-elf-gcc PARENT_SCOPE)
        set(CMAKE_ASM_COMPILER ${CC_PATH}/msp430-elf-gcc PARENT_SCOPE)
    else()
        message(STATUS "Using custom compiler: ${COMPILER}")
        set(CMAKE_C_COMPILER "${COMPILER}" PARENT_SCOPE)
        set(CMAKE_ASM_COMPILER "${COMPILER}" PARENT_SCOPE)
    endif()
    
    set(LINKER_SCRIPT "-T${ARCH_DIR}/memmap.ld")
    if (CUSTOM_MEMMAP)
        set(LINKER_SCRIPT "-T${CUSTOM_MEMMAP}")
    endif()
    
    #set(EXTRA_FLAGS "-Wpedantic;-Wshadow;-Wconversion;-Wpedantic;")
    set(EXTRA_FLAGS "")
    set(GENERAL_FLAGS "${EXTRA_FLAGS};-fno-builtin;-ffreestanding;-fno-optimize-sibling-calls;-fno-builtin-fma;-ffp-contract=off;-ffunction-sections")
    set(MSP430_FLAGS "-mlarge;-mdata-region=upper;-mmcu=msp430fr5994;-mhwmult=none;-specs=${PICOLIBC_ROOT}/picolibc.specs;${LINKER_SCRIPT};-DCUSTOM_ARCH_STARTUP;-Wl,--gc-sections")
    
    separate_arguments(OPT_FLAGS NATIVE_COMMAND "${OPT_FLAGS}")
    separate_arguments(LINK_DIRS NATIVE_COMMAND "${LINK_DIRS}")
    separate_arguments(LINK_LIBS NATIVE_COMMAND "${LINK_LIBS}")

    set(ARCH_OBJDUMP "${CC_PATH}/msp430-elf-objdump" PARENT_SCOPE)

#    set(ARCH_LINK_DIRS "${MSP430_ROOT}/include" PARENT_SCOPE)
    set(ARCH_INC_DIRS "${MSP430_ROOT}/include" PARENT_SCOPE)

    set(ARCH_FLAGS "${GENERAL_FLAGS};${MSP430_FLAGS};${OPT_FLAGS}" PARENT_SCOPE)
    set(ARCH_LINK_FLAGS "${GENERAL_FLAGS};${MSP430_FLAGS};${OPT_FLAGS}" PARENT_SCOPE)
    set(ARCH_LINK_DIRS "${LINK_DIRS}" PARENT_SCOPE)
    set(ARCH_LIBS "${LINK_LIBS}" PARENT_SCOPE)
    set(ARCH_SOURCES "${ARCH_DIR}/supportFuncs.c;${ARCH_DIR}/vectors.S" PARENT_SCOPE)
    
    set(ARCH_OBJDUMP ${CC_PATH}/msp430-elf-objdump PARENT_SCOPE)
    
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/hex)
    set(ARCH_POST_COMMAND ${CC_PATH}/msp430-elf-objcopy -O binary ${PROJECT_BINARY_DIR}/bin/__BENCHMARK__.elf ${PROJECT_BINARY_DIR}/hex/__BENCHMARK__.bin PARENT_SCOPE)
endfunction()
