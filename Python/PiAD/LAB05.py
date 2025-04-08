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

    for x_values, y_values in zip(x, y):
        if (x_values, y_values) not in slownik:
            slownik[(x_values, y_values)] = 1.0
        else:
            slownik[(x_values, y_values)] += 1.0

    ni = list(slownik.values())

    if prob:
        ni = [licz / len(x) for licz in ni]

    return list(np.unique(x)), list(np.unique(y)), ni


def entropy(x):
    _, pi = freq(x)
    I_X = 0
    for i in range(len(pi)):
        I_X -= pi[i] * np.log2(pi[i])
    return I_X


def conditionalEntropy(x, y):
    pass


def infoGain(x, y):
    return entropy(y) - conditionalEntropy(x, y)


x = ['słońce', 'słońce', 'deszcz', 'deszcz', 'słońce']
y = ['tak', 'tak', 'nie', 'nie', 'tak']

ig = infoGain(x, y)
print(f"Przyrost informacji: {ig:.4f}")
