import argparse
import numpy as np


parser = argparse.ArgumentParser(description="Zadanie z Laboratorium #5 \
                                 z Bioinformatyki Strukturalnej")
parser.add_argument("seq", help="sekwencja")
args = parser.parse_args()

allowed = [('G', 'C'), ('C', 'G'), ('A', 'U'), ('U', 'A')]


def fill_the_matrix(seq):
    seq_length = len(seq)
    matrix = np.zeros(shape=(seq_length, seq_length))
    for i in range(1, seq_length):
        for j in range(seq_length - i):
            row = j
            col = i + j
            candidates = []

            for k in range(row, col):
                candidates.append(matrix[row, k] + matrix[k + 1, col])

            nucleotide_pair = (seq[row], seq[col])
            if nucleotide_pair in allowed:
                candidates.append(matrix[row + 1, col - 1] + 1)
            else:
                candidates.append(matrix[row + 1, col - 1])

            matrix[row, col] = max(candidates)
    return matrix


def traceback(i, j, seq, matrix, structure):
    if i < j:
        paired = 0
        if (seq[i], seq[j]) in allowed:
            paired = 1
        if matrix[i, j] == matrix[i + 1, j]:
            traceback(i + 1, j, seq, matrix, structure)
        elif matrix[i, j] == matrix[i, j - 1]:
            traceback(i, j - 1, seq, matrix, structure)
        elif matrix[i, j] == matrix[i + 1, j - 1] + paired:
            # print(i, "\t", j)
            structure.append((i, j))
            traceback(i + 1, j - 1, seq, matrix, structure)
        else:
            for k in range(i + 1, j - 1):
                if matrix[i, j] == matrix[i, k] + matrix[k + 1, j]:
                    traceback(i, k, seq, matrix, structure)
                    traceback(k + 1, j, seq, matrix, structure)
                    break

    return structure


def dot_bracket(seq, structure):
    dot_bracket = ["." for _ in range(len(seq))]
    for pair in structure:
        dot_bracket[pair[0]] = "("
        dot_bracket[pair[1]] = ")"
    return "".join(dot_bracket)


def nussinov(seq):
    matrix = fill_the_matrix(seq)
    structure = []
    structure = traceback(0, len(seq) - 1, seq, matrix, structure)
    dot_bracket_string = dot_bracket(seq, structure)
    print(dot_bracket_string)


nussinov(args.seq)
