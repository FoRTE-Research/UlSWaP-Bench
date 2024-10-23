from utils import *

ARM_CSV_FILE = 'output/arm_counts.csv'
RISCV_DUMP_DIR = 'output/totcount'
MSP430_DUMP_DIR = 'output/msp430_counts2'

def get_runtime_count_msp430(file_path:str) -> int:
    with open(file_path, 'r') as f:
        lines = f.readlines()

    count = 0
    for line in lines[1:]:
        count += int(line.split(',')[1])

    return count


def get_runtime_count_map_msp430(directory:str) -> dict:
    runtime_counts = {}
    bench_names = get_bench_names()

    for benchname in bench_names:
        file_path = os.path.join(directory, f'{benchname}.csv')
        if not os.path.exists(file_path):
            print(f'No file found for {benchname}')
            runtime_counts[benchname] = 0
            continue
        count = get_runtime_count_msp430(file_path)
        runtime_counts[benchname] = count

    return runtime_counts


def get_runtime_count_map_arm(csv_file:str) -> dict:
    runtime_counts = {}
    with open(csv_file, 'r') as f:
        lines = f.readlines()

    for line in lines[1:]:
        parts = line.split(',')
        runtime_counts[parts[0]] = int(parts[1])

    # rearrange the counts in the order of the benchmarks
    bench_names = get_bench_names()
    sorted_runtime_counts = {}
    for bench in bench_names:
        sorted_runtime_counts[bench] = runtime_counts[bench]

    return sorted_runtime_counts


def get_runtime_count_riscv(file_path:str) -> int:
    with open(file_path, 'r') as f:
        lines = f.readlines()

    final_count_line = lines[-2]
    count = int(final_count_line.split()[0])
    return count


def get_runtime_count_map_riscv(directory:str) -> dict:
    runtime_counts = {}
    bench_names = get_bench_names()

    for benchname in bench_names:
        file_path = os.path.join(directory, f'{benchname}')
        if not os.path.exists(file_path):
            print(f'No file found for {benchname}')
            runtime_counts[benchname] = 0
            continue
        count = get_runtime_count_riscv(file_path)
        runtime_counts[benchname] = count

    return runtime_counts


def plot_total_instruction_count(output_file:str=None) -> None:
    import matplotlib as mpl
    import matplotlib.pyplot as plt
    import numpy as np

    FONT_SIZE = 8
    XTICK_SIZE_MAIN = FONT_SIZE
    XTICK_SIZE_SEC = FONT_SIZE
    YTICK_SIZE = FONT_SIZE
    YAXIS_LABEL_SIZE = FONT_SIZE
    BAR_TEXT_SIZE = 4
    FACTOR = 1000

    BAR_LINE_WIDTH = 1
    HATCH_LINE_WIDTH = 0.5
    mpl.rcParams['hatch.linewidth'] = HATCH_LINE_WIDTH

    archs = ['RISC-V', 'MSP430', 'ARM']
    bench_list = get_bench_names()

    runtime_counts = {}
    for arch in archs:
        if arch == 'RISC-V':
            runtime_counts[arch] = g_runtime_counts_riscv.values()
        elif arch == 'MSP430':
            runtime_counts[arch] = g_runtime_counts_msp.values()
        elif arch == 'ARM':
            runtime_counts[arch] = g_runtime_counts_arm.values()

    fig, ax = plt.subplots()
    fig.set_figwidth(6.5)
    fig.set_figheight(4)
    plt.yscale('log')

    x = np.arange(len(get_bench_names()))  # the label locations
    width = 0.27  # the width of the bars
    multiplier = 0

    for arch, counts in runtime_counts.items():
        offset = width * multiplier
        rects = ax.bar(x + offset, counts, width, label=arch, color=ARCH_COLORS[multiplier], hatch=ARCH_HATCHES[multiplier], edgecolor='black', linewidth=BAR_LINE_WIDTH)
        multiplier += 1

    ax.set_ylabel(f'Total instructions executed', fontsize=YAXIS_LABEL_SIZE, labelpad=1)
    ax.set_xticks(x + width * (len(archs) - 1) / 2, labels=get_bench_names())

    # Category labels
    sec = ax.secondary_xaxis(location=0)
    sec.set_xticks(get_label_xtick_positions(), labels=list('\n\n\n\n\n\n\n\n' + group for group in ALL_BENCHMARKS.keys()), weight='bold', size=XTICK_SIZE_SEC)
    sec.tick_params('x', length=0)

    # Lines between the categories:
    sec2 = ax.secondary_xaxis(location=0)
    sec2.set_xticks(get_line_xticks(), labels=[])
    sec2.tick_params('x', length=82, width=1)
    ax.set_xlim(-0.5, len(get_bench_names()))

    # print total size on top of each bar
    # for bench, count in runtime_counts.items():
    #     # ax.text(bench, count, str(count), rotation=90, ha='center', va='bottom', fontsize=BAR_TEXT_SIZE)
    #     compact_count = get_compact_num(count)
    #     ax.text(bench, count, compact_count, ha='center', va='bottom', fontsize=BAR_TEXT_SIZE)

    plt.xticks(rotation=90, ha='center', fontsize=XTICK_SIZE_MAIN)
    plt.yticks(fontsize=YTICK_SIZE)
    plt.legend(fontsize=FONT_SIZE, bbox_to_anchor=(0.23, 1))
    ax.grid(axis='y')
    plt.tight_layout()

    if output_file is not None:
        plt.savefig(output_file)
    else:
        plt.show()

    return


def main():
    args = parse_args()

    global g_runtime_counts_riscv
    global g_runtime_counts_msp
    global g_runtime_counts_arm

    g_runtime_counts_riscv = get_runtime_count_map_riscv(RISCV_DUMP_DIR)
    g_runtime_counts_msp = get_runtime_count_map_msp430(MSP430_DUMP_DIR)
    g_runtime_counts_arm = get_runtime_count_map_arm(ARM_CSV_FILE)

    plot_total_instruction_count(args.plot_file)


if __name__ == '__main__':
    main()
