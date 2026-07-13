import csv
import subprocess as sp
from utils import *

MEM_TYPES = ['.text', '.rodata', '.data', '.bss', 'stack']

READELF_EXECS = {
    'riscv': 'riscv64-unknown-elf-readelf',
    'msp430': '/opt/msp430-gcc/bin/msp430-elf-readelf',
    'arm': 'arm-none-eabi-readelf'
}


def get_runtime_memory(file_path:str) -> int:
    with open(file_path, 'r') as f:
        lines = f.readlines()

    if 'sp<=' not in lines[0]:
        raise ValueError(f"Cannot determine top of stack for {file_path}")
    stack_top = int(lines[0].split('<=')[1].strip(), 16)

    low_watermark = stack_top
    for line in lines:
        if 'sp<=' not in line:
            continue
        count = int(line.split('<=')[1].strip(), 16)
        if count < low_watermark:
            low_watermark = count

    return stack_top - low_watermark


def get_runtime_memory_map(directory:str) -> dict:
    runtime_memory = {}
    for file in os.listdir(directory):
        bench_name = file
        file_path = os.path.join(directory, file)
        count = get_runtime_memory(file_path)
        runtime_memory[bench_name] = count

    return runtime_memory


"""
Retrieves the output of the 'readelf -S' command for a given file.

Args:
    file_path (str): The full path to the file.

Returns:
    str: The output of the 'readelf -S' command.
"""
def get_readelf_sections_output(file_path:str, arch:str) -> str:
    cmd = f'{READELF_EXECS[arch]} -S {file_path}'
    return sp.check_output(cmd, shell=True).decode()


"""
Parses the section output and extracts the sizes of different sections.

Args:
    section_output (str): The output of the 'readelf -S' command.
    file_path (str): The full path to the file.

Returns:
    dict: A dictionary mapping section names to their sizes.
"""
def get_section_sizes(section_output:str) -> dict[str, int]:
    section_sizes = dict.fromkeys(MEM_TYPES, 0)
    section_lines = section_output.split('\n')

    for i, line in enumerate(section_lines[4:]):
        if '.text' in line or '.vectors' in line:
            section_sizes['.text'] += int(line.split()[-6], 16)
        elif '.rodata' in line:
            section_sizes['.rodata'] += int(line.split()[-6], 16)
        elif '.data' in line or '.sdata' in line:
            section_sizes['.data'] += int(line.split()[-6], 16)
        elif '.bss' in line or '.sbss' in line:
            section_sizes['.bss'] += int(line.split()[-6], 16)

    return section_sizes


"""
Retrieves the relevant section sizes of all binary files in a directory.

Args:
    directory (str): The path to the directory.

Returns:
    dict: A dictionary mapping benchmark names to a dictionary of their section sizes.
"""
def get_all_binary_sizes(directory:str, arch:str) -> dict[str, dict[str, int]]:
    binary_sizes = {}
    for file in os.listdir(directory):
        if file.endswith('.elf'):
            bench_name = file.split('.')[0]
            file_path = os.path.join(directory, file)
            sections_output = get_readelf_sections_output(file_path, arch)
            section_sizes = get_section_sizes(sections_output)
            binary_sizes[bench_name] = section_sizes

    return binary_sizes


def print_mem_table(bench_total_mem:dict[str, dict[str, int]]) -> None:
    print('Benchmark     |  .text  | .rodata |  .data  |  .bss   |  stack ')
    print('--------------|---------|---------|---------|---------|--------')
    for bench in get_bench_names():
        sizes = bench_total_mem[bench]
        print(f'{bench:<13} | {sizes[".text"]:>7} | {sizes[".rodata"]:>7} | {sizes[".data"]:>7} | {sizes[".bss"]:>7} | {sizes["stack"]:>7}')

    return


def write_mem_csv(bench_total_mem:dict[str, dict[str, int]], output_file:str) -> None:
    with open(output_file, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['Benchmark', '.text', '.rodata', '.data', '.bss', 'stack'])
        for bench in get_bench_names():
            sizes = bench_total_mem[bench]
            writer.writerow([bench, sizes['.text'], sizes['.rodata'], sizes['.data'], sizes['.bss'], sizes['stack']])

    return


help_msg = '''
This script prints the total memory usage (volatile and non-volatile) of each benchmark.
The input consists of two directories: the binary directory containing elf files and a dump directory containing the output
from verilator executions that print the stack pointer.
If no output file is specified, the memory usage will be printed to the console in a table format.
If an output file is specified, the memory usage will be written to it in CSV format.
'''


def main():
    parent_parser = get_parent_parser(False, False)
    parser = argparse.ArgumentParser(parents=[parent_parser], description=help_msg, conflict_handler='resolve')
    parser.add_argument('-i', '--input', type=str, required=True, nargs=2, help='Binary directory followed by runtime memory dump directory')
    parser.add_argument('--arch', type=str, required=True, choices=ARCHITECTURES, help='Target architecture')
    args = parser.parse_args()

    elf_dir = args.input[0]
    dump_dir = args.input[1]
    output_file = args.output
    arch = args.arch

    for tool in READELF_EXECS.values():
        if not (check_tool_exists(tool, False)):
            print('Please edit the READELF_EXECS dictionary in current script to set the correct path for the tool.')
            return

    if not check_dir_exists(elf_dir, False):
        print('Binary directory does not exist')
        return

    if not check_dir_exists(dump_dir, False):
        print('Runtime memory dump directory does not exist')
        return

    binary_size_dict = get_all_binary_sizes(elf_dir, arch)
    runtime_memory_map = get_runtime_memory_map(dump_dir)

    total_memory_dict = binary_size_dict.copy()
    for bench, _ in binary_size_dict.items():
        total_memory_dict[bench]['stack'] = runtime_memory_map[bench]

    if output_file is None:
        print_mem_table(total_memory_dict)
    else:
        write_mem_csv(total_memory_dict, output_file)

    return


if __name__ == '__main__':
    main()
