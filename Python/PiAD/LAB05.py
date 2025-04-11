import pandas as pd
import numpy as np
import scipy as sp
from sklearn.datasets import fetch_rcv1


def freq(x, prob=True):
    slownik = {}
    for y in x:
        if y not in slownik:
            slownik[y] = 1.0
        else:
            slownik[y] += 1.0

    ni = list(slownik.values())

    if prob:
        ni = [licz / len(x) for licz in ni]

    return list(slownik.keys()), ni


def freq2(x, y, prob=True):
    slownik = {}
    for x_val, y_val in zip(x, y):
        key = (x_val, y_val)
        if key not in slownik:
            slownik[key] = 1.0
        else:
            slownik[key] += 1.0

    ni = list(slownik.values())

    if prob:
        ni = [licz / len(x) for licz in ni]

    x_unique = [k[0] for k in slownik.keys()]
    y_unique = [k[1] for k in slownik.keys()]

    return list(np.unique(x_unique)), list(np.unique(y_unique)), ni


def entropy(x):
    _, pi = freq(x)
    return -sum(p * np.log2(p) for p in pi if p > 0)


def conditionalEntropy(x, y):
    xi, pi = freq(x)
    cond_H = 0
    for x_val, p_x in zip(xi, pi):
        y_subset = [y[i] for i in range(len(x)) if x[i] == x_val]
        cond_H += p_x * entropy(y_subset)
    return cond_H


def infoGain(x, y):
    return entropy(y) - conditionalEntropy(x, y)


zoo = pd.read_csv("csv/zoo.csv")

X = zoo.iloc[:, :-1].values
y = zoo.iloc[:, -1].values

features = zoo.columns[:-1]
target = zoo.columns[-1]

for col in features:
    ig = infoGain(zoo[col].tolist(), zoo[target].tolist())
    print(f"{col}: IG = {ig:.4f}")


# zadanie 6
rcv1 = fetch_rcv1()
X = rcv1["data"]
Y = rcv1.target[:, 87]
