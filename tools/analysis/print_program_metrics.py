import os
import sys
import json
import tempfile as tf
import subprocess as sp
from utils import *
from print_unused_functions import get_unused_functions

BENCHMARK_TOP_DIR = f'{os.path.dirname(os.path.abspath(__file__))}/../..'
BENCHMARK_SRC_DIR = f'{BENCHMARK_TOP_DIR}/src'
PLUGIN_LIB = f'{BENCHMARK_TOP_DIR}/tools/analysis/IR/build/lib/libBasicBlockInstCounter.so'

INPUT_HEADER_NAMES = ['input.h', 'data.h', 'mnist_resnet_q.h', 'inputs.c', 'inputs.h']
INPUT_HEADERS_REGEX = '|'.join([f'{header}' for header in INPUT_HEADER_NAMES])


class BenchmarkProperties:
    def __init__(self, name:str, c_loc:int, h_loc:int, num_basic_blocks:int, num_instructions:int, muldiv_usage:str):
        self.name = name
        self.c_loc = c_loc
        self.h_loc = h_loc
        self.num_basic_blocks = num_basic_blocks
        self.num_instructions = num_instructions

        if muldiv_usage == 'none':
            self.muldiv_usage = '-'
        elif muldiv_usage == 'double':
            self.muldiv_usage = 'D'
        elif muldiv_usage == 'float':
            self.muldiv_usage = 'F'
        elif muldiv_usage == 'int':
            self.muldiv_usage = 'I'
        return


def get_files_to_analyze(benchmark:str, build_dir:str) -> list[str]:
    obj_dir = f'{build_dir}/CMakeFiles/{benchmark}.dir/src/{benchmark}'
    obj_files = [f for f in os.listdir(obj_dir) if f.endswith('.c.o')]
    c_files = [BENCHMARK_TOP_DIR + f'/src/{benchmark}/{f[:-2]}' for f in obj_files]
    return c_files


def get_ir_properties(benchmark:str, build_dir:str) -> tuple[int, int]:
    num_basic_blocks = 0
    num_instructions = 0

    c_files = get_files_to_analyze(benchmark, build_dir)
    unused_funcs = get_unused_functions(benchmark, build_dir)

    with tf.TemporaryDirectory() as temp_dir:
        for c_file in c_files:
            fname = os.path.basename(c_file)
            cmd = f'clang-15 -S -emit-llvm -o {temp_dir}/{fname}.ll {c_file} -I {BENCHMARK_TOP_DIR}/src'
            sp.run(cmd, shell=True, check=True, capture_output=True)

            cmd = f'opt-15 -load-pass-plugin {PLUGIN_LIB} -passes=print-basic-block-inst-counter -disable-output {temp_dir}/{fname}.ll'
            proc = sp.run(cmd, shell=True, capture_output=True)
            output = proc.stderr.decode()

            for line in output.splitlines():
                func = line.split(',')[0]
                if func not in unused_funcs:
                    num_basic_blocks += int(line.split(',')[1])
                    num_instructions += int(line.split(',')[2])

    return num_basic_blocks, num_instructions


def get_benchmark_loc(benchmark:str) -> tuple[int, int]:
    benchmark_path = os.path.join(BENCHMARK_SRC_DIR, benchmark)

    cmd = f'cloc {benchmark_path} --include-lang="C,C/C++ Header" --not-match-f="{INPUT_HEADERS_REGEX}" --hide-rate --quiet --json'
    proc = sp.run(cmd, shell=True, capture_output=True)
    if proc.returncode != 0:
        print(f'Failed to run cloc for {benchmark}')
        print(proc.stderr.decode())
        sys.exit(1)

    output = proc.stdout.decode()
    cloc_data = json.loads(output)

    # Make sure the only languages in the cloc output are C and C/C++ Header
    for lang in cloc_data.keys():
        if lang not in ['C', 'C/C++ Header', 'header', 'SUM']:
            print(f'Unexpected language in cloc output for {benchmark}: {lang}')
            sys.exit(1)

    c_loc = cloc_data.get('C').get('code')
    h_loc = cloc_data.get('C/C++ Header', {}).get('code', 0)

    return c_loc, h_loc


