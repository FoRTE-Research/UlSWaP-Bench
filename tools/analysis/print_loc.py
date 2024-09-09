import os
import subprocess as sp
import sys

BENCHMARK_TOP_DIR = f'{os.path.dirname(os.path.abspath(__file__))}/../..'
BENCHMARK_SRC_DIR = f'{BENCHMARK_TOP_DIR}/src'

def get_benchmarks_loc() -> dict[str, int]:
    # Loop over all directories in BENCHMARK_SRC_DIR
    loc = {}
    for benchmark in os.listdir(BENCHMARK_SRC_DIR):
        benchmark_path = os.path.join(BENCHMARK_SRC_DIR, benchmark)
        if not os.path.isdir(benchmark_path):
            continue

        cmd = f'cloc {benchmark_path} --force-lang=C,h --include-lang=C'

        proc = sp.run(cmd, shell=True, capture_output=True)
        if proc.returncode != 0:
            print(f'Failed to run cloc for {benchmark}')
            print(proc.stderr.decode())
            sys.exit(1)

        output = proc.stdout.decode()
        cloc_lines = output.splitlines()
        total_loc = cloc_lines[-2].split()[-1]
        loc[benchmark] = int(total_loc)

    # Sort the dictionary by benchmark name
    loc = dict(sorted(loc.items()))

    return loc


def main():
    loc = get_benchmarks_loc()
    for benchmark, loc in loc.items():
        print(f'{benchmark},{loc}')


if __name__ == '__main__':
    main()
