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
            elif '.data' in segment_line:
                segment = '.data'
            elif 'bss' in segment_line:
                continue
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


"""
Plots a stacked bar chart of section sizes for each benchmark.

Args:
    binary_sizes (dict): A dictionary containing the sizes of binary files for each benchmark.
    output_file (str, optional): The path to the output file. Defaults to None.
"""
def plot_binary_sizes(binary_sizes:dict[str, dict[str, int]], output_file:str=None):
    import matplotlib.pyplot as plt

    # Sort the dictionary alphabetically
    sorted_binary_sizes = dict(sorted(binary_sizes.items()))

    # Create a stacked bar chart of binary sizes for each benchmark
    fig, ax = plt.subplots()
    fig.set_size_inches(20, 10)
    bar_width = 0.5
    x = list(sorted_binary_sizes.keys())
    bar_bottom = [0] * len(x)
    for segment in ['text', 'rodata', 'data']:
        sizes = [sorted_binary_sizes[bench].get(f'.{segment}', 0) for bench in x]
        print(sizes)
        ax.bar(x, sizes, bar_width, label=segment, bottom=bar_bottom)
        bar_bottom = [bar_bottom[i] + sizes[i] for i in range(len(x))]

    ax.set_xlabel('Benchmarks')
    ax.set_ylabel('Size (bytes)')
    # ax.set_title('Binary Size')

    # ax.set_ylim(0, 110000)

    # print total size on top of each bar
    # for i, v in enumerate(bar_bottom):
    #     ax.text(i, v, str(v), ha='center', va='bottom')

    # rotate x-axis labels
    plt.xticks(rotation=45, ha='right')
    ax.legend()
    ax.grid(axis='y')
    plt.tight_layout()

    if output_file:
        fig.savefig(output_file)
    else:
        plt.show()


def main():
    args = parse_args()
    if not check_dir_exists(args.bin_dir, False):
        print('Binary directory does not exist')
        return

    binary_size_dict = get_all_binary_sizes(args.bin_dir)
    plot_binary_sizes(binary_size_dict, args.plot_file)


if __name__ == '__main__':
    main()
