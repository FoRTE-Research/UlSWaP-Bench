import json
from utils import *
from plot_utils import *

COUNT_LABEL = 'Instruction Count: '


def get_runtime_count(file_path:str) -> int:
    with open(file_path, 'r') as f:
        lines = f.readlines(-10)
        for line in reversed(lines):
            if COUNT_LABEL in line:
                count = int(line.split(COUNT_LABEL)[1].strip())
                return count

    raise ValueError(f'No line with {COUNT_LABEL} found in {file_path}')


def get_runtime_count_map(arch:str, arch_counts_dir:str) -> dict[str, int]:
    runtime_counts = {}
    bench_names = get_bench_names()

    for benchname in bench_names:
        file_path = os.path.join(arch_counts_dir, f'{benchname}')
        if not os.path.exists(file_path):
            print(f'No file found for {benchname}')
            runtime_counts[benchname] = 0
            continue
        count = get_runtime_count(file_path)
        runtime_counts[benchname] = count

    return runtime_counts


def get_runtime_count_maps(riscv_dir:str, msp430_dir:str, arm_dir:str) -> dict[str, dict[str, int]]:
    runtime_count_maps = {}
    for arch in ARCHITECTURES:
        if arch == 'riscv':
            runtime_count_maps[arch] = get_runtime_count_map(arch, riscv_dir)
        elif arch == 'msp430':
            runtime_count_maps[arch] = get_runtime_count_map(arch, msp430_dir)
        elif arch == 'arm':
            runtime_count_maps[arch] = get_runtime_count_map(arch, arm_dir)

    return runtime_count_maps


def plot_total_instruction_count(counts:dict[str, dict[str, int]], output_file:str|None=None) -> None:
    FONT_SIZE = 7.5
    FIGURE_HEIGHT = 3
    SEC_GAP_LINES = 7
    SEC_TICK_LENGTH = 73
    LEGEND_LABEL_SPACING = 0.35
    TICK_PADDING = 2
    BAR_LINE_WIDTH = 0.8
    HATCH_LINE_WIDTH = 0.5
    BAR_WIDTH = 0.27

    mpl.rcParams['hatch.linewidth'] = HATCH_LINE_WIDTH

    fig, ax = plt.subplots(constrained_layout=True)
    fig.set_figwidth(PAGEWIDTH)
    fig.set_figheight(FIGURE_HEIGHT)
    plt.yscale('log')

    x = np.arange(len(get_bench_names()))  # the label locations
    multiplier = 0

    for arch, count_map in counts.items():
        offset = BAR_WIDTH * multiplier
        rects = ax.bar(x + offset, list(count_map.values()), BAR_WIDTH, label=ARCH_LABELS[arch], color=ARCH_COLORS[multiplier], hatch=ARCH_HATCHES[multiplier], edgecolor='black', linewidth=BAR_LINE_WIDTH)
        multiplier += 1

    ax.set_ylabel(f'Total instructions executed', fontsize=FONT_SIZE, labelpad=TICK_PADDING)
    ax.set_xticks(x + BAR_WIDTH * (len(ARCHITECTURES) - 1) / 2, labels=get_bench_names())

    # Category labels
    sec = ax.secondary_xaxis(location=0)
    sec.set_xticks(get_label_xtick_positions(), labels=list('\n' * SEC_GAP_LINES + group for group in ALL_BENCHMARKS.keys()), weight='bold', size=FONT_SIZE)
    sec.tick_params('x', length=0)

    # Lines between the categories:
    sec2 = ax.secondary_xaxis(location=0)
    sec2.set_xticks(get_line_xticks(), labels=[])
    sec2.tick_params('x', length=SEC_TICK_LENGTH, width=1)
    ax.set_xlim(-0.5, len(get_bench_names()))

    plt.xticks(rotation=90, ha='center', fontsize=FONT_SIZE)
    plt.yticks(fontsize=FONT_SIZE)
    plt.legend(fontsize=FONT_SIZE, bbox_to_anchor=(0.23, 1), labelspacing=LEGEND_LABEL_SPACING)
    ax.grid(axis='y')

    # Reduce padding between tick and tick label
    ax.tick_params(axis='both', which='major', pad=TICK_PADDING)

    if output_file is not None:
        plt.savefig(output_file)
    else:
        plt.show()

    return


help_msg = '''
This script retrieves and plots the total instruction count for each benchmark for all 3 architectures
as a grouped bar chart.
The inputs are directories with filenames matching the names of the benchmarks, and each file containing one
line towards the end of the file with the total instruction count preceded by the label "Total instructions:".
All other lines in the files are ignored.
Inputs are provided using arch-specific arguments instead of -i/--input.
If no output file is specified, the plot will be displayed.
'''


def main():
    parent_parser = get_parent_parser(False, False)
    parser = argparse.ArgumentParser(parents=[parent_parser], description=help_msg)
    parser.add_argument('--riscv', required=True, help='Path to directory with RISC-V instruction count dumps')
    parser.add_argument('--msp430', required=True, help='Path to directory with MSP430 instruction count dumps')
    parser.add_argument('--arm', required=True, help='Path to directory with ARM instruction count dumps')
    parser.add_argument('--cache', help='Path to the JSON file with cached instruction counts')
    parser.add_argument('--refresh', action='store_true', help='Do not use the cached counts; refresh the existing cache.')
    args = parser.parse_args()
    cache_file = args.cache
    output_file = args.output
    refresh = args.refresh
    counts = None

    if refresh or cache_file is None:
        counts = get_runtime_count_maps(args.riscv, args.msp430, args.arm)
        if cache_file is not None:
            with open(cache_file, 'w') as f:
                json.dump(counts, f, indent=4)
    else:
        if cache_file is not None:
            with open(cache_file, 'r') as f:
                counts = json.load(f)

    assert(counts is not None)
    assert('riscv' in counts and 'msp430' in counts and 'arm' in counts)
    plot_total_instruction_count(counts, output_file)


if __name__ == '__main__':
    main()
