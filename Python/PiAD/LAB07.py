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
X[:, 1] = 2 * X[:, 0] + 0.5 * np.random.randn(200)

# b) Wizualizacja
plt.scatter(X[:, 0], X[:, 1], color='green')
plt.xlabel("X1")
plt.ylabel("X2")
plt.axis("equal")

# c) PCA
XReduced, eigVecs, eigVals, meanVec = wiPCA(X, 1)

# Rzut punktów na pierwszą składową
first_component = eigVecs[:, 0]
projection = ((X - meanVec) @ first_component[:, None]) * first_component + meanVec

plt.scatter(projection[:, 0], projection[:, 1], color='red')
plt.title("PCA Zadanie 1.")
plt.axis("equal")

# Wizualizacja wektorów własnych
origin = meanVec
vec = eigVecs[:, 0]
length = np.sqrt(eigVals[0]) * 2
plt.quiver(origin[0], origin[1],
            vec[0] * length, vec[1] * length,
            angles='xy', scale_units='xy', scale=1, color='black')

vec = eigVecs[:, 1]
length = np.sqrt(eigVals[1]) * 2
plt.quiver(origin[0], origin[1],
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
X_pca, components, vals, meanVec = wiPCA(X, 2)

pca = PCA(2)
X_pcaOrig = pca.fit_transform(X)

# c) Wizualizacja
plt.figure(figsize=(10, 10))
plt.subplot(2, 1, 1)
plt.scatter(X_pca[:, 0], X_pca[:, 1], c=y, cmap='viridis', edgecolor='k')
plt.title("wiPCA - Zadanie 2.")
plt.xlabel("PC1")
plt.ylabel("PC2")

plt.subplot(2, 1, 2)
plt.scatter(X_pcaOrig[:, 0], X_pcaOrig[:, 1], c=y, cmap='viridis', edgecolor='k')
plt.title("libPCA - Zadanie 2.")
plt.xlabel("PC1")
plt.ylabel("PC2")

plt.savefig("plots/plot_LAB07_02.png")
plt.show()


#zadanie 3
# a) Wczytanie bazy digits
digits = load_digits()
X = digits.data
y = digits.target

# b) PCA
X_pca2, _, _, _ = wiPCA(X, 2)
X_pcaOrig = pca.fit_transform(X)

plt.figure(figsize=(10, 10))
plt.subplot(2, 1, 1)
plt.scatter(X_pca2[:, 0], X_pca2[:, 1], c=y, cmap='tab10', s=10)
plt.title("wiPCA - Zadanie 3.")
plt.xlabel("PC1")
plt.ylabel("PC2")
plt.colorbar()

plt.subplot(2, 1, 2)
plt.scatter(X_pcaOrig[:, 0], X_pcaOrig[:, 1], c=y, cmap='tab10', s=10)
plt.title("libPCA - Zadanie 3.")
plt.xlabel("PC1")
plt.ylabel("PC2")
plt.colorbar()

plt.savefig("plots/plot_LAB07_03.png")
plt.show()

# c) Krzywa wariancji
_, _, eigVals, _ = wiPCA(X, X.shape[1])
explained_variance_ratio = eigVals / np.sum(eigVals)

plt.plot(np.cumsum(explained_variance_ratio))
plt.title("Kumulatywna wariancja (Digits)")
plt.xlabel("Liczba komponentów")
plt.ylabel("Kumulatywna wariancja")
plt.grid()

plt.savefig("plots/plot_LAB07_04.png")
plt.show()

# d) Estymacja błędu
def pca_inverse(X_reduced, components, mean_vec):
    return X_reduced @ components.T + mean_vec

reconstruction_errors = []
components_range = range(1, 65)

for n in components_range:
    X_red, comps, _, mean = wiPCA(X, n)
    X_rec = pca_inverse(X_red, comps[:, :n], mean)
    error = np.mean(np.linalg.norm(X - X_rec, axis=1))
    reconstruction_errors.append(error)

plt.plot(components_range, reconstruction_errors)
plt.title("Średni błąd rekonstrukcji (Digits)")
plt.xlabel("Liczba komponentów")
plt.ylabel("Błąd rekonstrukcji")
plt.grid()
plt.savefig("plots/plot_LAB07_05.png")
plt.show()
