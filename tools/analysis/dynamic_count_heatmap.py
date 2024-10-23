import heatmap
import matplotlib.pyplot as plt
import numpy as np

from utils import *

def get_instruction_count_maps(dir_name:str) -> dict[str, dict[str, int]]:
    benchmark_maps = {}

    for filename in os.listdir(dir_name):
        filepath = os.path.join(dir_name, filename)
        instr_count_map = {}
        benchname = filename.split('.')[0]
        with open(filepath, 'r') as fp:
            csv = fp.read()

        for line in csv.splitlines():
            if 'execs' in line:
                continue
            instr, count = line.split(',')
            instr = instr.replace('MSP430_OP_', '')
            instr_count_map[instr] = int(count)

        benchmark_maps[benchname] = instr_count_map

    benchmark_maps = dict(sorted(benchmark_maps.items(), key=lambda item: item[0]))

    return benchmark_maps


def get_hw_count_map(instr_count_maps:dict[str, dict[str, int]]) -> dict[str, dict[str, int]]:
    hw_count_map = {}
    for benchmark, instr_count_map in instr_count_maps.items():
        hw_count_map[benchmark] = instr_count_map.copy()
        for instr, count in instr_count_map.items():
            if instr.endswith('I') and instr != 'LUI':
                hw_instr = instr[:-1]
                hw_count_map[benchmark][hw_instr] = hw_count_map[benchmark].get(hw_instr, 0) + count
                hw_count_map[benchmark].pop(instr)

    return hw_count_map


def get_normalized_instruction_count_maps(instr_count_maps:dict[str, dict[str, int]]) -> dict[str, dict[str, float]]:
    normalized_maps = {}
    for benchmark, instr_count_map in instr_count_maps.items():
        normalized_map = {k: v / sum(instr_count_map.values()) for k, v in instr_count_map.items()}
        normalized_maps[benchmark] = normalized_map

    return normalized_maps


def show_heatmap(instr_count_maps:dict[str, dict[str, float]], image_name:str|None) -> None:
    XTICK_SIZE_MAIN = 6
    YTICK_SIZE = 6
    YAXIS_LABEL_SIZE = 6

    benchmark_names = instr_count_maps.keys()
    instr_names = set()
    for instr_count_map in instr_count_maps.values():
        instr_names.update(instr_count_map.keys())
    instr_names = list(instr_names)
    instr_names.sort()

    heatmap_data = np.zeros((len(benchmark_names), len(instr_names)))
    for i, benchmark_name in enumerate(benchmark_names):
        instr_count_map = instr_count_maps[benchmark_name]
        for j, instr_name in enumerate(instr_names):
            heatmap_data[i, j] = instr_count_map.get(instr_name, 0)

    fig, ax = plt.subplots()

    if 'msp430' in image_name:
        fig.set_figwidth(7.5)
        fig.set_figheight(3.6)
    else:
        fig.set_figwidth(6.5)
        fig.set_figheight(3.6)

    im, cbar = heatmap.heatmap(heatmap_data, benchmark_names, instr_names, ax, cbarlabel='Relative Instruction Freqruency', cmap='gray_r')
    heatmap.annotate_heatmap(im, data=heatmap_data, valfmt='{x:.0f}', fontsize=5)

    fig.tight_layout()

    if image_name is not None:
        plt.savefig(image_name)
    else:
        plt.show()

    return


def main():
    args = parse_args()
    dump_dir = args.dump_dir
    instr_count_maps = get_instruction_count_maps(dump_dir)
    hw_count_map = get_hw_count_map(instr_count_maps)
    normalized_maps = get_normalized_instruction_count_maps(hw_count_map)
    # normalized_maps = get_normalized_instruction_count_maps(instr_count_maps)
    show_heatmap(normalized_maps, args.plot_file)
    return


if __name__ == '__main__':
    main()

# Usage:
# python dynamic_count_heatmap.py --dump_dir=directory/with/csv/files [--plot_file=heatmap.pdf]
