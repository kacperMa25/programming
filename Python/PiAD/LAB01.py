import numpy as np

# zadanie 1
print("\nZadanie 1.\n")
matrix = np.random.randint(1, 10, size=(10, 5))
print("Suma na przekatnej = ", np.trace(matrix))
print("Elementy na przekatnej = ", np.diag(matrix))

# zadanie 2
print("\nZadanie 2.\n")
a = np.random.normal(size=10)
b = np.random.normal(size=10)

print("Pomnozone wartosci rozkladu normalnego = ", a * b, sep="\n")

# zadanie 3
print("\nZadanie 3.\n")
a = np.random.randint(1, 101, size=5)

b = np.random.randint(1, 101, size=5)

matrixA = np.array((a, a, a, a, a)).transpose()
matrixB = np.array((b, b, b, b, b)).transpose()
print(matrixA + matrixB)

# zadanie 4
print("\nZadanie 4.\n")
matrixA = np.ones((5, 4))
matrixB = np.ones((4, 5))

print(np.reshape(matrixA, (4, 5)) + matrixB)

# zadanie 5
print("\nZadanie 5.\n")
A = np.arange(1, 26).reshape(5, 5)
print("Oryginalna macierz:\n", A)

col3 = A[:, 2]
col4 = A[:, 3]

result = col3 * col4

print("\nWynik mnożenia kolumn 3 i 4:\n", result)


# zadanie 6
print("\nZadanie 6.\n")
matrixA = np.random.normal(size=(3, 3))
matrixB = np.random.uniform(size=(3, 3))

print(
    "Średnia ",
    "rozkladu normalnego: ",
    np.average(matrixA),
    ", rozkladu jednostajego: ",
    np.average(matrixB),
)

print(
    "Odchylenie: ",
    "rozkladu normalego: ",
    np.std(matrixA),
    ", rozkladu jednostajego",
    np.std(matrixB),
)

print(
    "Suma elementow: ",
    "rozkladu normalnego: ",
    np.sum(matrixA),
    ", rozkladu jednostajego: ",
    np.sum(matrixB),
)

print(
    "Miniumum: ",
    "rozkladu nromalego:",
    np.min(matrixA),
    ", rozkladu jednostajnego: ",
    np.min(matrixB),
)


print(
    "Maksimum: ",
    "rozkladu nromalego:",
    np.max(matrixA),
    ", rozkladu jednostajnego: ",
    np.max(matrixB),
)

# zadanie 7
print("\nZadanie 7.\n")
matrixA = np.random.randint(1, 11, size=(3, 3))
matrixB = np.random.randint(1, 11, size=(3, 3))

print(matrixA, matrixB, "\n", sep="\n\n")
print(matrixA * matrixB, np.dot(matrixA, matrixB), sep="\n\n")
# Różnica polega na tym, że funkcja dot mnoży
# macierze tablicowo tzn. Z[i, j] = X[i, :] * Y[:, j],
# a użycie operatora * sprawia, że Z[i, j] = X[i, j] * Y[i, j]

# zadanie 8
print("\nZadanie 8.\n")
A = np.arange(36).reshape(6, 6)

strides = A.strides

B = np.lib.stride_tricks.as_strided(A, shape=(3, 5), strides=strides)
print(A, B, sep="\n\n")

# zadanie 9
print("\nZadanie 9.\n")
A = np.arange(12).reshape(2, 6)
B = np.arange(12, 24).reshape(2, 6)

C = np.hstack((A, B))
C = np.vstack((C, B.reshape(1, 12)))
print(C)
# Funkcje te warto używać, gdy chcemy połączyć
# ze sobą dwie tablice/macierze pionowo bądź poziomo.
# Mogą służyć również do dodawania kolumn/wierszy do macierzy.

# zadanie 10
print("\nZadanie 10.\n")

M = np.arange(24, dtype=np.int32).reshape(4, 6)

strides = M.strides
yellow = np.lib.stride_tricks.as_strided(M, shape=(2, 3), strides=strides)

green = np.lib.stride_tricks.as_strided(M[:, 3:], shape=(2, 3), strides=strides)

blue = np.lib.stride_tricks.as_strided(M[2:, :], shape=(2, 3), strides=strides)
red = np.lib.stride_tricks.as_strided(M[2:, 3:], shape=(2, 3), strides=strides)
print(yellow, green, blue, red, sep="\n\n")

print("Maks yellow=", np.max(yellow))
print("Maks green=", np.max(green))
print("Maks blue=", np.max(blue))
print("Maks red=", np.max(red))
