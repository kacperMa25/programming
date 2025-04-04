import numpy as np
import matplotlib.pyplot as plt

f = 2.0
Tc = 2
phi = 0
fs = 8000


def xt(t, phi, f):
    return np.cos(2 * np.pi * f * t + phi) * np.cos(2.5 * t**0.2 * np.pi)


def yt(t, xt):
    return (xt * t) / (3 + np.cos(20 * np.pi * t))


def zt(t, xt, yt):
    return t**2 * np.abs(xt * yt - 2 / (10 + yt))


def vt(t, xt, yt, zt):
    return zt**3 + 3 * np.sin(zt * yt) * np.abs(yt - xt)


x = np.linspace(0, Tc, num=Tc * fs)
y1 = xt(x, phi, f)
y2 = yt(x, y1)
y3 = zt(x, y1, y2)
y4 = vt(x, y1, y2, y3)

plt.figure()
plt.subplot(221)
plt.plot(x, y1)
plt.subplot(222)
plt.plot(x, y2)
plt.subplot(223)
plt.plot(x, y3)
plt.subplot(224)
plt.plot(x, y4)
plt.show()
