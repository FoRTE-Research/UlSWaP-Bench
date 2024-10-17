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


def print_summaries_latex(summaries:dict[str,BenchmarkSummary]) -> None:
    for category in ALL_BENCHMARKS.keys():
        print(' ' * 8 + f'\\multirow{{{len(ALL_BENCHMARKS[category])}}}{{*}}{{\\rotatebox[origin=c]{{90}}{{{category}}}}}')
        for benchmark in ALL_BENCHMARKS[category]:
            summary = summaries[benchmark]
            escaped_benchmark = benchmark.replace('_', '\_')
            print(' ' * 8 + f'& {escaped_benchmark} & {summary.nvm} & {summary.ram} & {summary.device_class.name} \\\\')
        print(' ' * 8 + '\hline')

    return


def main():
    mem_map = get_mem_map(RISCV_CSV_FILE)
    summaries = get_benchmark_summaries(mem_map)
    print_summaries_latex(summaries)
    return


if __name__ == '__main__':
    main()
