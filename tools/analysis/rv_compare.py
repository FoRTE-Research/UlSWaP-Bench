import csv

from utils import *
from plot_utils import *
from plot_total_instruction_count import get_runtime_count

# Four files and four subdirectories are expected in the top-level dump directory: one CSV
# file per sub-architecture and one subdirectory per sub-architecture for instruction counts.
# The format is as follows, with `{0}` being the top-level dump directory and `{1}` being the
# sub-architecture in lower case (e.g., 'rv32i', 'rv32e', etc.).
MEMFILE_FMT = '{0}/{1}_totmem.csv'
COUNTDIR_FMT = '{0}/riscv/{1}'

ALT_SUB_ARCHS = ['RV32E', 'RV32IM', 'RV32IMF']
BASE_SUB_ARCH = 'RV32I'


def get_stack_size(benchmark:str, filename:str) -> int:
    # Get the stack size from the CSV file
    with open(filename, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            if row['Benchmark'] == benchmark:
                stack_size = int(row['stack'])
                break
        else:
            raise ValueError(f'Benchmark {benchmark} not found in {filename}')

    return stack_size


def get_text_size(benchmark:str, filename:str) -> int:
    # Get the text size from the CSV file
    with open(filename, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            if row['Benchmark'] == benchmark:
                text_size = int(row['.text'])
                break
        else:
            raise ValueError(f'Benchmark {benchmark} not found in {filename}')

    return text_size


def get_instruction_count(benchmark:str, directory:str) -> int:
    file_path = os.path.join(directory, benchmark)
    count = get_runtime_count(file_path)

    return count


def draw_group_gridlines(ax, benchmarks: list[str], group_center_offset: float, linewidth: float) -> None:
    # Vertical gridlines between benchmark groups (rather than through the tick label)
    color = mpl.rcParams['grid.color']
    alpha = mpl.rcParams['grid.alpha']
    for i in range(len(benchmarks) - 1):
        ax.axvline(i + 0.5 + group_center_offset, linestyle='--', linewidth=linewidth, color=color, alpha=alpha)


def plot_instruction_count_diffs(top_dump_dir: str, output_file: str | None = None) -> None:
    FONT_SIZE = 7.5
    FIGURE_HEIGHT = 3
    SEC_GAP_LINES = 7
    SEC_TICK_LENGTH = 73
    LEGEND_LABEL_SPACING = 0.35
    TICK_PADDING = 2
    BAR_LINE_WIDTH = 0.8
    HATCH_LINE_WIDTH = 0.5
    BAR_WIDTH = 0.27
    GRID_LINE_WIDTH = 0.25

    mpl.rcParams['hatch.linewidth'] = HATCH_LINE_WIDTH

    fig, ax = plt.subplots(constrained_layout=True)
    fig.set_figwidth(PAGEWIDTH)
    fig.set_figheight(FIGURE_HEIGHT)

    benchmarks = get_bench_names()
    baselines    = [get_instruction_count(b, COUNTDIR_FMT.format(top_dump_dir, 'rv32i'))   for b in benchmarks]
    rv32e_counts = [get_instruction_count(b, COUNTDIR_FMT.format(top_dump_dir, 'rv32e'))   for b in benchmarks]
    rv32im_counts  = [get_instruction_count(b, COUNTDIR_FMT.format(top_dump_dir, 'rv32im'))  for b in benchmarks]
    rv32imf_counts = [get_instruction_count(b, COUNTDIR_FMT.format(top_dump_dir, 'rv32imf')) for b in benchmarks]

    rv32e_diffs   = [(rv32e_counts[i]   - baselines[i]) / baselines[i] * 100 for i in range(len(benchmarks))]
    rv32im_diffs  = [(rv32im_counts[i]  - baselines[i]) / baselines[i] * 100 for i in range(len(benchmarks))]
    rv32imf_diffs = [(rv32imf_counts[i] - baselines[i]) / baselines[i] * 100 for i in range(len(benchmarks))]

    diffs = [rv32e_diffs, rv32im_diffs, rv32imf_diffs]

    x = np.arange(len(benchmarks))
    multiplier = 0
    for i, (arch, diff) in enumerate(zip(ALT_SUB_ARCHS, diffs)):
        offset = BAR_WIDTH * multiplier
        ax.bar(
            x + offset, diff, BAR_WIDTH,
            label=arch,
            color=ARCH_COLORS[i],
            hatch=ARCH_HATCHES[i],
            edgecolor='black',
            linewidth=BAR_LINE_WIDTH,
        )
        multiplier += 1

    # Baseline reference line
    ax.axhline(0, color='black', linewidth=0.8, linestyle='--')

    ax.set_ylabel('Instruction count change (%)', fontsize=FONT_SIZE, labelpad=TICK_PADDING)

    ax.set_xticks(x + BAR_WIDTH * (len(ALT_SUB_ARCHS) - 1) / 2, labels=benchmarks)

    # Category labels
    sec = ax.secondary_xaxis(location=0)
    sec.set_xticks(
        get_label_xtick_positions(),
        labels=list('\n' * SEC_GAP_LINES + group for group in ALL_BENCHMARKS.keys()),
        weight='bold',
        size=FONT_SIZE,
    )
    sec.tick_params('x', length=0)

    # Lines between categories
    sec2 = ax.secondary_xaxis(location=0)
    sec2.set_xticks(get_line_xticks(), labels=[])
    sec2.tick_params('x', length=SEC_TICK_LENGTH, width=1)
    ax.set_xlim(-0.5, len(benchmarks))
    ax.set_ylim(-100, 35)

    # Crosses for benchmarks that don't use the relevant extension
    for i, bench in enumerate(benchmarks):
        usage = MULDIV_USAGE[bench]
        bar_center = x[i] + BAR_WIDTH * (len(ALT_SUB_ARCHS) - 1) / 2

        # RV32IM: cross if no M usage
        if usage == 'none':
            ax.plot(x[i] + BAR_WIDTH * 1, 0, marker='x', color='black', markersize=4,
                    markeredgewidth=0.8, clip_on=False, zorder=5)
        # RV32IMF: cross if no F usage (i.e. not 'float')
        if usage != 'float':
            ax.plot(x[i] + BAR_WIDTH * 2, 0, marker='x', color='black', markersize=4,
                    markeredgewidth=0.8, clip_on=False, zorder=5)

    plt.xticks(rotation=90, ha='center', fontsize=FONT_SIZE)
    plt.yticks(fontsize=FONT_SIZE)
    plt.legend(fontsize=FONT_SIZE, loc='upper right', ncols=3, labelspacing=LEGEND_LABEL_SPACING)
    ax.grid(axis='y', linestyle='--', linewidth=GRID_LINE_WIDTH)
    draw_group_gridlines(ax, benchmarks, BAR_WIDTH * (len(ALT_SUB_ARCHS) - 1) / 2, GRID_LINE_WIDTH)

    # Reduce padding between tick and tick label
    ax.tick_params(axis='both', which='major', pad=TICK_PADDING)

    if output_file is not None:
        plt.savefig(output_file)
    else:
        plt.show()


def plot_memory_diffs(top_dump_dir: str, output_file: str | None = None) -> None:
    FONT_SIZE = 7.5
    FIGURE_HEIGHT = 5
    SEC_GAP_LINES = 7
    SEC_TICK_LENGTH = 73
    LEGEND_LABEL_SPACING = 0.35
    TICK_PADDING = 2
    BAR_LINE_WIDTH = 0.8
    HATCH_LINE_WIDTH = 0.5
    BAR_WIDTH = 0.27
    GRID_LINE_WIDTH = 0.25

    mpl.rcParams['hatch.linewidth'] = HATCH_LINE_WIDTH

    fig, (ax_text, ax_stack) = plt.subplots(2, 1, sharex=True, constrained_layout=True)
    fig.set_figwidth(PAGEWIDTH)
    fig.set_figheight(FIGURE_HEIGHT)

    benchmarks = get_bench_names()

    def compute_diffs(get_fn, base_dir, dirs):
        baselines = [get_fn(b, base_dir) for b in benchmarks]
        return [
            [(get_fn(b, d) - baselines[i]) / baselines[i] * 100 for i, b in enumerate(benchmarks)]
            for d in dirs
        ]

    dirs = [MEMFILE_FMT.format(top_dump_dir, 'rv32e'), MEMFILE_FMT.format(top_dump_dir, 'rv32im'), MEMFILE_FMT.format(top_dump_dir, 'rv32imf')]
    text_diffs  = compute_diffs(get_text_size,  MEMFILE_FMT.format(top_dump_dir, 'rv32i'), dirs)
    stack_diffs = compute_diffs(get_stack_size, MEMFILE_FMT.format(top_dump_dir, 'rv32i'), dirs)

    x = np.arange(len(benchmarks))

    def draw_bars(ax, diffs):
        for i, (arch, diff) in enumerate(zip(ALT_SUB_ARCHS, diffs)):
            ax.bar(
                x + BAR_WIDTH * i, diff, BAR_WIDTH,
                label=arch,
                color=ARCH_COLORS[i],
                hatch=ARCH_HATCHES[i],
                edgecolor='black',
                linewidth=BAR_LINE_WIDTH,
            )
        ax.axhline(0, color='black', linewidth=0.8, linestyle='--')
        ax.grid(axis='y', linestyle='--', linewidth=GRID_LINE_WIDTH)
        draw_group_gridlines(ax, benchmarks, BAR_WIDTH * (len(ALT_SUB_ARCHS) - 1) / 2, GRID_LINE_WIDTH)

    def draw_crosses(ax,):
        for i, bench in enumerate(benchmarks):
            usage = MULDIV_USAGE[bench]
            if usage == 'none':
                ax.plot(x[i] + BAR_WIDTH * 1, 0, marker='x', color='black', markersize=4,
                        markeredgewidth=0.8, clip_on=False, zorder=5)
            if usage != 'float':
                ax.plot(x[i] + BAR_WIDTH * 2, 0, marker='x', color='black', markersize=4,
                        markeredgewidth=0.8, clip_on=False, zorder=5)

    draw_bars(ax_text, text_diffs)
    draw_crosses(ax_text)
    ax_text.legend(fontsize=FONT_SIZE, loc='upper right', ncols=3, bbox_to_anchor=(1, 1), labelspacing=LEGEND_LABEL_SPACING)
    ax_text.set_ylabel('Text size change (%)', fontsize=FONT_SIZE, labelpad=TICK_PADDING)
    ax_text.tick_params(axis='y', labelsize=FONT_SIZE)
    ax_text.tick_params(axis='x', which='both', length=0)
    ax_text.set_ylim(-90, 40)

    draw_bars(ax_stack, stack_diffs)
    draw_crosses(ax_stack)
    ax_stack.set_ylabel('Stack size change (%)', fontsize=FONT_SIZE, labelpad=TICK_PADDING)
    ax_stack.tick_params(axis='y', labelsize=FONT_SIZE)
    ax_stack.tick_params(axis='x', which='major', pad=TICK_PADDING)
    ax_stack.set_ylim(-95, 0)

    # X-axis labels on bottom subplot only
    ax_stack.set_xticks(x + BAR_WIDTH * (len(ALT_SUB_ARCHS) - 1) / 2, labels=benchmarks)
    ax_stack.tick_params(axis='x', labelrotation=90, labelsize=FONT_SIZE)

    # Category labels
    sec = ax_stack.secondary_xaxis(location=0)
    sec.set_xticks(
        get_label_xtick_positions(),
        labels=list('\n' * SEC_GAP_LINES + group for group in ALL_BENCHMARKS.keys()),
        weight='bold',
        size=FONT_SIZE,
    )
    sec.tick_params('x', length=0)

    # Lines between categories
    sec2 = ax_stack.secondary_xaxis(location=0)
    sec2.set_xticks(get_line_xticks(), labels=[])
    sec2.tick_params('x', length=SEC_TICK_LENGTH, width=1)

    ax_text.set_xlim(-0.5, len(benchmarks))

    if output_file is not None:
        plt.savefig(output_file)
    else:
        plt.show()


help_msg = '''
Plot the differences in memory usage (stack and text) or instruction count for various RV32I*
sub-architectures compared to a baseline RV32I implementation.
The input is the top level directory containing the memory usage CSV files and instruction
count directories for the various sub-architectures. Refer to the top of this file for the
expected directory structure and file naming conventions.
If no output file is specified, the plot will be displayed.
'''

def main():
    parent_parser = get_parent_parser(True, False)
    parser = argparse.ArgumentParser(parents=[parent_parser], description=help_msg)
    parser.add_argument('--inst', action='store_true', help='Plot instruction count differences')
    parser.add_argument('--mem', action='store_true', help='Plot memory differences')
    args = parser.parse_args()
    output_file = args.output
    top_dump_dir = args.input

    if args.inst:
        plot_instruction_count_diffs(top_dump_dir, output_file)
    elif args.mem:
        plot_memory_diffs(top_dump_dir, output_file)
    else:
        print('Please specify either --inst or --mem to plot differences.')
    return


if __name__ == '__main__':
    main()
