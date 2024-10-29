import csv
from utils import *

ARM_CSV_FILE = 'output/arm_totmem.csv'
RISCV_CSV_FILE = 'output/riscv_totmem.csv'
MSP430_CSV_FILE = 'output/msp430_totmem.csv'

def get_nvm_map(filename:str) -> dict[str, dict[str, int]]:
    nvm_map = {}

    with open(filename, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            nvm_map[row['Benchmark']] = {
                '.text': int(row['.text']),
                '.rodata': int(row['.rodata']),
                '.data': int(row['.data']),
            }

    return nvm_map


def plot_grouped_stacked_nvm(output_file:str=None) -> None:
    import matplotlib.pyplot as plt
    import matplotlib as mpl
    import numpy as np
    import matplotlib.patches as mpatches

    FONT_SIZE = 8
    XTICK_SIZE_MAIN = 8
    XTICK_SIZE_SEC = 8
    YTICK_SIZE = 8
    YAXIS_LABEL_SIZE = 8
    LEGEND_SIZE = 8

    BAR_LINE_WIDTH = 1
    HATCH_LINE_WIDTH = 0.5
    mpl.rcParams['hatch.linewidth'] = HATCH_LINE_WIDTH

    fig, ax = plt.subplots()
    fig.set_figwidth(6.5)
    fig.set_figheight(3.5)
    bar_width = 0.27

    bench_names = get_bench_names()
    positions = np.arange(len(bench_names))

    # Create the bars
    riscv_bar_bottoms = [0] * len(bench_names)
    msp_bar_bottoms = [0] * len(bench_names)
    arm_bar_bottoms = [0] * len(bench_names)
    for i, mem_type in enumerate(NVM_MEM_TYPES):
        riscv_sizes = [riscv_nvm[bench][mem_type] for bench in bench_names]
        msp_sizes = [msp_nvm[bench][mem_type] for bench in bench_names]
        arm_sizes = [arm_nvm[bench][mem_type] for bench in bench_names]

        riscv_bar = ax.bar(positions, riscv_sizes, bar_width, hatch=ARCH_HATCHES[0], color=MEM_COLORS[i], edgecolor='black', bottom=riscv_bar_bottoms, linewidth=BAR_LINE_WIDTH)
        msp430_bar = ax.bar(positions + bar_width, msp_sizes, bar_width, hatch=ARCH_HATCHES[1], color=MEM_COLORS[i], edgecolor='black', bottom=msp_bar_bottoms, linewidth=BAR_LINE_WIDTH)
        arm_bar = ax.bar(positions + 2 * bar_width, arm_sizes, bar_width, hatch=ARCH_HATCHES[2], color=MEM_COLORS[i], edgecolor='black', bottom=arm_bar_bottoms, linewidth=BAR_LINE_WIDTH)

        riscv_bar_bottoms = [riscv_bar_bottoms[i] + riscv_sizes[i] for i in range(len(bench_names))]
        msp_bar_bottoms = [msp_bar_bottoms[i] + msp_sizes[i] for i in range(len(bench_names))]
        arm_bar_bottoms = [arm_bar_bottoms[i] + arm_sizes[i] for i in range(len(bench_names))]

    # Legends
    arch_patches = [mpatches.Patch(facecolor='white', hatch=ARCH_HATCHES[i], edgecolor='black', label=ARCHITECTURES[i]) for i in range(len(ARCHITECTURES))]
    mem_patches = [mpatches.Patch(facecolor=MEM_COLORS[i], edgecolor='black', label=NVM_MEM_TYPES[i]) for i in range(len(NVM_MEM_TYPES))]

    arch_legend = ax.legend(handles=arch_patches, title='Architecture', loc='upper right', fontsize=LEGEND_SIZE, title_fontsize=LEGEND_SIZE)
    ax.add_artist(arch_legend)
    mem_legend = ax.legend(handles=mem_patches, title='Memory Type', loc='upper left', fontsize=LEGEND_SIZE, title_fontsize=LEGEND_SIZE)
    ax.add_artist(mem_legend)

    # Benchmark labels
    ax.set_xticks(positions + bar_width)
    ax.set_xticklabels(bench_names, rotation=90)
    plt.xticks(rotation=90, ha='center', fontsize=XTICK_SIZE_MAIN)

    # Benchmark category labels
    sec = ax.secondary_xaxis(location=0)
    sec.set_xticks(get_label_xtick_positions(), labels=list('\n\n\n\n\n\n\n\n' + group for group in ALL_BENCHMARKS.keys()), weight='bold', size=XTICK_SIZE_SEC)
    sec.tick_params('x', length=0)

    # Lines between the categories:
    sec2 = ax.secondary_xaxis(location=0)
    sec2.set_xticks(get_line_xticks(), labels=[])
    sec2.tick_params('x', length=82, width=1)

    # X-axis limits
    ax.set_xlim(-0.5, len(get_bench_names()))

    # Y-axis label
    ax.set_ylabel('Size (KiB)', fontsize=YAXIS_LABEL_SIZE, labelpad=0)
    plt.yticks(np.arange(0, 132000, 16384), fontsize=YTICK_SIZE, labels=[str(int(i/1024)) for i in range(0, 132000, 16384)])
    ax.grid(axis='y')

    # Tiny/small separating line
    ax.axhline(65536, color='black', linewidth=1, linestyle='--', label='64KB')
    ax.text(-0.3, 65536+1300, 'Small \u2191', fontsize=FONT_SIZE, va='bottom')
    ax.text(-0.3, 65536-2900, 'Tiny   \u2193', fontsize=FONT_SIZE, va='top')

    plt.tight_layout()

    if output_file:
        fig.savefig(output_file)
    else:
        plt.show()

    return


help_msg = '''
This script retrieves and plots the total non-volatile memory (NVM) usage for each benchmark for all 3 architectures
as a stacked and grouped bar chart.
The input files are CSV files with at least the following columns: Benchmark, .text, .rodata, .data.
The three filenames are currently hardcoded as global variables in the script.
If no output file is specified, the plot will be displayed.
'''


def main():
    parent_parser = get_parent_parser(False, False)
    parser = argparse.ArgumentParser(parents=[parent_parser], description=help_msg)
    args = parser.parse_args()
    output_file = args.output

    global riscv_nvm, msp_nvm, arm_nvm

    riscv_nvm = get_nvm_map(RISCV_CSV_FILE)
    msp_nvm = get_nvm_map(MSP430_CSV_FILE)
    arm_nvm = get_nvm_map(ARM_CSV_FILE)

    plot_grouped_stacked_nvm(output_file)

    return


if __name__ == '__main__':
    main()
