import sys
import random

RAND_SEED = 1234
ARRAY_SIZE = 640

def main():
    output = ''

    output += '#ifndef __INPUT_H__\n'
    output += '#define __INPUT_H__\n\n'
    output += '#include <stdint.h>\n\n'
    output += '#define ARRAY_SIZE %d\n\n' % ARRAY_SIZE

    output += 'typedef struct {\n'
    output += '    int16_t x, y, z;\n'
    output += '    float distance;\n'
    output += '} Vertex;\n\n'
    output += 'Vertex vertices[] =  {\n'

    random.seed(RAND_SEED)
    minval = -(2 ** 15)
    maxval = 2 ** 15 - 1
    for _ in range(ARRAY_SIZE):
        x = random.randint(minval, maxval)
        y = random.randint(minval, maxval)
        z = random.randint(minval, maxval)

        output += '    { %6d, %6d, %6d, %3.1f },\n' % (x, y, z, 0)

    output += '};\n\n'
    output += '#endif  // __INPUT_H__\n'

    with open(sys.argv[1], 'w') as f:
        f.write(output)


if __name__ == '__main__':
    main()
