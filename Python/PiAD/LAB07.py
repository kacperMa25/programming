import numpy as np
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.datasets import load_iris
from sklearn.datasets import load_digits

def wiPCA(X, dim):
    mean_vec = np.mean(X, axis=0)
    X_centered = X - mean_vec
    covMat = np.cov(X_centered.T)
    eigenVals, eigenVecs = np.linalg.eigh(covMat)
    idx = np.argsort(eigenVals)[::-1]
    eigenVals = eigenVals[idx]
    eigenVecs = eigenVecs[:, idx]
    components = eigenVecs[:, :dim]

    return X_centered @ components, eigenVecs, eigenVals, mean_vec


# zadanie 1
# a) Generowanie macierzy losowej
X = np.random.randn(200, 2)
X[:, 1] = 0.75 * X[:, 0] + np.random.randn(200)

# b) Wizualizacja losowych punktów
plt.scatter(X[:, 0], X[:, 1], color='green')
plt.xlabel("X1")
plt.ylabel("X2")
plt.axis("equal")

# c) PCA
XReduced, eigVecs, eigVals, meanVec = wiPCA(X, 1)

# Rzut punktów na pierwszą składową
firstComponent = eigVecs[:, 0]
projection = ((X - meanVec) @ firstComponent[:, None]) * firstComponent + meanVec

# Wizualicja rzutowania
plt.scatter(projection[:, 0], projection[:, 1], color='red')
plt.title("PCA Zadanie 1.")
plt.axis("equal")

# Wizualizacja wektorów własnych
pos00 = meanVec
vec = eigVecs[:, 0]
length = np.sqrt(eigVals[0]) * 2
plt.quiver(pos00[0], pos00[1],
            vec[0] * length, vec[1] * length,
            angles='xy', scale_units='xy', scale=1, color='black')

vec = eigVecs[:, 1]
length = np.sqrt(eigVals[1]) * 2
plt.quiver(pos00[0], pos00[1],
            vec[0] * length, vec[1] * length,
            angles='xy', scale_units='xy', scale=1, color='black')

plt.savefig("plots/plot_LAB07_01.png")
plt.show()

# zadanie 2

# a) Wczytanie
iris = load_iris()
X = iris.data
y = iris.target

# b) PCA
Xpca, dims, vals, meanVec = wiPCA(X, 2)

pca = PCA(2)
XpcaOrig = pca.fit_transform(X)

# c) Wizualizacja
plt.figure(figsize=(10, 8))
plt.subplot(2, 1, 1)
plt.scatter(Xpca[:, 0], Xpca[:, 1], c=y, cmap='viridis')
plt.title("wiPCA - Zadanie 2.")
plt.xlabel("PC1")
plt.ylabel("PC2")

plt.subplot(2, 1, 2)
plt.scatter(XpcaOrig[:, 0], XpcaOrig[:, 1], c=y, cmap='viridis')
plt.title("libPCA - Zadanie 2.")
plt.xlabel("PC1")
plt.ylabel("PC2")

plt.tight_layout()
plt.savefig("plots/plot_LAB07_02.png")
plt.show()


#zadanie 3
# a) Wczytanie bazy digits
digits = load_digits()
X = digits.data
y = digits.target

# b) PCA
Xpca, _, _, _ = wiPCA(X, 2)
XpcaOrig = pca.fit_transform(X)


# c) Krzywa wariancji
_, _, eigVals, _ = wiPCA(X, X.shape[1])
explainedVarianceRatio = eigVals / np.sum(eigVals)

plt.plot(np.cumsum(explainedVarianceRatio))
plt.title("Kumulatywna wariancja (Digits)")
plt.xlabel("Liczba komponentów")
plt.ylabel("Kumulatywna wariancja")
plt.grid()

plt.savefig("plots/plot_LAB07_04.png")
plt.show()

# d)
plt.figure(figsize=(10, 8))
plt.subplot(2, 1, 1)
plt.scatter(Xpca[:, 0], Xpca[:, 1], c=y, cmap='tab10')
plt.title("wiPCA - Zadanie 3.")
plt.xlabel("PC1")
plt.ylabel("PC2")
plt.colorbar()

plt.subplot(2, 1, 2)
plt.scatter(XpcaOrig[:, 0], XpcaOrig[:, 1], c=y, cmap='tab10')
plt.title("libPCA - Zadanie 3.")
plt.xlabel("PC1")
plt.ylabel("PC2")
plt.colorbar()

plt.tight_layout()
plt.savefig("plots/plot_LAB07_03.png")
plt.show()

# e) Estymacja błędu
def invPCA(XReduced, components, meanVec):
    return XReduced @ components.T + meanVec

errors = []
dims = range(1, 65)

for dim in dims:
    X_red, comps, _, mean = wiPCA(X, dim)
    X_rec = invPCA(X_red, comps[:, :dim], mean)
    error = np.mean(np.linalg.norm(X - X_rec, axis=1))
    errors.append(error)

plt.plot(dims, errors)
plt.title("Błąd rekonstrukcji")
plt.xlabel("Składowe")
plt.ylabel("Błąd")
plt.grid()
plt.savefig("plots/plot_LAB07_05.png")
plt.show()
