import numpy as np

def read_matrix(filename):
    with open(filename) as f:
        n = int(f.readline())
        M = []
        for _ in range(n):
            row = list(map(float, f.readline().split()))
            M.append(row)
        return np.array(M)

A = read_matrix("matrix_A.txt")
B = read_matrix("matrix_B.txt")
C = read_matrix("matrix_C.txt")

C_true = np.dot(A, B)

if np.allclose(C, C_true):
    print("Матрицы совпадают.")
else:
    print("Матрицы не совпадают.")