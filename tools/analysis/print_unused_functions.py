import os
import subprocess as sp

from utils import *

READELF = 'riscv64-unknown-elf-readelf'
CMAKE_FILES_PATH = ''

def get_functions_from_elf_file(elf_file:str) -> list[str]:
    cmd = f'{READELF} -s {elf_file}'
    output = sp.check_output(cmd, shell=True).decode()
    lines = output.split('\n')
    functions = []
    for line in lines:
        if 'FUNC' in line:
            function = line.split()[-1]
            functions.append(function)

    return functions


def get_unused_functions(benchmark:str, build_dir:str) -> list[str]:
    unused_funcs = []

    obj_funcs = []
    benchmark_obj_dir = f'{build_dir}/CMakeFiles/{benchmark}.dir/src/{benchmark}'
    for file in os.listdir(benchmark_obj_dir):
        if file.endswith('.c.o'):
            elf_file = f'{benchmark_obj_dir}/{file}'
            obj_funcs += get_functions_from_elf_file(elf_file)

    benchmark_elf_file = f'{build_dir}/bin/{benchmark}.elf'
    elf_funcs = get_functions_from_elf_file(benchmark_elf_file)

    for func in obj_funcs:
        if func not in elf_funcs:
            unused_funcs.append(func)

    return unused_funcs


def get_all_unused_functions(build_dir:str) -> dict[str, list[str]]:
    unused_funcs = {}
    for entry in os.listdir(f'{build_dir}/bin'):
        if entry.endswith('.elf'):
            benchmark = entry.split('.')[0]
            unused_funcs[benchmark] = get_unused_functions(benchmark, build_dir)

    return unused_funcs


def print_unused_functions(unused_funcs:dict[str, list[str]]):
    for benchmark, funcs in unused_funcs.items():
        print(f'\033[1m{benchmark}:\033[0m')
        for func in funcs:
            print(func)

        print()

    return


help_msg = '''
This script reads the ELF files for each benchmark and finds functions that are compiled but not used in the final binary.
The output is printed to the console.
'''


def main():
    parent_parser = get_parent_parser(True, False)
    parser = argparse.ArgumentParser(parents=[parent_parser], description=help_msg)
    args = parser.parse_args()

    build_dir = args.input
    check_dir_exists(build_dir, create=False)

    unused_funcs = get_all_unused_functions(build_dir)
    print_unused_functions(unused_funcs)


if __name__ == '__main__':
    main()
