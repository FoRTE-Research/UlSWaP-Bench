from utils import *
from static_memory import get_all_binary_sizes
from runtime_memory import get_runtime_memory_map


"""
Plots a stacked bar chart of section sizes for each benchmark.

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


def main():
    args = parse_args()
    if not check_dir_exists(args.bin_dir, False):
        print('Binary directory does not exist')
        return

    binary_size_dict = get_all_binary_sizes(args.bin_dir)
    runtime_memory_map = get_runtime_memory_map(args.dump_dir)

    total_memory_dict = binary_size_dict.copy()
    for bench, _ in binary_size_dict.items():
        total_memory_dict[bench]['stack'] = runtime_memory_map[bench]

    for bench in get_bench_names():
        sizes = total_memory_dict[bench]
        nvm_size = sizes['.text'] + sizes['.rodata']
        ram_size = sizes['.data'] + sizes['.bss'] + sizes['stack']
        print('%20s: text = %5d, rodata = %5d, data = %5d, bss = %5d, stack = %5d, nvm = %6d, ram = %5d' %
              (bench, sizes['.text'], sizes['.rodata'], sizes['.data'], sizes['.bss'], sizes['stack'], nvm_size, ram_size))
    plot_total_memory(total_memory_dict, args.plot_file)


if __name__ == '__main__':
    main()
