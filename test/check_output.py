import argparse

def compare_files(file1_name:str, file2_name:str, max_delta:float) -> bool:
    align = max(len(file1_name), len(file2_name)) + 1
    files_match = True

    try:
        with open(file1_name, 'r') as file1, open(file2_name, 'r') as file2:
            lines1 = file1.readlines()
            lines2 = file2.readlines()

            try:
                for line1, line2 in zip(lines1, lines2, strict=True):
                    lines_match = True
                    words1 = line1.split()
                    words2 = line2.split()

                    if len(words1) != len(words2):
                        lines_match = False
                        files_match = False

                    for word1, word2 in zip(words1, words2):
                        try:
                            float1 = float(word1)
                            float2 = float(word2)
                            if abs(float1 - float2) > max_delta:
                                lines_match = False
                                files_match = False
                                break
                        except ValueError:
                            if word1 != word2:
                                lines_match = False
                                files_match = False
                                break

                    if not lines_match:
                        print('{0:{1}}: {2}'.format(file1_name, align, line1.strip()))
                        print('{0:{1}}: {2}'.format(file2_name, align, line2.strip()))
                        print()

            except ValueError:
                if len(lines1) > len(lines2):
                    print(f'{file1_name} has {len(lines1) - len(lines2)} more lines than {file2_name}')
                else:
                    print(f'{file2_name} has {len(lines2) - len(lines1)} more lines than {file1_name}')
                return False

    except FileNotFoundError:
        print("One or both files not found.")
        return False

    return files_match


def main():
    parser = argparse.ArgumentParser(description='Compare two files word by word.')
    parser.add_argument('file1', help='First file name')
    parser.add_argument('file2', help='Second file name')
    parser.add_argument('-d', '--max_delta', type=float, default=0.0001, help='Maximum allowable difference for floating-point comparison')

    args = parser.parse_args()

    file1 = args.file1
    file2 = args.file2
    max_delta = args.max_delta

    result = compare_files(file1, file2, max_delta)

    if result:
        print("Files match.")


if __name__ == "__main__":
    main()
