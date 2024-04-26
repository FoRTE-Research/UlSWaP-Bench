import random
import sys

# Function to generate a random weight between min and max
def get_random_weight(min_weight, max_weight):
    return random.randint(min_weight, max_weight)

def main(size):
    adjacency_matrix = []

    # Generating random weights for the adjacency matrix
    for i in range(size):
        row = []
        for j in range(size):
            if i == j:
                row.append(0)  # Diagonal elements are set to 0
            else:
                row.append(get_random_weight(1, 20))  # Random weights between 1 and 100
        adjacency_matrix.append(row)

    # Printing the adjacency matrix
    for row in adjacency_matrix:
        print(" ".join(map(str, row)))

if __name__ == "__main__":
    main(int(sys.argv[1]))
