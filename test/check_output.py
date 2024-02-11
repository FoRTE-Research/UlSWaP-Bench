import argparse

def compare_files(file1_name, file2_name, max_delta):
    mismatched_lines = []
    try:
        with open(file1_name, 'r') as file1, open(file2_name, 'r') as file2:
            while True:
                line_file1 = file1.readline().strip()
                line_file2 = file2.readline().strip()

                # Check for EOF
                if not line_file1 and not line_file2:
                    break

                # Handle blank lines in file 1
                while not line_file1 and line_file2:
                    line_file1 = file1.readline().strip()

                # Handle blank lines in file 2
                while line_file1 and not line_file2:
                    line_file2 = file2.readline().strip()

                words1 = line_file1.split()
                words2 = line_file2.split()

                if len(words1) != len(words2):
                    mismatched_lines.append((line_file1, line_file2))
                else:
                    for word1, word2 in zip(words1, words2):
                        try:
                            float1 = float(word1)
                            float2 = float(word2)
                            if abs(float1 - float2) > max_delta:
                                mismatched_lines.append((line_file1, line_file2))
                                break
                        except ValueError:
                            if word1 != word2:
                                mismatched_lines.append((line_file1, line_file2))
                                break

        return mismatched_lines

    except FileNotFoundError:
        print("One or both files not found.")
        return []


def main():
    parser = argparse.ArgumentParser(description='Compare two files word by word.')
    parser.add_argument('file1', help='First file name')
    parser.add_argument('file2', help='Second file name')
    parser.add_argument('-d', '--max_delta', type=float, default=0.0001, help='Maximum allowable difference for floating-point comparison')

    args = parser.parse_args()

    file1 = args.file1
    file2 = args.file2
    max_delta = args.max_delta

    mismatched_lines = compare_files(file1, file2, max_delta)

    if not mismatched_lines:
        print("Files match.")
    else:
        print("Files do not match. Mismatched lines:")
        for line1, line2 in mismatched_lines:
            print(f"File 1: {line1}\nFile 2: {line2}\n")


if __name__ == "__main__":
    main()
