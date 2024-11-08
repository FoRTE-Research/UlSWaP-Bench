# UlSWaP Bench
UlSWaP Bench (pronounced you'll swap bench) is a benchmark suite for ultra-low size, weight and power devices.

## Prerequisites
You will need [CMake](https://cmake.org/download/) as well as the cross-compiler for your target platform.

## Building
    cmake . -B <build_directory> -DARCH=<arch> [-DNO_PRINT]
    cmake --build <build_directory> -j`nproc`

`arch` must be a subdirectory within the hw/ directory.
The -DNO_PRINT option will disable all printing via printf.

To build a subset of benchmarks:

    cmake --build <build_directory> -j`nproc` --target "benchmark1;benchmark2"

The executable ELF files will be in <build_directory>/bin.

<!-- ### Other build options -->

## Currently supported hardware
### Native
The "native" hardware is intended primarily for cross-testing.
It uses the host's default C compiler and libraries.
So far, this has only been tested on an x64 Linux host.
If `-DHOST_TEST=1` is provided, some of the signal processing benchmarks will output a file that can be used to verify the correctness of the benchmark's implementation.

### RISC-V
The "riscv" hardware targeted here is a custom core available at <>, and uses a custom compiler available at <>.
[Picolibc](https://github.com/picolibc/picolibc) is used as the C library.

### Spike
"spike" targets the [Spike](https://github.com/riscv-software-src/riscv-isa-sim) RISC-V simulator.
It also uses a custom version of Clang and links against Picolibc.

### MSP430
Targets the MSP430FR5994 microcontroller.
GCC is the compiler and Picolibc is the C library.

<!-- ### ARM
Targets the TivaC ... -->

## Adding a new hardware platform
To support a new hardware platform, create a new subdirectory inside hw/, and add a file named config.cmake to it.
This file sets the C compiler, libraries, compiler/linker flags, linker script, post-build commands, etc. for the given hardware.
These variables need to be set with `PARENT_SCOPE` so they can be picked up by the parent CMakeLists.txt.
Important variables that can be set include (not all are necessary):
1. CMAKE_C_COMPILER - The C compiler.
2. CMAKE_ASM_COMPILER - The assembly compiler.
3. ARCH_LINK_DIRS - The location(s) of the C library and/or hardware-specific libraries to link against.
4. ARCH_INC_DIRS - The locations(s) of C library and/or hardware-specific header files.
5. ARCH_FLAGS - Hardware-specific compiler flags. This will probably include the location of the linker script.
6. ARCH_LINK_FLAGS - Hardware-specific linker flags.
7. ARCH_SOURCES - Hardware-specific source files. These will probably be a combination of C and assembly files with functions for printing, entry, etc.
8. ARCH_OBJDUMP - The objdump executable for this hardware. If set, the object dump for each benchmark executable will be sent to <build_directory>/lst.
9. ARCH_POST_COMMAND - A post-build command to run per benchmark.

See riscv/config.cmake for an example that uses all of the above variables, and native/config.cmake that sets the minimum required variables.
All other hardware-specific files should be located in the same directory.

## Testing
To test your hardware platform, compile the benchmarks with printing enabled, and compare the output to the gold reference located at test/reference_output.
The benchmarks are designed to print meaningful output that can be used for testing purposes.
The reference outputs have been captured on an x64 Linux platform.
The check_output.py script can be used for comparison, and the `-d` option can be used to set the maximum allowable difference for floating point numbers (default=0.0001).

## Analysis
There are a number of analysis scripts in the tools/analysis directory.
Run them with the `-h` argument to find out what they do and the arguments they expect.

## Directory layout
The repository is organised as follows:

    .
    ├── hw/                     # Hardware-specific source and build files
    |   ├── arm/
    |   ├── msp430/
    |   ├── native/
    |   ├── riscv/
    |   ├── spike/
    ├── src/                    # Benchmark source code (one folder per benchmark)
    ├── test/
    |   ├── check_output.py
    |   ├── reference_output/   # Gold reference output for each benchmark
    ├── tools/
    |   ├── analysis/           # Scripts to analyse the benchmarks
    |   ├── input_gen/          # Scripts to generate input for various benchmarks
