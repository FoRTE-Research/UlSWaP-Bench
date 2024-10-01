import os
from enum import Enum

VSOC_EXEC = '/home/danchiba/FrankenRV/hdl/sim/obj_dir/VSOC'

ALL_BENCHMARKS = {
    'Security': ['aes', 'chacha20', 'poly1305', 'rsa', 'ecc', 'sha256'],
    'Communication': ['crc', 'dijkstra', 'patricia', 'lzfx_compress', 'lzfx_decompress', 'lorawan_down', 'lorawan_up'],
    'AI': ['activity_recognition', 'sensor_fusion', 'neural_net', 'anomaly'],
    'Signal Processing': ['fft', 'adpcm_encode', 'mp3_encode', 'jpeg_encode', 'susan_edges', 'susan_corners', 'susan_smoothing'],
    'General': ['basicmath', 'bitcount', 'qsort', 'stringsearch']
}

def get_bench_names() -> list[str]:
    bench_names = []
    for bench_group in ALL_BENCHMARKS.values():
        bench_names += bench_group

    return bench_names


def get_label_xtick_positions() -> list[float]:
    positions = []
    bench_idx = 0
    for i, bench_group in enumerate(ALL_BENCHMARKS.keys()):
        positions.append(bench_idx + (len(ALL_BENCHMARKS[bench_group]) - 1) / 2)
        bench_idx += len(ALL_BENCHMARKS[bench_group])

    return positions


def get_line_xticks() -> list[float]:
    positions = []
    bench_idx = 0
    for bench_group in ALL_BENCHMARKS.values():
        positions.append(bench_idx + len(bench_group) - 0.5)
        bench_idx += len(bench_group)

    return positions


class RunStatus(Enum):
    SUCCESS = 1
    FAILED = 2

def parse_args():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--benchmark', type=str, help='Benchmark to run')
    parser.add_argument('--hex_file', type=str, help='Path to the hex file')
    parser.add_argument('--bin_dir', type=str, help='Directory containing firmware binaries')
    parser.add_argument('--build_dir', type=str, help='Directory containing object files')
    parser.add_argument('--hex_dir', type=str, help='Directory containing firmware hex files')
    parser.add_argument('--dump_file', type=str, help='Dump file to analyze')
    parser.add_argument('--dump_dir', type=str, help='Directory containing verilator dump')
    parser.add_argument('--output_dir', type=str, help='Directory to store the output files')
    parser.add_argument('--plot_file', type=str, help='File in which to store the plot')

    return parser.parse_args()


def check_dir_exists(dir_path:str, create:bool) -> bool:
    if not os.path.exists(dir_path):
        if create:
            os.mkdir(dir_path)
        else:
            print(f'{dir_path} does not exist')
            return False
    return True
