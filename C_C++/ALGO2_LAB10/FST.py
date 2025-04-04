import numpy as np

a = np.array([])

for i in range(1, 4):
    n = 1 << i

    print("Rozmiar: ", n)
    for j in range(n):
        a = np.append(a, [j / n])

    array = np.fft.fft(a)

    a = np.array([])
    print(array, "\n")

a = np.array([-4, 3, 8, -1])
print(np.fft.fft(a))
