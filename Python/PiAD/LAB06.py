import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA


# Przygotowywanie danych
dataFrame = pd.read_csv("csv/autos.csv")

stringCols = []
numCols = []

for col in dataFrame.columns:
    if pd.api.types.is_numeric_dtype(dataFrame[col]):
        numCols.append(col)
    else:
        stringCols.append(col)

for col in stringCols:
    unique_vals = dataFrame[col].unique()
    mapping = {val: idx for idx, val in enumerate(unique_vals)}
    dataFrame[col] = dataFrame[col].map(mapping)

X = dataFrame.dropna().reset_index(drop=True)

if "Unnamed: 0" in X.columns:
    X.drop("Unnamed: 0", axis=1, inplace=True)

X = X.astype(float)

# zadanie 1
def distp(X, C, e=2):
    n, m = X.shape
    K = C.shape[0]
    D = np.zeros((n, K))
    for i in range(n):
        for k in range(K):
            D[i, k] = np.linalg.norm(X[i] - C[k], ord=e)
    return D

def distm(X, C, V):
    V_inv = np.linalg.inv(V)
    n, m = X.shape
    K = C.shape[0]
    D = np.zeros((n, K))
    for i in range(n):
        for k in range(K):
            diff = X[i] - C[k]
            D[i, k] = np.sqrt(diff.T @ V_inv @ diff)
    return D

# Zadanie 2
def ksrodki(X, k, distance='euclidean', max_iter=100):
    n, m = X.shape
    idx = np.random.choice(n, size=k, replace=False)
    C = X[idx, :].copy()
    P = np.zeros((n, k))

    if distance == 'mahalanobis':
        V = np.cov(X, rowvar=False)
        V += np.eye(m) * 1e-6
    else:
        V = None

    for _ in range(max_iter):
        C_old = C.copy()
        if distance == 'euclidean':
            D = distp(X, C)
        elif distance == 'mahalanobis':
            D = distm(X, C, V)
        else:
            raise ValueError("Unknown distance metric")

        P = np.zeros((n, k))
        for i in range(n):
            min_dist = np.min(D[i])
            min_indices = np.where(D[i] == min_dist)[0]
            chosen_k = np.random.choice(min_indices)
            P[i, chosen_k] = 1

        for j in range(k):
            if P[:, j].sum() > 0:
                C[j] = (P[:, j][:, None] * X).sum(axis=0) / P[:, j].sum()

        if np.allclose(C, C_old):
            break

    return C, P.argmax(axis=1)


# zadanie 3
X_np = X.to_numpy()
C_result, CX_result = ksrodki(X_np, k=3, distance='euclidean')

pca = PCA(n_components=2)
X_pca = pca.fit_transform(X_np)

plt.figure(figsize=(8, 6))
scatter = plt.scatter(X_pca[:, 0], X_pca[:, 1], c=CX_result, cmap='viridis', s=50, alpha=0.7)

C_pca = pca.transform(C_result)
plt.scatter(C_pca[:, 0], C_pca[:, 1], c='red', marker='x', s=200, linewidths=3, label='Centroidy')

plt.legend(handles=scatter.legend_elements()[0], labels=[f'Klaster {i}' for i in range(3)])
plt.xlabel('PCA 1')
plt.ylabel('PCA 2')
plt.title('Wyniki k-średnich (k=3) (euclidean)')
plt.grid(True)
plt.show()

# zadanie 4
def calculate_wcss(X, centroids, labels):
    wcss = 0
    for k in range(len(centroids)):
        cluster_points = X[labels == k]
        if len(cluster_points) > 0:
            distances = np.sum((cluster_points - centroids[k]) ** 2, axis=1)
            wcss += np.sum(distances)
    return wcss

wcss = calculate_wcss(X_np, C_result, CX_result)
print(f"WCSS: {wcss:.4f}")
