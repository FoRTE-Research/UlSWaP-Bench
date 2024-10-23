from multiprocessing import Pool, Manager
from enum import Enum
import subprocess as sp
import sys
import os
import time

from utils import *
from instruction_parser import *

MAX_JOBS = 4
MAX_BUF_SIZE = 1024 * 1024 * 1024
CHUNK_LINES = 1024 * 1024 * 1024


class RunStatus(Enum):
    SUCCESS = 1
    FAILED = 2


class ParseRunResult:
    def __init__(self, status:RunStatus, benchmark:str, time_taken:float, message:str=None):
        self.status = status
        self.benchmark = benchmark
        self.time_taken = time_taken
        self.message = message


def parse_dump(dump_file:str, output_dir:str) -> ParseRunResult:
    print(f'Parsing {dump_file}')
    output_file = os.path.join(output_dir, dump_file.split('/')[-1])
    print(f'Output file: {output_file}')

    start = time.time()
    with open(dump_file, 'r', buffering=MAX_BUF_SIZE) as fp_dump, open(output_file, 'a', buffering=MAX_BUF_SIZE) as fp_out:
        for line in fp_dump:
            hex_instruction = line.strip()
            if (hex_instruction == '00100073'):     # ebreak
                break
            # bin_instruction = line.strip()
            # if (bin_instruction == '00000000000100000000000001110011'):
            #     break

            instruction = disassemble_riscv_op(hex_instruction)
            fp_out.write(f'{instruction}\n')

    end = time.time()
    time_taken = end - start
    return ParseRunResult(RunStatus.SUCCESS, dump_file, time_taken)


def parse_all_dumps(dump_dir:str, output_dir:str):
    files_to_parse = []
    for fname in os.listdir(dump_dir):
        files_to_parse.append(os.path.join(dump_dir, fname))

    num_jobs = MAX_JOBS
    job_pool = Pool(processes=num_jobs)

    for _, dump_file in enumerate(files_to_parse):
        job_pool.apply_async(parse_dump, args=(dump_file,output_dir), callback=success_callback, error_callback=failure_callback)

    job_pool.close()
    job_pool.join()



def success_callback(result:ParseRunResult):
    if result.status == RunStatus.SUCCESS:
        print(f'{result.benchmark} parsed sucessfully in {result.time_taken:.3f} seconds')
    else:
        print(f'{result.benchmark} failed after {result.time_taken:.3f} seconds')

    if result.message is not None:
        print(result.message)


def failure_callback(exception:BaseException):
    print('Process pool failure: ' + str(exception))


def main():
    args = parse_args()
    check_dir_exists(args.output_dir, True)

    if args.dump_file is not None:
        res = parse_dump(args.dump_file, args.output_dir)
        success_callback(res)
    else:
        check_dir_exists(args.dump_dir, False)
        parse_all_dumps(args.dump_dir, args.output_dir)


if __name__ == '__main__':
    main()