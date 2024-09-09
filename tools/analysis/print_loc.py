import os
import subprocess as sp
import sys

BENCHMARK_TOP_DIR = f'{os.path.dirname(os.path.abspath(__file__))}/../..'
BENCHMARK_SRC_DIR = f'{BENCHMARK_TOP_DIR}/src'

def get_benchmarks_loc() -> dict[str, (int, int)]:
    # Loop over all directories in BENCHMARK_SRC_DIR
    loc = {}
    for benchmark in os.listdir(BENCHMARK_SRC_DIR):
        benchmark_path = os.path.join(BENCHMARK_SRC_DIR, benchmark)
        if not os.path.isdir(benchmark_path):
            continue

        cmd = f'cloc {benchmark_path} --include-lang="C,C/C++ Header" -not-match-f=input.h --hide-rate --quiet'

        proc = sp.run(cmd, shell=True, capture_output=True)
        if proc.returncode != 0:
            print(f'Failed to run cloc for {benchmark}')
            print(proc.stderr.decode())
            sys.exit(1)

        output = proc.stdout.decode()
        cloc_lines = output.splitlines()
        c_loc = cloc_lines[5].split()[-1]
        h_loc = cloc_lines[6].split()[-1] if 'Header' in cloc_lines[6] else 0
        loc[benchmark] = (int(c_loc), int(h_loc))

    # Sort the dictionary by benchmark name
    loc = dict(sorted(loc.items()))

    return loc


def main():
    loc = get_benchmarks_loc()
    for benchmark, loc in loc.items():
        print(f'{benchmark},{loc[0]},{loc[1]}')


if __name__ == '__main__':
    main()
