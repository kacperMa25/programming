import numpy as np
import matplotlib.pyplot as plt

Tb = 0.1
B = 10
T = Tb * B
W = 2
fn = W / Tb
fs = 100 * fn
t = np.arange(0, T, 1/fs)

A1 = 10
A2 = 20

bits = np.random.randint(2, size=B)
print(bits)

zAt = np.zeros(len(t))
zPt = np.zeros(len(t))

for i, bit in enumerate(bits):
    idx = (i * Tb <= t) & ((i + 1) * Tb > t)
    A = A1 if bit == 0 else A2
    zAt[idx] = A * np.sin(2 * np.pi * fn * t[idx])

    phaze = 0 if bit == 0 else np.pi
    zPt[idx] = np.sin(2 * np.pi * fn * t[idx] + phaze)


plt.figure(figsize=(12, 10))

plt.subplot(4, 2, 1)
plt.plot(t, zAt)
plt.xlabel("Time [s]")
plt.ylabel("Amplitude")

plt.subplot(4, 2, 2)
plt.plot(t, zPt)
plt.xlabel("Time [s]")
plt.ylabel("Amplitude")

tempSigA = np.sin(2 * np.pi * fn * t)
tempSigP = np.sin(2 * np.pi * fn * t + np.pi)

xAt = zAt * tempSigA
xPt = zPt * tempSigP

plt.subplot(4, 2, 3)
plt.plot(t, xAt)
plt.xlabel("Time [s]")
plt.ylabel("Amplitude")

plt.subplot(4, 2, 4)
plt.plot(t, xPt)
plt.xlabel("Time [s]")
plt.ylabel("Amplitude")

pAt = np.zeros(len(t))
pPt = np.zeros(len(t))

epsilon = 1e-6
idx = 0
for i in range(len(t)):
    if any(abs(t[i] - k * Tb) < epsilon for k in range(B)):
        idx = i

    pAt[i] = np.sum(xAt[idx:i]) * 1/fs
    pPt[i] = np.sum(xPt[idx:i]) * 1/fs

plt.subplot(4, 2, 5)
plt.plot(t, pAt)
plt.xlabel("Time [s]")
plt.ylabel("Amplitude")

plt.subplot(4, 2, 6)
plt.plot(t, pPt)
plt.xlabel("Time [s]")
plt.ylabel("Amplitude")

def checkTreshold(signal, threshold):
    ct = np.zeros(len(t))
    for i in range(B):
        idx = (i * Tb <= t) & ((i + 1) * Tb > t)
        ct[idx] = 0 if np.sum(signal[idx]) < threshold else 1
    return ct

plt.subplot(4, 2, 7)
plt.plot(t, checkTreshold(pAt, 50))


plt.subplot(4, 2, 8)
plt.plot(t, checkTreshold(pPt, 1e-5))
plt.xlabel("Time [s]")

plt.tight_layout()
plt.show()
