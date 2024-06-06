cmake_minimum_required(VERSION 3.20)

project(Benchmarks C ASM)

# Create a list of available architectures
file(GLOB available_architectures RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/arch ${CMAKE_CURRENT_SOURCE_DIR}/arch/*)

# Make sure the user has specified an architecture
if(NOT ARCH)
    message(FATAL_ERROR "Please specify an architecture to build the benchmarks for")
    message("Available architectures: ${available_architectures}")
endif()

# Make sure all specified architectures are valid
if(NOT ${ARCH} IN_LIST available_architectures)
    message(FATAL_ERROR "Architecture ${arch} is not available")
else()
    include(arch/${ARCH}/config.cmake)
    set(ARCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH})
    cmake_language(CALL set_${ARCH}_config)
endif()

# Create a list of subdirectories in src
file(GLOB src_paths RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
foreach(src_path ${src_paths})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${src_path})
        list(APPEND available_benchmarks ${src_path})
    endif()
endforeach()

# If a specific benchmark is requested, only build that one, otherwise build all available benchmarks
if(BENCHMARKS)
    set(benchmarks ${BENCHMARKS})
else()
    set(benchmarks ${available_benchmarks})
endif()

# Add all benchmarks to the build
foreach(benchmark ${benchmarks})
    # First make sure the benchmark exists
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/${benchmark})
        message(FATAL_ERROR "Benchmark ${benchmark} does not exist")
        message("Available benchmarks: ${available_benchmarks}")
    endif()
endforeach()

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/lst)

message("Benchmarks to build: ${benchmarks}")

foreach(benchmark ${benchmarks})
    add_executable(${benchmark})
    add_subdirectory(src/${benchmark})
    set_target_properties(${benchmark} PROPERTIES OUTPUT_NAME ${benchmark}.elf)
    set_target_properties(${benchmark} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
    target_sources(${benchmark} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/mainmain.c ${ARCH_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/common.c)
    target_include_directories(${benchmark} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

    # Check if LOWER_OPT_LEVEL is set for the benchmark
    get_property(BENCH_LOWER_OPT_LEVEL TARGET ${benchmark} PROPERTY LOWER_OPT_LEVEL)
    if (BENCH_LOWER_OPT_LEVEL EQUAL "1")
        message(STATUS "Setting lower optimization level for ${benchmark}")
        target_compile_options(${benchmark} PRIVATE -O1)
    else()
        target_compile_options(${benchmark} PRIVATE -O3)
    endif()

    target_compile_options(${benchmark} PRIVATE ${ARCH_FLAGS})

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
        add_custom_command(
            TARGET ${benchmark}
            POST_BUILD
            DEPENDS ${benchmark}
            COMMENT "Generating disassembly for ${benchmark}"
            COMMAND ${ARCH_OBJDUMP} -d ${PROJECT_BINARY_DIR}/bin/${benchmark}.elf > ${PROJECT_BINARY_DIR}/lst/${benchmark}.lst
        )

        set_target_properties(${benchmark} PROPERTIES ADDITIONAL_CLEAN_FILES ${PROJECT_BINARY_DIR}/lst/${benchmark}.lst)
    endif()
endforeach()
