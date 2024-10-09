import random
import sys

NUM_NODES = 32
RAND_SEED = 1234
MAX_WEIGHT = 255

# Function to generate a random weight between min and max
def get_random_weight(min_weight, max_weight):
    # 33% chance of generating max wieght
    if random.randint(1, 3) == 1:
        return MAX_WEIGHT
    else:
        return random.randint(min_weight, max_weight)

def get_adjacency_matrix(size:int) -> list[list[int]]:
    adjacency_matrix = []
    for i in range(size):
        row = []
        for j in range(size):
            if i == j:
                row.append(0)  # Diagonal elements are set to 0
            else:
                row.append(get_random_weight(1, 50))  # Random weights between 1 and 50
        adjacency_matrix.append(row)

    return adjacency_matrix


def main():
    random.seed(RAND_SEED)
    output = ''

    output += '#ifndef INPUT_H\n'
    output += '#define INPUT_H\n\n'
    output += '#include <stdint.h>\n\n'
    output += f'#define NUM_NODES {NUM_NODES}\n\n'
    output += f'const uint8_t AdjMatrix[NUM_NODES][NUM_NODES] =\n'
    output += '{\n'

    adjacency_matrix = get_adjacency_matrix(NUM_NODES)
    for row in adjacency_matrix:
        output += '    {'
        for weight in row:
            output += '{:3d}, '.format(weight)
        output += '},\n'

    output += '};\n\n'
    output += '#endif  // INPUT_H\n'

    with open(sys.argv[1], 'w') as f:
        f.write(output)


if __name__ == "__main__":
    main()
