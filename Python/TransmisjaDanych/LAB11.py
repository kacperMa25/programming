import numpy as np


def padding(x, howMany):
    while len(x) < howMany:
        x.insert(0, 0)
    return x


def hamming_15_11(signal):
    k = 11
    pow2Idx = [1, 2, 4, 8]
    messageIdx = [v - 1 for v in range(1, 16) if v not in pow2Idx]
    binaryTable = np.zeros((15, 4))
    for i in range(1, 16):
        binaryTable[i - 1] = padding([int(x) for x in bin(i)[2:]], 4)
    P = np.array(binaryTable[messageIdx][:, ::-1])
    Ik = np.eye(k)
    G = np.hstack((P, Ik))
    print(G)
    tSignal = signal.reshape(signal.shape + (1,))
    print(tSignal.T)
    return (tSignal.T @ G)[0] % 2


signal = np.random.randint(0, 2, size=11)
print(signal)
h = hamming_15_11(signal)
print(h)
