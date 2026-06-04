import shutil
import sys
import time
import os
import subprocess as sp
from multiprocessing import Pool

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BENCH_TOP_DIR = os.path.join(SCRIPT_DIR, '../..')
UTILS_DIR = os.path.join(BENCH_TOP_DIR, 'tools/analysis')
sys.path.insert(0, UTILS_DIR)
from utils import *

BENCHMARK_TEST_DIR = BENCH_TOP_DIR + '/test'
BENCHMARK_COMPLETION_MSG = 'Benchmark execution complete.'
CHECKSUM_FILE_SIZE = 10
SPIKE_TIMEOUT = 60
MAX_JOBS = 10

class VerificationMethod(Enum):
    PRINTF = 'printf'
    CHECKSUM = 'checksum'


def run_spike(elf_file:str, output_dir:str, verif_method:VerificationMethod=VerificationMethod.PRINTF) -> None:
    benchmark = os.path.basename(elf_file).split('.')[0]
    output_file = f'{output_dir}/{benchmark}'
    msg = f'{benchmark:>13}: '

    cmd = f'spike --isa=RV32IM {elf_file} > {output_file}'
    start = time.time()
    try:
        proc = sp.run(cmd, shell=True, capture_output=True, text=True, timeout=SPIKE_TIMEOUT)
    except sp.TimeoutExpired:
        msg += color('Timed out', 'red', 'b')
        msg += f' after {SPIKE_TIMEOUT} seconds'
        print(msg)
        return
    end = time.time()
    time_taken = end - start

    if proc.returncode != 0:
        msg += color('Runtime failure', 'red', 'b')
        print(msg)
        return

    msg += f'Completed in {time_taken:>6.2f} seconds. '
    result, _ = check_benchmark_output(benchmark, output_dir, verif_method)
    if result:
        msg += color('Passed', 'green', 'b')
    else:
        msg += color('Failed', 'red', 'b')

    print(msg)


def run_all_spike(elf_dir:str, output_dir:str, verif_method:VerificationMethod=VerificationMethod.PRINTF) -> None:
    elf_files_to_run = []
    for fname in os.listdir(elf_dir):
        if fname.endswith('.elf'):
            elf_files_to_run.append(os.path.join(elf_dir, fname))

    num_jobs = MAX_JOBS
    job_pool = Pool(processes=num_jobs)

    for _, elf_file in enumerate(elf_files_to_run):
        job_pool.apply_async(run_spike, args=(elf_file,output_dir,verif_method))

    job_pool.close()
    job_pool.join()


def check_benchmark_output(benchmark:str, output_dir:str, verif_method:VerificationMethod) -> tuple[bool,str]:
    test_output_file = f'{output_dir}/{benchmark}'

    if verif_method == VerificationMethod.PRINTF:
        gold_output_file = f'{BENCHMARK_TEST_DIR}/reference_output/{benchmark}'
        cmd = f'python3 {BENCHMARK_TEST_DIR}/check_output.py {gold_output_file} {test_output_file} -d 0.01'
        proc = sp.run(cmd, shell=True, capture_output=True, text=True)

        if proc.stdout.strip() == 'Files match.':
            return (True, '')
        else:
            return (False, proc.stdout)

    elif verif_method == VerificationMethod.CHECKSUM:
        gold_output_file = f'{BENCHMARK_TEST_DIR}/checksums/{benchmark}'
        with open(gold_output_file, 'r') as f:
            gold_checksum = f.read().strip()
        with open(test_output_file, 'r') as f:
            test_checksum = f.readlines()[0].strip()

        if gold_checksum == test_checksum:
            return (True, '')
        else:
            return (False, f'Expected checksum: {gold_checksum}    Got checksum: {test_checksum}')


def check_spike_exists() -> None:
    if shutil.which('spike') is None:
        print(color('Error: spike command not found.', 'red'))
        sys.exit(1)


def main():
    parser = argparse.ArgumentParser(parents=[get_parent_parser()], description='Test benchmarks using Spike')
    parser.add_argument('-m', '--method', type=str, choices=['printf', 'checksum'], default='printf', help='Verification method to use (default: printf)')
    args = parser.parse_args()

    if not args.input or not args.output:
        print(color("Input and output directories must be specified. Exiting...", 'red'))
        sys.exit(1)
    check_dir_exists(args.output, create=True)

    check_spike_exists()
    if os.path.isfile(args.input):
        run_spike(args.input, args.output, VerificationMethod(args.method))
    elif check_dir_exists(args.input, create=False):
        run_all_spike(args.input, args.output, VerificationMethod(args.method))
    else:
        print('Invalid input. Please provide a valid elf file or directory containing elf files.')


    return


if __name__ == '__main__':
    main()
