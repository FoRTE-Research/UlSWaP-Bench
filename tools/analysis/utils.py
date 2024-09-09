import os
from enum import Enum

VSOC_EXEC = '/home/danchiba/FrankenRV/hdl/sim/obj_dir/VSOC'

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
