import csv
from utils import *
from static_memory import get_all_binary_sizes
from runtime_memory import get_runtime_memory_map


"""
Plots a stacked bar chart with all memory types for each benchmark. (Function no longer used)

Args:
    bench_total_mem (dict): A dictionary containing the sizes of binary files for each benchmark.
    output_file (str, optional): The path to the output file. Defaults to None.
"""
def plot_total_memory(bench_total_mem:dict[str, dict[str, int]], output_file:str=None):
    import matplotlib.pyplot as plt

    # Create a stacked bar chart of binary sizes for each benchmark
    fig, ax = plt.subplots()
    fig.set_size_inches(20, 10)
    bar_width = 0.5
    bench_names = get_bench_names()
    bar_bottom = [0] * len(bench_names)
    for mem_type in MEM_TYPES:
        sizes = [bench_total_mem[bench][mem_type] for bench in bench_names]
        ax.bar(bench_names, sizes, bar_width, label=mem_type, bottom=bar_bottom)
        bar_bottom = [bar_bottom[i] + sizes[i] for i in range(len(bench_names))]

    # ax.set_xlabel('Benchmarks')
    ax.set_ylabel('Size (bytes)')
    # ax.set_title('Binary Size')

    # ax.set_ylim(0, 110000)

    # Benchmark category labels
    sec = ax.secondary_xaxis(location=0)
    sec.set_xticks(get_label_xtick_positions(), labels=list('\n\n\n\n\n\n\n\n\n' + group for group in ALL_BENCHMARKS.keys()), weight='bold')

    sec.tick_params('x', length=0)

    # Lines between the categories:
    sec2 = ax.secondary_xaxis(location=0)
    sec2.set_xticks(get_line_xticks(), labels=[])
    sec2.tick_params('x', length=120, width=1.5)
    ax.set_xlim(-0.5, len(bench_names) - 0.5)

    # print total size on top of each bar
    for i, v in enumerate(bar_bottom):
        ax.text(i, v, str(v), ha='center', va='bottom')

    # rotate x-axis labels
    # plt.xticks(rotation=45, ha='right')
    plt.xticks(rotation=90, ha='center')
    ax.legend()
    ax.grid(axis='y')
    plt.tight_layout()

    if output_file:
        fig.savefig(output_file)
    else:
        plt.show()


def print_mem_table(bench_total_mem:dict[str, dict[str, int]]) -> None:
    print('Benchmark     |  .text  | .rodata |  .data  |  .bss   |  stack ')
    print('--------------|---------|---------|---------|---------|--------')
    for bench in get_bench_names():
        sizes = bench_total_mem[bench]
        print(f'{bench:<13} | {sizes[".text"]:>7} | {sizes[".rodata"]:>7} | {sizes[".data"]:>7} | {sizes[".bss"]:>7} | {sizes["stack"]:>7}')

    return


def print_mem_csv(bench_total_mem:dict[str, dict[str, int]], output_file:str) -> None:
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
    parent_parser = get_parent_parser(True, False)
    parser = argparse.ArgumentParser(parents=[parent_parser], description=help_msg, conflict_handler='resolve')
    parser.add_argument('-i', '--input', type=str, required=True, nargs=2, help='Binary directory followed by runtime memory dump directory')
    args = parser.parse_args()

    bin_dir = args.input[0]
    dump_dir = args.input[1]
    output_file = args.output

    if not check_dir_exists(bin_dir, False):
        print('Binary directory does not exist')
        return

    if not check_dir_exists(dump_dir, False):
        print('Runtime memory dump directory does not exist')
        return

    binary_size_dict = get_all_binary_sizes(bin_dir)
    runtime_memory_map = get_runtime_memory_map(dump_dir)

    total_memory_dict = binary_size_dict.copy()
    for bench, _ in binary_size_dict.items():
        total_memory_dict[bench]['stack'] = runtime_memory_map[bench]

    if output_file is None:
        print_mem_table(total_memory_dict)
    else:
        print_mem_csv(total_memory_dict, output_file)

    return


if __name__ == '__main__':
    main()
