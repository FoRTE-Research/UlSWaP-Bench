import os
import matplotlib.pyplot as plt
import matplotlib as mpl
import matplotlib.colors as mcolors
import matplotlib.patches as mpatches
import numpy as np

from utils import ALL_BENCHMARKS


PAGEWIDTH = 7
COLUMNWIDTH = 3.34

NVM_MEM_TYPES = ['.text', '.rodata', '.data']
RAM_MEM_TYPES = ['.data', '.bss', 'stack']
ARCH_COLORS = ['purple', 'tab:orange', 'yellow']
MEM_COLORS  = ['purple', 'tab:orange', 'yellow']
ARCH_HATCHES = ['///', '....', '\\\\\\']
ARCH_LABELS = {
    'riscv': 'RISC-V',
    'msp430': 'MSP430',
    'arm': 'ARM'
}

def get_luminescence(named_color:str) -> float:
    r, g, b = mcolors.to_rgb(named_color)
    lum = 0.2126*r + 0.7152*g + 0.0722*b
    return round(lum, 3)


def get_label_xtick_positions() -> list[float]:
    positions = []
    bench_idx = 0
    for i, bench_group in enumerate(ALL_BENCHMARKS.keys()):
        positions.append(bench_idx + (len(ALL_BENCHMARKS[bench_group]) - 0.5) / 2)
        bench_idx += len(ALL_BENCHMARKS[bench_group])

    return positions


def get_line_xticks() -> list[float]:
    positions = [-0.25]
    bench_idx = 0
    for bench_group in ALL_BENCHMARKS.values():
        positions.append(bench_idx + len(bench_group) - 0.25)
        bench_idx += len(bench_group)

    return positions


def get_compact_num(num:int) -> str:
    if num < 1000:
        return str(num)
    elif num < 10000:
        return f'{round(num / 1000, 1)}K'
    elif num < 10**6:
        return f'{round(num / 1000)}K'
    elif num < 10**7:
        return f'{round(num / 10**6, 1)}M'
    elif num < 10**9:
        return f'{round(num / 10**6)}M'
    elif num < 10**10:
        return f'{round(num / 10**9, 1)}G'
    elif num < 10**11:
        return f'{round(num / 10**9)}G'
    else:
        return 'uh-oh'


def crop_pdf(filename:str) -> None:
    os.system(f'pdfcrop {filename} {filename}')
    return


if __name__ == '__main__':
    print('This file is a module and not intended to be run directly.')
