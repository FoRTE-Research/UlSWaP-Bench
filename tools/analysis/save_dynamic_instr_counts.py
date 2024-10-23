from utils import *
from instruction_parser import *

def create_instruction_count_map(instr_dump_file:str, output_dir:str) -> dict[str, int]:
    instr_count_map = {}
    for instr in RISCV_INSTRUCTIONS:
        instr_count_map[instr] = 0

    with open(instr_dump_file, 'r') as f:
        lines = f.readlines()

    for line in lines:
        instruction = line.strip()
        instr_count_map[instruction] = instr_count_map.get(instruction, 0) + 1

    output_filename = instr_dump_file.split('/')[-1] + '.csv'
    output_filepath = os.path.join(output_dir, os.path.basename(output_filename))
    with open(output_filepath, 'w') as f:
        for instr, count in instr_count_map.items():
            f.write(f'{instr},{count}\n')

    return instr_count_map


def get_instruction_count_maps(dump_dir:str, output_dir:str) -> dict[str, dict[str, int]]:
    benchmark_maps = {}

    for filename in os.listdir(dump_dir):
        instr_count_map = create_instruction_count_map(os.path.join(dump_dir, filename), output_dir)
        benchname = filename.split('/')[-1]
        benchmark_maps[benchname] = instr_count_map

    return benchmark_maps


def main():
    args = parse_args()
    check_dir_exists(args.dump_dir, create=False)
    check_dir_exists(args.output_dir, create=True)
    get_instruction_count_maps(args.dump_dir, args.output_dir)
    return


if __name__ == '__main__':
    main()
