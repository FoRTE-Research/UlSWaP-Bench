import os
from utils import *

STACK_TOP = 520192

def get_runtime_memory(file_path:str) -> int:
    with open(file_path, 'r') as f:
        lines = f.readlines()

    low_watermark = STACK_TOP

    for line in lines[:-1]:
        count = int(line.split()[2])
        if count < low_watermark:
            low_watermark = count

    return STACK_TOP - low_watermark

def get_runtime_memory_map(directory:str) -> dict:
    runtime_memory = {}
    for file in os.listdir(directory):
        bench_name = file
        file_path = os.path.join(directory, file)
        count = get_runtime_memory(file_path)
        runtime_memory[bench_name] = count

    return runtime_memory
