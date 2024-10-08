import subprocess as sp
import os

from utils import *

READELF_EXEC = 'riscv64-unknown-elf-readelf'

"""
Retrieves the output of the 'readelf -l' command for a given file.

Args:
    file_path (str): The full path to the file.

Returns:
    str: The output of the 'readelf -l' command.
"""
def get_readelf_segment_output(file_path:str) -> str:
    cmd = f'{READELF_EXEC} -l {file_path}'
    return sp.check_output(cmd, shell=True).decode()


"""
Parses the segment output and extracts the sizes of different segments.

Args:
    segment_output (str): The output of the 'readelf -l' command.
    file_path (str): The full path to the file.

Returns:
    dict: A dictionary mapping section names to their sizes.
"""
def get_segment_sizes(segment_output:str, file_path:str) -> dict[str, int]:
    segment_sizes = {}
    seg_sections = segment_output.split('Segment Sections...')[1].splitlines()
    prog_headers = segment_output.split('Program Headers:')[1].split('Section to Segment mapping:')[0].splitlines()

    for i, line in enumerate(prog_headers[1:]):
        if 'LOAD' in line:
            segment_line = seg_sections[i]
            if '.text' in segment_line:
                segment = '.text'
            elif '.rodata' in segment_line:
                segment = '.rodata'
            elif '.data' in segment_line or '.sdata' in segment_line:
                segment = '.data'
            elif '.sbss' in segment_line or '.bss' in segment_line:
                segment = '.bss'
            else:
                segment = 'unknown'
                print(f'Unknown segment found: {segment_line} in {file_path}')
            size = int(line.split()[5], 16)
            segment_sizes[segment] = size

    if len(segment_sizes) < 1:
        print('Segment sizes not found for ' + file_path)
    elif len(segment_sizes) > 3:
        print('More than 3 segments found for ' + file_path)

    return segment_sizes


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
            segment_output = get_readelf_segment_output(file_path)
            segment_sizes = get_segment_sizes(segment_output, file_path)
            binary_sizes[bench_name] = segment_sizes

    return binary_sizes

