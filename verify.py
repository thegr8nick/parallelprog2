import numpy as np

def read_matrix(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    dims = list(map(int, lines[0].split()))
    data = [list(map(float, line.split())) for line in lines[1:]]
    return np.array(data)

A = read_matrix('matrixA.txt')
B = read_matrix('matrixB.txt')
C_cpp = read_matrix('result.txt')

C_np = np.dot(A, B)

if np.allclose(C_cpp, C_np):
    print("Результат совпадает с NumPy!")
else:
    print("Ошибка: результат не совпадает с NumPy.")
    print("Ожидалось:\n", C_np)
    print("Получено:\n", C_cpp) 