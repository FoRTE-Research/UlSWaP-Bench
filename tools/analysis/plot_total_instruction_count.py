from utils import *

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

    return runtime_counts


def get_runtime_count(file_path:str) -> int:
    with open(file_path, 'r') as f:
        lines = f.readlines()

    final_count_line = lines[-2]
    count = int(final_count_line.split()[0])
    return count


def get_runtime_count_map(directory:str) -> dict:
    runtime_counts = {}
    bench_names = get_bench_names()

    for benchname in bench_names:
        file_path = os.path.join(directory, f'{benchname}')
        if not os.path.exists(file_path):
            print(f'No file found for {benchname}')
            runtime_counts[benchname] = 0
            continue
        count = get_runtime_count(file_path)
        runtime_counts[benchname] = count

    return runtime_counts


def plot_total_instruction_count(runtime_counts_unsorted:dict[str, int], output_file:str=None) -> None:
    import matplotlib.pyplot as plt

    XTICK_SIZE_MAIN = 8
    XTICK_SIZE_SEC = 8
    YTICK_SIZE = 8
    YAXIS_LABEL_SIZE = 8
    BAR_TEXT_SIZE = 5
    FACTOR = 1000

    runtime_counts = {}
    for bench in get_bench_names():
        runtime_counts[bench] = runtime_counts_unsorted[bench]

    # Create a stacked bar chart of binary sizes for each benchmark
    fig, ax = plt.subplots()
    fig.set_figwidth(6)

    for bench in runtime_counts.keys():
        runtime_counts[bench] = round(runtime_counts[bench] / FACTOR)

    ax.bar(runtime_counts.keys(), runtime_counts.values())
    ax.set_ylabel(f'Total instructions ({FACTOR}s)', fontsize=YAXIS_LABEL_SIZE)

    sec = ax.secondary_xaxis(location=0)
    sec.set_xticks(get_label_xtick_positions(), labels=list('\n\n\n\n\n\n\n\n' + group for group in ALL_BENCHMARKS.keys()), weight='bold', size=XTICK_SIZE_SEC)

    sec.tick_params('x', length=0)

    # Lines between the categories:
    sec2 = ax.secondary_xaxis(location=0)
    sec2.set_xticks(get_line_xticks(), labels=[])
    sec2.tick_params('x', length=65, width=1)
    ax.set_xlim(-0.5, len(runtime_counts) - 0.5)

    # print total size on top of each bar
    for bench, count in runtime_counts.items():
        # ax.text(bench, count, str(count), rotation=90, ha='center', va='bottom', fontsize=BAR_TEXT_SIZE)
        compact_count = get_compact_num(count)
        ax.text(bench, count, compact_count, ha='center', va='bottom', fontsize=BAR_TEXT_SIZE)

    plt.xticks(rotation=90, ha='center', fontsize=XTICK_SIZE_MAIN)
    plt.yticks(fontsize=YTICK_SIZE)
    ax.grid(axis='y')
    plt.tight_layout()

    if output_file is not None:
        plt.savefig(output_file)
    else:
        plt.show()

    return


def main():
    args = parse_args()
    if args.msp430:
        runtime_counts = get_runtime_count_map_msp430(args.dump_dir)
    elif args.arm:
        runtime_counts = get_runtime_count_map_arm(args.dump_file)
    else:
        runtime_counts = get_runtime_count_map(args.dump_dir)

    print(runtime_counts)
    plot_total_instruction_count(runtime_counts, args.plot_file)


if __name__ == '__main__':
    main()
