import numpy as np


def negate(bit):
    return 0 if bit == 1 else 1


def hamming(signal, n=7, k=4):
    m = n - k
    codedSignal = np.zeros(n, dtype=int)
    ps = np.zeros(m)
    ps[0] = (signal[0] + signal[1] + signal[3]) % 2
    ps[1] = (signal[0] + signal[2] + signal[3]) % 2
    ps[2] = (signal[1] + signal[2] + signal[3]) % 2

    for i in range(m):
        codedIndex = 2**i - 1
        codedSignal[codedIndex] = ps[i]

    codedSignal[2] = signal[0]
    codedSignal[4] = signal[1]
    codedSignal[5] = signal[2]
    codedSignal[6] = signal[3]
    return codedSignal


def decodeHamming(signal):
    decoded = np.copy(signal)
    x1p = (signal[2] + signal[4] + signal[6]) % 2
    x2p = (signal[2] + signal[5] + signal[6]) % 2
    x4p = (signal[4] + signal[5] + signal[6]) % 2

    x1D = (signal[0] + x1p) % 2
    x2D = (signal[1] + x2p) % 2
    x4D = (signal[3] + x4p) % 2
    S = x1D * 2**0 + x2D * 2**1 + x4D * 2**2
    if S != 0:
        decoded[S - 1] = negate(decoded[S - 1])
    return decoded


size = 80
bits = np.random.randint(0, 2, size=size)
hammingMatrix = np.zeros(shape=(int(size / 4), 7))
for i, step in enumerate(range(0, size, 4)):
    hammingMatrix[i] = hamming(bits[step : step + 4])
    print(f"{step}: Hamming: {hammingMatrix[i]}, Original: {bits[step : step + 4]}")

for i, step in enumerate(range(0, size, 4)):
    print(
        f"{step}: Hamming decoded: {decodeHamming(hammingMatrix[i])}, Original: {hammingMatrix[i]}"
    )
signal = [1, 0, 1, 0, 1, 0, 1]
print(decodeHamming([1, 0, 1, 0, 0, 0, 1]), [1, 0, 1, 0, 1, 0, 1])
