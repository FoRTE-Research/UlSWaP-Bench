#!/bin/python
import sys

# Need to know how many rows, since C requires that to be specified
numRows = 0
infile = open('input.dat')
for line in infile:
    if len(line.strip()) > 0:
        numRows += 1
infile.close()

print('#ifndef INPUT_H')
print('#define INPUT_H\n')
print('#include <stdint.h>\n')
print(f'#define NUM_NODES {numRows}')
print(f'const uint8_t AdjMatrix[{numRows}][{numRows}] = ')
print('{')

infile = open('input.dat')

for line in infile:
    parts = line.strip().split()
    print('    {', end='')
    for part in parts:
        # print with padding using spaces
        print('{:2d}, '.format(int(part)), end='')
    print('},')

print('};')
print('#endif  // INPUT_H')