def get_benchmark_properties(benchmark:str, build_dir:str) -> BenchmarkProperties:
    c_loc, h_loc = get_benchmark_loc(benchmark)
    num_basic_blocks, num_instructions = get_ir_properties(benchmark, build_dir)
    muldiv_usage = MULDIV_USAGE[benchmark]

    return BenchmarkProperties(benchmark, c_loc, h_loc, num_basic_blocks, num_instructions, muldiv_usage)


def get_properties_csv(all_benchmark_props:dict[str,BenchmarkProperties]) -> str:
    output = 'Benchmark,C LOC,H LOC,Basic Blocks,Instructions,MulDiv Usage\n'
    for benchmark in all_benchmark_props.values():
        output += f'{benchmark.name},{benchmark.c_loc},{benchmark.h_loc},{benchmark.num_basic_blocks},{benchmark.num_instructions},{benchmark.muldiv_usage} \n'

    return output


def get_properties_latex(all_benchmark_props:dict[str,BenchmarkProperties]) -> str:
    output = ''
    for category in ALL_BENCHMARKS.keys():
        output += ' ' * 8 + f'\\multirow{{{len(ALL_BENCHMARKS[category])}}}{{*}}{{\\rotatebox[origin=c]{{90}}{{{category}}}}}' + '\n'
        for benchmark in ALL_BENCHMARKS[category]:
            benchmark_props = all_benchmark_props[benchmark]
            escaped_benchmark = benchmark.replace('_', '\\_')
            output += ' ' * 8 + f'& {escaped_benchmark} & {benchmark_props.c_loc} & {benchmark_props.h_loc} & ' +\
                  f'{benchmark_props.num_basic_blocks} & {benchmark_props.num_instructions} & {benchmark_props.muldiv_usage} \\\\' + '\n'
        output += ' ' * 8 + '\\hline' + '\n'

    return output


def print_properties_table(benchmark_props:dict[str,BenchmarkProperties]) -> None:
    print('Benchmark     | C LOC | H LOC | Basic Blocks | Instructions | MulDiv Usage')
    print('--------------|-------|-------|--------------|--------------|-------------')
    for benchmark in benchmark_props.values():
        print(f'{benchmark.name:<13} | {benchmark.c_loc:>5} | {benchmark.h_loc:>5} | {benchmark.num_basic_blocks:>12} | {benchmark.num_instructions:>12} | {benchmark.muldiv_usage:>11}')

    return


help_msg = '''
This script retrieves and outputs the following program metrics for each benchmark: lines of code in C files and header files,
number of LLVM-IR basic blocks, and number of LLVM-IR instructions.
The input directory should be the top-level CMake build directory.
If no output file is specified, the metrics will be printed to the console.
If an output file is specified, it will be written in CSV format or LaTeX table format (body only) depending on the file extension.
'''

def main():
    parent_parser = get_parent_parser()
    parser = argparse.ArgumentParser(parents=[parent_parser], description=help_msg)
    parser.add_argument('--benchmark', type=str, help='Individual benchmarks to analyze', choices=get_bench_names(), default=None)
    args = parser.parse_args()
    build_dir = args.input
    output_file = args.output

    if not check_file_exists(PLUGIN_LIB):
        print('LLVM plugin library not found. Please build the plugin first.')
        return

    if not check_dir_exists(build_dir, False):
        return

    all_benchmark_props = {}
    output = ''

    if args.benchmark is not None:
        if output_file is not None and output_file.endswith('.tex'):
            print('LaTeX output is not supported for individual benchmarks.')
            return

        benchmark_props = get_benchmark_properties(args.benchmark, build_dir)
        all_benchmark_props[benchmark_props.name] = benchmark_props
    else:
        for benchmark in get_bench_names():
            benchmark_props = get_benchmark_properties(benchmark, build_dir)
            all_benchmark_props[benchmark] = benchmark_props

    if output_file is None:
        print_properties_table(all_benchmark_props)
    elif output_file.endswith('.csv'):
        output = get_properties_csv(all_benchmark_props)
    elif output_file.endswith('.tex'):
        output = get_properties_latex(all_benchmark_props)
    else:
        print('Unsupported output format. Please use .csv or .tex.')
        return

    if output_file is not None:
        with open(output_file, 'w') as f:
            f.write(output)

    return


if __name__ == '__main__':
    main()
