import numpy as np
import scipy.sparse as sp
import pandas as pd


def freq(x, prob=True):
    slownik = {}

    for y in x:
        if y not in slownik:
            slownik[y] = 1.0
        else:
            slownik[y] += 1

    ni = list(slownik.values())

    if prob:
        ni = [licz / len(x) for licz in ni]

    return list(slownik.keys()), ni


def freq2(x, y, prob=True):
    slownik = {}

    for x_values, y_values in x, y:
        if (x_values, y_values) not in slownik:
            slownik[(x_values, y_values)] = 1.0
        else:
            slownik[(x_values, y_values)] += 1.0

    ni = list(slownik.values())

    if prob:
        ni = [licz / len(x) for licz in ni]

    return np.unique(x), np.unique(y), ni


def entropy(x):
    _, ni = freq(x, prob=True)
    I_X = 0
    for i in range(len(ni)):
        I_X -= ni[i] * np.log2(ni[i])
    return I_X


print(entropy([1, 1, 1, 1, 1, 1, 1, 1, 3, 2, 2, 2, 2, 2, 2, 3, 3, 4]))
