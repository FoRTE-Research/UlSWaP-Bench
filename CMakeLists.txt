cmake_minimum_required(VERSION 3.20)

project(Benchmarks C ASM)

# Make sure the user has specified a hardware configuration file
if(NOT CONFIG)
    message(FATAL_ERROR "Please provide a CMake configuration file for your target platform via the CONFIG variable")
endif()

include(${CONFIG})
cmake_language(CALL set_hw_config)

# Create a list of subdirectories in src
file(GLOB src_paths RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
foreach(src_path ${src_paths})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${src_path})
        list(APPEND benchmarks ${src_path})
    endif()
endforeach()

if (PRINT AND CHECKSUM_TEST)
    message(FATAL_ERROR "Cannot enable both printf and checksum tests simultaneously")
endif()

if (PRINT)
    message(STATUS "Enabling printf")
endif()

if (CHECKSUM_TEST)
    message(STATUS "Enabling checksum tests")
endif()


foreach(benchmark ${benchmarks})
    add_executable(${benchmark})
    add_subdirectory(src/${benchmark})
    set_target_properties(${benchmark} PROPERTIES OUTPUT_NAME ${benchmark}.elf)
    set_target_properties(${benchmark} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/elf)
    target_sources(${benchmark} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/mainmain.c ${ARCH_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/common.c)
    target_include_directories(${benchmark} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

    if (DEBUG)
        target_compile_options(${benchmark} PRIVATE -Og, -g)
    elseif (FAST)
        target_compile_options(${benchmark} PRIVATE -O3)
    else()  # Default optimization level
        target_compile_options(${benchmark} PRIVATE -Os)
    endif()

    if (CHECKSUM_TEST)
        target_compile_definitions(${benchmark} PRIVATE -DCHECKSUM_TEST=1)
        target_sources(${benchmark} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/checksum.c)
        target_link_libraries(${benchmark} m c)
    else()
        target_compile_definitions(${benchmark} PRIVATE -DCHECKSUM_TEST=0)
    endif()

    if (PRINT)
        target_compile_definitions(${benchmark} PRIVATE -DPRINT=1 -DPICOLIBC_FLOAT_PRINTF_SCANF)
    else()
        target_compile_definitions(${benchmark} PRIVATE -DPRINT=0)
    endif()

    target_compile_options(${benchmark} PRIVATE ${ARCH_FLAGS})
    target_compile_options(${benchmark} PRIVATE -ffunction-sections -fdata-sections)
    target_link_options(${benchmark} PRIVATE -Wl,--gc-sections)

    if (DEFINED ARCH_LINK_DIRS)
        target_link_directories(${benchmark} PRIVATE ${ARCH_LINK_DIRS})
    endif()

    if (DEFINED ARCH_INC_DIRS)
        target_include_directories(${benchmark} PRIVATE ${ARCH_INC_DIRS})
    endif()

    if (DEFINED ARCH_LINK_FLAGS)
        target_link_options(${benchmark} PRIVATE ${ARCH_LINK_FLAGS})
    endif()

    if (DEFINED ARCH_LIBS)
        target_link_libraries(${benchmark} ${ARCH_LIBS})
    endif()

    if (DEFINED ARCH_OBJDUMP)
        file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/lst)
        add_custom_command(
            TARGET ${benchmark}
            POST_BUILD
            # DEPENDS ${benchmark}
            COMMENT "Generating disassembly for ${benchmark}"
            COMMAND ${ARCH_OBJDUMP} -d ${PROJECT_BINARY_DIR}/elf/${benchmark}.elf > ${PROJECT_BINARY_DIR}/lst/${benchmark}.lst
        )

        set_target_properties(${benchmark} PROPERTIES ADDITIONAL_CLEAN_FILES ${PROJECT_BINARY_DIR}/lst/${benchmark}.lst)
    endif()

    if (DEFINED ARCH_OBJCOPY)
        file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
        add_custom_command(
            TARGET ${benchmark}
            POST_BUILD
            # DEPENDS ${benchmark}
            COMMENT "Generating binary for ${benchmark}"
            COMMAND ${ARCH_OBJCOPY} -O binary ${PROJECT_BINARY_DIR}/elf/${benchmark}.elf ${PROJECT_BINARY_DIR}/bin/${benchmark}.bin
        )

        set_target_properties(${benchmark} PROPERTIES ADDITIONAL_CLEAN_FILES ${PROJECT_BINARY_DIR}/bin/${benchmark}.bin)
    endif()

    if (DEFINED ARCH_POST_COMMAND)
        string(REPLACE "__BENCHMARK__" ${benchmark} ARCH_POST_COMMAND "${ARCH_POST_COMMAND}")
        add_custom_command(
            TARGET ${benchmark}
            POST_BUILD
            COMMAND ${ARCH_POST_COMMAND}
            COMMAND_EXPAND_LISTS
        )
        # Revert for next iteration
        string(REPLACE ${benchmark} "__BENCHMARK__" ARCH_POST_COMMAND "${ARCH_POST_COMMAND}")
    endif()
endforeach()
