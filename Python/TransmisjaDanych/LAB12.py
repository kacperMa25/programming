import numpy as np
import matplotlib.pyplot as plt


Tb = 0.0001
B = 10
T = B * Tb
W = 2
fn = W / Tb
fs = fn * 80
t = np.arange(0, T, 1 / fs)

A1 = 2
A2 = 10

fn1 = (W + 1) / Tb
fn2 = (W + 2) / Tb


def checkTreshold(signal, threshold):
    ct = np.zeros(len(t))
    for i in range(B):
        idx = (i * Tb <= t) & ((i + 1) * Tb > t)
        ct[idx] = 0 if np.sum(signal[idx]) < threshold else 1
    return ct


def generate():
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

    return zAt, zFt, zPt


def demodulate(zAt, zFt, zPt):
    B = 10
    Tb = 0.0001
    T = B * Tb
    W = 2
    fn = W / Tb
    fs = fn * 80
    t = np.arange(0, T, 1 / fs)

    fn1 = (W + 1) / Tb
    fn2 = (W + 2) / Tb

    bits = np.random.randint(0, 2, size=B)
    print(bits)
    tempSigA = np.sin(2 * np.pi * fn * t)
    tempSigP = np.sin(2 * np.pi * fn * t + np.pi)

    x1Ft = zFt * np.sin(2 * np.pi * fn1 * t)
    x2Ft = zFt * np.sin(2 * np.pi * fn2 * t)

    p1Ft = np.zeros(len(t))
    p2Ft = np.zeros(len(t))

    xAt = zAt * tempSigA
    xPt = zPt * tempSigP

    pAt = np.zeros(len(t))
    pPt = np.zeros(len(t))

    epsilon = 1e-6
    idx = 0
    for i in range(len(t)):
        if any(abs(t[i] - k * Tb) < epsilon for k in range(B)):
            idx = i

        pAt[i] = np.sum(xAt[idx:i]) * 1 / fs
        pPt[i] = np.sum(xPt[idx:i]) * 1 / fs
        p1Ft[i] = np.sum(x1Ft[idx:i]) * 1 / fs
        p2Ft[i] = np.sum(x2Ft[idx:i]) * 1 / fs

    pFt = p2Ft - p1Ft

    cAt = checkTreshold(pAt, 50)
    cFt = checkTreshold(pFt, 0)
    cPt = checkTreshold(pPt, 1e-5)

    return cAt, cFt, cPt
