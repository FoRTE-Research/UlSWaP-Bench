from utils import *


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


def plot_total_instruction_count(runtime_counts:dict[str, int], output_file:str=None) -> None:
    import matplotlib.pyplot as plt

    # Create a stacked bar chart of binary sizes for each benchmark
    fig, ax = plt.subplots()
    fig.set_figwidth(7)
    # bar_width = 0.5

    ax.bar(runtime_counts.keys(), runtime_counts.values())
    ax.set_ylabel('Total instructions')

    sec = ax.secondary_xaxis(location=0)
    sec.set_xticks(get_label_xtick_positions(), labels=list('\n\n\n\n\n\n\n\n\n' + group for group in ALL_BENCHMARKS.keys()), weight='bold')

    sec.tick_params('x', length=0)

    # Lines between the categories:
    sec2 = ax.secondary_xaxis(location=0)
    sec2.set_xticks(get_line_xticks(), labels=[])
    sec2.tick_params('x', length=120, width=1.5)
    ax.set_xlim(-0.5, len(runtime_counts) - 0.5)

    # print total size on top of each bar
    for bench, count in runtime_counts.items():
        ax.text(bench, count, str(count), ha='center', va='bottom')

    plt.xticks(rotation=90, ha='center')
    ax.grid(axis='y')
    plt.tight_layout()

    if output_file is not None:
        plt.savefig(output_file)
    else:
        plt.show()

    return


def main():
    args = parse_args()
    runtime_counts = get_runtime_count_map(args.dump_dir)
    print(runtime_counts)
    plot_total_instruction_count(runtime_counts, args.plot_file)


if __name__ == '__main__':
    main()
