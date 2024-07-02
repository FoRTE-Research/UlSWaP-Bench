#!/bin/python

print('struct input_data_format {')
print('    char addr[16];')
print('};\n')

print('const struct input_data_format input_data[] = {')

infile = open('small.udp')

for line in infile:
    parts = line.strip().split()
    print('{' + '"' + parts[1] + '"' + '},')

print('};\n')

print('const struct input_data_format * fakeFile;')
