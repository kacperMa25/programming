import pandas as pd
import numpy as np
from collections import Counter
from scipy.sparse import issparse
from scipy.sparse import csr_matrix
from sklearn.datasets import fetch_rcv1


# zadanie 1
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


# zadanie 2
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


# zadanie 3
def entropy(x):
    _, pi = freq(x)
    return -sum(p * np.log2(p) for p in pi if p > 0)


def conditionalEntropy(x, y):
    xi, pi = freq(x)
    cond_H = 0
    for x_val, p_x in zip(xi, pi):
        mask = (x == x_val) if not issparse(
            x) else (x.toarray().flatten() == x_val)
        y_subset = y[mask]
        if len(y_subset) > 0:
            cond_H += p_x * entropy(y_subset)
    return cond_H


def infoGain(x, y):
    return entropy(y) - conditionalEntropy(x, y)


# zadanie 4
zoo = pd.read_csv("csv/zoo.csv")

X = zoo.iloc[:, :-1].values
y = zoo.iloc[:, -1].values
features = zoo.columns[:-1]
target = zoo.columns[-1]

ig_scores = []
for col in features:
    ig = infoGain(zoo[col].values, zoo[target].values)
    ig_scores.append((col, ig))

ig_scores = sorted(ig_scores, key=lambda x: x[1], reverse=True)

for col, ig in ig_scores:
    print(f"{col}: IG = {ig:.4f}")


# zadanie 5
def freq(x, prob=True):
    if len(x) == 0:
        return [], []

    if issparse(x):
        x = x.toarray().flatten()

    counts = Counter(x)
    xi = list(counts.keys())
    ni = list(counts.values())

    if prob:
        total = sum(ni)
        ni = [count / total for count in ni]

    return xi, ni


def freq2(x, y, prob=True):
    if len(x) == 0 or len(y) == 0:
        return [], [], []

    if issparse(x):
        x = x.toarray().flatten()
    if issparse(y):
        y = y.toarray().flatten()

    counts = Counter(zip(x, y))
    keys = list(counts.keys())
    ni = list(counts.values())

    if prob:
        total = sum(ni)
        ni = [count / total for count in ni]

    xi = np.unique([k[0] for k in keys]).tolist()
    yi = np.unique([k[1] for k in keys]).tolist()

    return xi, yi, ni


# zadanie 6
rcv1 = fetch_rcv1()
X = rcv1.data
Y = rcv1.target[:, 87]

X_binarized = (X > 0).astype(int)

ig_scores = []
n_features = X_binarized.shape[1]

for i in range(n_features):
    feature = X_binarized[:, i].toarray().flatten()
    ig = infoGain(feature, Y.toarray().flatten())
    ig_scores.append((i, ig))

ig_scores = sorted(ig_scores, key=lambda x: x[1], reverse=True)[:50]

feature_names = rcv1.feature_names if hasattr(rcv1, 'feature_names') else [
    f"word_{i}" for i in range(n_features)]
for idx, ig in ig_scores:
    print(f"Słowo: {feature_names[idx]}, IG = {ig:.4f}")
