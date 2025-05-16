import numpy as np
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec

B = 10 #ilosc bitow
Tb = 0.1 #czas trwania pojedynczego bitu
T = B * Tb #czas trwania sygnalu
W = 2 #nie jestem pewny co to
fn = W / Tb #czestotliwosc
fs = fn * 80 #czest próbkowania
t = np.arange(0, T, 1/fs) #

fn1 = (W + 1)/Tb
fn2 = (W + 2)/Tb

bits = np.random.randint(0, 2, size=B)
print(bits)

zFt = np.zeros(len(t))

for i, bn in enumerate(bits):
    idx = (t >= i * Tb) & (t < (i + 1) * Tb)
    fD = fn1 if bn == 0 else fn2
    zFt[idx] = np.sin(2 * np.pi * fD * t[idx])

x1Ft = zFt * np.sin(2 * np.pi * fn1 * t)
x2Ft = zFt * np.sin(2 * np.pi * fn2 * t)

p1Ft = np.zeros(len(t))
p2Ft = np.zeros(len(t))

epsilon = 1e-6
idx = 0
for i in range(len(t)):
    if any(abs(t[i] - k * Tb) < epsilon for k in range(B)):
        idx = i

    p1Ft[i] = np.sum(x1Ft[idx:i]) * 1/fs
    p2Ft[i] = np.sum(x2Ft[idx:i]) * 1/fs

pFt = p2Ft - p1Ft

def checkTreshold(signal, threshold):
    ct = np.zeros(len(t))
    for i in range(B):
        idx = (i * Tb <= t) & ((i + 1) * Tb > t)
        ct[idx] = 0 if np.sum(signal[idx]) < threshold else 1
    return ct

def shrinkBitSignal(signal):
    shrinked = np.zeros(B)
    for i in range(B):
        idx = (i * Tb <= t) & ((i + 1) * Tb > t)
        shrinked[i] = signal[idx][0]
    return shrinked

cFt = checkTreshold(pFt, 0)

plt.figure(figsize=(15, 8))
gs = GridSpec(5, 2, width_ratios=[1, 1])

ax1 = plt.subplot(gs[0, :])
ax1.plot(t, zFt)

ax2_left = plt.subplot(gs[1, 0])
ax2_left.plot(t, x1Ft)
ax2_right = plt.subplot(gs[1, 1])
ax2_right.plot(t, x2Ft)

ax3_left = plt.subplot(gs[2, 0])
ax3_left.plot(t, p1Ft)
ax3_right = plt.subplot(gs[2, 1])
ax3_right.plot(t, p2Ft)

ax4 = plt.subplot(gs[3, :])
ax4.plot(t, pFt)

ax5 = plt.subplot(gs[4, :])
ax5.plot(t, cFt)

plt.tight_layout()
plt.show()

demBits = shrinkBitSignal(cFt)
plt.figure()
plt.subplot(2, 1, 1)
plt.title("Original")
plt.stem(np.arange(B), bits)
plt.subplot(2, 1, 2)
plt.title("Demodulated")
plt.stem(np.arange(B), demBits)
plt.tight_layout()
plt.show()
