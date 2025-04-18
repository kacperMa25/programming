import numpy as np
import matplotlib.pyplot as plt

B = 10 #ilosc bitow
Tb = 0.0001 #czas trwania pojedynczego bitu
T = B * Tb #czas trwania sygnalu
W = 2 #nie jestem pewny co to
fn = W / Tb #czestotliwosc
fs = fn * 80 #czest próbkowania
t = np.arange(0, T, 1/fs) #

A1 = 2
A2 = 10

fn1 = (W + 1)/Tb
fn2 = (W + 2)/Tb

bits = np.random.randint(0, 2, size=B)
print(bits)

zAt = np.zeros(len(t))
zFt = np.zeros(len(t))
zPt = np.zeros(len(t))

for i, bn in enumerate(bits):
    idx = (t >= i * Tb) & (t < (i + 1) * Tb)

    A = A1 if bn == 0 else A2
    zAt[idx] = A * np.sin(2 * np.pi * fn * t[idx])

    fD = fn1 if bn == 0 else fn2
    zFt[idx] = np.sin(2 * np.pi * fD * t[idx])

    additional = 0 if bn == 0 else np.pi
    zPt[idx] = A * np.sin(2 * np.pi * fn * t[idx] + additional)

plt.figure()
plt.subplot(3,1,1)
plt.plot(t, zAt)
plt.subplot(3,1,2)
plt.plot(t, zFt)
plt.subplot(3,1,3)
plt.plot(t, zPt)

plt.tight_layout()
plt.show()