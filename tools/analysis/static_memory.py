import subprocess as sp
import os

from utils import *

READELF_EXEC = 'riscv64-unknown-elf-readelf'

"""
Retrieves the output of the 'readelf -l' command for a given file.

Args:
    file_path (str): The full path to the file.

Returns:
    str: The output of the 'readelf -S' command.
"""
def get_readelf_section_output(file_path:str) -> str:
    cmd = f'{READELF_EXEC} -S {file_path}'
    return sp.check_output(cmd, shell=True).decode()


"""
Parses the section output and extracts the sizes of different sections.

Args:
    section_output (str): The output of the 'readelf -l' command.
    file_path (str): The full path to the file.

Returns:
    dict: A dictionary mapping section names to their sizes.
"""
def get_section_sizes(section_output:str) -> dict[str, int]:
    section_sizes = dict.fromkeys(MEM_TYPES, 0)
    section_lines = section_output.split('\n')

    for i, line in enumerate(section_lines[4:]):
        if '.text' in line:
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
def get_all_binary_sizes(directory:str) -> dict[str, dict[str, int]]:
    binary_sizes = {}
    for file in os.listdir(directory):
        if file.endswith('.elf'):
            bench_name = file.split('.')[0]
            file_path = os.path.join(directory, file)
            section_output = get_readelf_section_output(file_path)
            section_sizes = get_section_sizes(section_output)
            binary_sizes[bench_name] = section_sizes

    return binary_sizes

