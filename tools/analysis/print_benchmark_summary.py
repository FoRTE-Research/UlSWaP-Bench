import csv
from utils import *

RISCV_CSV_FILE = 'output/riscv_totmem.csv'

class DeviceClass(Enum):
    Tiny = 1
    Small = 2


class BenchmarkSummary:
    def __init__(self, name:str, nvm:int, ram:int):
        self.name = name
        self.nvm = nvm
        self.ram = ram

        if nvm < 65536 and ram < 8192:
            self.device_class = DeviceClass.Tiny
        else:
            self.device_class = DeviceClass.Small

        return


def get_mem_map(filename:str) -> dict[str, dict[str, int]]:
    mem_map = {}

    with open(filename, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            mem_map[row['Benchmark']] = {
                '.text': int(row['.text']),
                '.rodata': int(row['.rodata']),
                '.data': int(row['.data']),
                '.bss': int(row['.bss']),
                'stack': int(row['stack']),
            }

    return mem_map


def get_bench_mem(benchmark:str, mem_map:dict[str, dict[str, int]]) -> tuple[int, int]:
    sizes = mem_map[benchmark]
    nvm_size = sizes['.text'] + sizes['.rodata'] + sizes['.data']
    ram_size = sizes['.data'] + sizes['.bss'] + sizes['stack']

    return nvm_size, ram_size


def get_benchmark_summary(benchmark:str, mem_map:dict[str, dict[str, int]]) -> BenchmarkSummary:
    nvm, ram = get_bench_mem(benchmark, mem_map)

    return BenchmarkSummary(benchmark, nvm, ram)


def get_benchmark_summaries(mem_map:dict[str, dict[str, int]]) -> dict[str,BenchmarkSummary]:
    summaries = {}
    for bench in get_bench_names():
        summaries[bench] = get_benchmark_summary(bench, mem_map)

    return summaries


def get_summaries_latex(summaries:dict[str,BenchmarkSummary]) -> str:
    output = ''
    for category in ALL_BENCHMARKS.keys():
        output += ' ' * 8 + f'\\multirow{{{len(ALL_BENCHMARKS[category])}}}{{*}}{{\\rotatebox[origin=c]{{90}}{{{category}}}}}' + '\n'
        for benchmark in ALL_BENCHMARKS[category]:
            summary = summaries[benchmark]
            escaped_benchmark = benchmark.replace('_', '\_')
            output += ' ' * 8 + f'& {escaped_benchmark} & {summary.nvm} & {summary.ram} & {summary.device_class.name} \\\\' + '\n'
        output += ' ' * 8 + '\hline' + '\n'

    return output


def get_summaries_csv(summaries:dict[str,BenchmarkSummary]) -> str:
    output = 'Benchmark,NVM,RAM,Device Class\n'
    for summary in summaries.values():
        output += f'{summary.name},{summary.nvm},{summary.ram},{summary.device_class.name}\n'

    return output


def print_summaries_table(summaries:dict[str,BenchmarkSummary]) -> None:
    print('Benchmark     | NVM (bytes) | RAM (bytes) | Device Class')
    print('--------------|-------------|-------------|-------------')
    for summary in summaries.values():
        print(f'{summary.name:<13} | {summary.nvm:>11} | {summary.ram:>11} | {summary.device_class.name:^13}')

    return


help_msg = '''
This script prints a summary of all benchmarks including their name, NVM usage, RAM usage, and device class.
The input file should be a CSV file with the following columns: Benchmark, .text, .rodata, .data, .bss, stack.
If no output file is specified, the summaries will be printed to the console.
If an output file is specified, it will be written in CSV format or LaTeX table format (body only) depending on the file extension.
'''


def main():
    parent_parser = get_parent_parser()
    parser = argparse.ArgumentParser(parents=[parent_parser], description=help_msg)
    args = parser.parse_args()
    input_file = args.input
    output_file = args.output

    if check_file_exists(input_file):
        mem_map = get_mem_map(input_file)
        summaries = get_benchmark_summaries(mem_map)

        if output_file is None:
            print_summaries_table(summaries)
        elif output_file.endswith('.csv'):
            output = get_summaries_csv(summaries)
        elif output_file.endswith('.tex'):
            output = get_summaries_latex(summaries)
        else:
            print('Unsupported output format. Please use .csv or .tex.')
            return

        if output_file is not None:
            with open(output_file, 'w') as f:
                f.write(output)

    return


if __name__ == '__main__':
    main()
