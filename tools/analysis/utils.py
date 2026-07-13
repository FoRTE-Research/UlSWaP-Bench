import argparse
import os
from enum import Enum
import shutil
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BENCH_TOP_DIR = os.path.join(SCRIPT_DIR, '../..')

ALL_BENCHMARKS = {
    'Security': ['aes', 'chacha20', 'poly1305', 'rsa', 'ecc', 'sha256'],
    'Communication': ['crc', 'dijkstra', 'patricia', 'lzfx_comp', 'lzfx_decomp', 'lorawan_down', 'lorawan_up'],
    'Signal Processing': ['fft', 'adpcm_encode', 'mp3_encode', 'jpeg_encode', 'susan_edges', 'susan_corners', 'susan_smooth'],
    'AI': ['image_class', 'anomaly', 'activity_rec', 'sensor_fusion'],
    'General': ['basicmath', 'bitcount', 'qsort', 'stringsearch']
}

ARCHITECTURES = ['riscv', 'msp430', 'arm']


def get_bench_names() -> list[str]:
    bench_names = []
    for bench_group in ALL_BENCHMARKS.values():
        bench_names += bench_group

    return bench_names


MULDIV_USAGE = {
    'fft': 'double',
    'mp3_encode': 'double',
    'anomaly': 'double',
    'basicmath': 'double',

    'jpeg_encode': 'float',
    'susan_edges': 'float',
    'susan_corners': 'float',
    'susan_smooth': 'float',
    'image_class': 'float',
    'sensor_fusion': 'float',
    'qsort': 'float',

    'poly1305': 'int',
    'rsa': 'int',
    'patricia': 'int',
    'lorawan_down': 'int',
    'lorawan_up': 'int',
    'adpcm_encode': 'int',
    'activity_rec': 'int',

    'aes': 'none',
    'chacha20': 'none',
    'ecc': 'none',
    'sha256': 'none',
    'crc': 'none',
    'dijkstra': 'none',
    'lzfx_comp': 'none',
    'lzfx_decomp': 'none',
    'bitcount': 'none',
    'stringsearch': 'none',
}


def color(text:str, color:str, emph:str|None=None) -> str:
    match color:
        case 'red': color_num = 31
        case 'green': color_num = 32
        case 'yellow': color_num = 33
        case 'blue': color_num = 34
        case 'magenta': color_num = 35
        case 'cyan': color_num = 36
        case 'white': color_num = 37
        case _: return text

    if emph is not None:
        match emph:
            case 'b': emph_num = 1
            case 'd': emph_num = 2
            case 'u': emph_num = 4
            case _: return text
        color_prefix = f'\N{esc}[{color_num};{emph_num}m'
    else:
        color_prefix = f'\N{esc}[{color_num}m'

    return color_prefix + text + '\N{esc}[0m'



class RunStatus(Enum):
    SUCCESS = 1
    FAILED = 2


def get_parent_parser(input_reqd:bool = True, output_reqd:bool = False) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(add_help=False)

    parser.add_argument('-i', '--input', type=str, required=input_reqd, help='Path to the input file or directory')
    parser.add_argument('-o', '--output', type=str, required=output_reqd, help='Path to the output file or directory')

    return parser


def check_tool_exists(tool:str, exit:bool=True) -> bool:
    if shutil.which(tool) is None:
        print(color(f'{tool} not found.', 'red', 'b'))
        if exit:
            sys.exit(1)

        return False
    return True


def check_dir_exists(dir_path:str, create:bool) -> bool:
    if os.path.isdir(dir_path):
        return True
    elif not os.path.exists(dir_path):
        if create:
            print(f'Creating directory: {dir_path}')
            os.mkdir(dir_path)
            return True
        else:
            print(f'{dir_path} does not exist')
            return False
    else:
        print(f'{dir_path} is not a directory')
        exit(1)


def check_file_exists(file_path:str) -> bool:
    if not os.path.isfile(file_path):
        return False
    else:
        return True


if __name__ == '__main__':
    print('This file is a module and not intended to be run directly.')
