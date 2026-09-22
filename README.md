# UlSWaP Bench
UlSWaP Bench (pronounced you'll swap bench) is a benchmark suite for ultra-low size, weight and power devices.
UlSWaP Bench is the next iteration of benchmarks targeting ultra-low size, weight, and power emebedded systems.
It takes the improvements of MiBench2 over MiBench, cleans everything up, simplifies the build environment, adds new classes of benchmarks, adjusts input sizes to work on memory-limited devices, adds self-check functionality to verify correct operation for each benchmark, and adds builds for RISC-V, ARM, and MSP430 architectures.
UlSWaP-Bench has been verified both on real devices and on existing ISA simulators.

UlSWaP Bench contains 28 benchmarks across 5 categories of embedded use cases - security, communication, signal processing, AI and general compuatation.
Benchmarks are split into two categories based on their memory requirements - Tiny (< 8 KiB RAM/64 KiB ROM) and Small (< 32 KiB RAM/256 KiB ROM).
Six benchmarks fall into the Small class - mp3_encode, susan_edges, susan_corners, susan_smooth, image_class and anomaly.
See below for a more detailed breakdown of the memory requirements of each benchmark.

If you want to use MiBench or MiBench2 or have used them in the past, we encourage you to explore UlSWaP Bench.
If you do use UlSWaP-Bench or MiBench2 going forward, please use the "Cite this repository" button on the top right-hand corner of the Github page to cite our [paper from EWSN 2026](https://dl.acm.org/doi/10.3217/9h3a-2j82).
Alternatively, you can also copy the contents of the CITATION.bib file directly.


## Prerequisites
You will need [CMake](https://cmake.org/download/) as well as the cross-compiler for your target platform.

## Building
    cmake . -B <build_directory> -DCONFIG=<hw-specific-cmake.config> [-DPRINT=1] [-DCHECKSUM=1]
    cmake --build <build_directory> -j`nproc`


To build a subset of benchmarks:

    cmake --build <build_directory> -j`nproc` --target "benchmark1;benchmark2"

The executable ELF files will be in <build_directory>/elf, while the binary images (if generated)
will be in <build_directory>/bin.

Refer to the hardware-specific READMEs in hw/**/ to learn about additional pre-requisites, build
steps and other options for currently supported hardware (TODO).


## Memory Requirements
The following are the memory requirements (volatile and non-volatile) for each benchmark on RISC-V, ARM and MSP430:
Non-volatile memory is the sum of the `.text`, `.rodata` and `.data` sections, and volatile memory is the sum of the `.data` and `.bss` sections plus the peak measured stack usage.
The RISC-V numbers are for RV32I.
Note that your exact usage may vary based on your compiler, flags and startup/exit code.

### Non-volatile memory (bytes)

| Benchmark     | RISC-V  | ARM    | MSP430  |
|:--------------|--------:|-------:|--------:|
| aes           |   4,395 |  3,011 |   3,681 |
| chacha20      |   8,836 |  4,657 |  15,081 |
| poly1305      |  19,661 | 17,705 |  22,231 |
| rsa           |  23,509 | 11,635 |  22,267 |
| ecc           |   3,396 |  1,548 |   2,676 |
| sha256        |  18,721 | 17,805 |  20,460 |
| crc           |  16,817 | 16,727 |  17,293 |
| dijkstra      |   1,936 |  1,596 |   2,076 |
| patricia      |   7,012 |  5,116 |   7,270 |
| lzfx_comp     |   6,781 |  6,477 |   7,365 |
| lzfx_decomp   |   5,410 |  5,286 |   6,184 |
| lorawan_down  |  11,608 |  6,718 |   9,107 |
| lorawan_up    |  11,812 |  7,388 |   9,196 |
| fft           |  32,004 |  9,280 |  21,238 |
| adpcm_encode  |  49,709 | 36,917 |  45,328 |
| mp3_encode    | 102,836 | 75,422 | 102,492 |
| jpeg_encode   |  34,920 | 24,400 |  29,712 |
| susan_edges   |  17,592 |  8,512 |  12,094 |
| susan_corners |  18,983 |  9,415 |  14,699 |
| susan_smooth  |  24,456 | 13,752 |  19,670 |
| image_class   |  45,600 | 31,564 |  39,060 |
| anomaly       |  92,236 | 75,900 |  85,770 |
| activity_rec  |   3,276 |  1,072 |   2,218 |
| sensor_fusion |   9,284 |  5,238 |   6,680 |
| basicmath     |  34,956 | 13,388 |  30,306 |
| bitcount      |   1,524 |  1,102 |   2,042 |
| qsort         |  16,644 |  9,728 |  12,034 |
| stringsearch  |  11,187 | 10,854 |  11,254 |

### Volatile memory (bytes)

| Benchmark     | RISC-V | ARM    | MSP430 |
|:--------------|-------:|-------:|-------:|
| aes           |  1,424 |  1,428 |  1,397 |
| chacha20      |  3,312 |  3,312 |  3,350 |
| poly1305      |    560 |    400 |    528 |
| rsa           |  7,568 |  7,576 |  7,617 |
| ecc           |    784 |    696 |    726 |
| sha256        |    992 |    592 |    978 |
| crc           |  1,056 |  1,080 |  1,104 |
| dijkstra      |  6,468 |  6,372 |  5,358 |
| patricia      |  7,988 |  8,021 |  8,138 |
| lzfx_comp     |  7,112 |  7,120 |  7,154 |
| lzfx_decomp   |  6,208 |  6,196 |  6,238 |
| lorawan_down  |  3,278 |  2,494 |  2,469 |
| lorawan_up    |  3,080 |  2,352 |  2,324 |
| fft           |  4,980 |  4,492 |  4,714 |
| adpcm_encode  |  3,184 |  3,068 |  3,190 |
| mp3_encode    | 26,728 | 26,256 | 26,438 |
| jpeg_encode   |  7,484 |  7,252 |  7,260 |
| susan_edges   | 28,512 | 28,224 | 28,338 |
| susan_corners | 26,279 | 26,055 | 25,725 |
| susan_smooth  | 25,637 | 25,349 | 25,474 |
| image_class   | 12,788 | 12,421 | 12,502 |
| anomaly       |    848 |    352 |    546 |
| activity_rec  |    488 |    375 |    274 |
| sensor_fusion |    368 |     48 |    128 |
| basicmath     |    880 |    328 |    584 |
| bitcount      |    196 |     52 |    146 |
| qsort         |  7,984 |  7,912 |  6,670 |
| stringsearch  |  7,584 |  7,568 |  7,554 |


## Adding a new hardware platform
To support a new hardware platform, you will need a config.cmake file, linker script and startup/exit source code files.
The config.cmake file sets the C compiler, libraries, compiler/linker flags, linker script, post-build commands, etc. for the given hardware.
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

See tm4c123/config.cmake for an example that uses all of the above variables, and native/config.cmake that sets the minimum required variables.
All other hardware-specific files should be located in the same directory.

## Testing
To test your hardware platform, compile the benchmarks with printing enabled, and compare the output to the gold reference located at test/reference_output.
The benchmarks are designed to print meaningful output that can be used for testing purposes.
Alternatively, if the printf overhead is too high for your target platform, compile the benchmarks with checksum testing enabled, and compare your output checksums to the ones located in test/checksums.
The reference outputs and checksums have been captured on an x64 Linux platform.
The check_output.py script can be used for comparison, and the `-d` option can be used to set the maximum allowable difference for floating point numbers (default=0.0001).

## Analysis
There are a number of analysis scripts in the tools/analysis directory.
Run them with the `-h` argument to find out what they do and the arguments they expect.

## Directory layout
The repository is organised as follows:

    .
    ├── hw/                     # Hardware-specific source and build files
    ├── src/                    # Benchmark source code (one folder per benchmark)
    ├── test/
    |   ├── check_output.py
    |   ├── reference_output/   # Gold reference output for each benchmark
    ├── tools/
    |   ├── analysis/           # Scripts to analyse the benchmarks
    |   ├── input_gen/          # Scripts to generate input for various benchmarks
