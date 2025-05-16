import numpy as np
from sklearn.datasets import make_classification, load_iris, make_regression
from sklearn.model_selection import LeaveOneOut, KFold
from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA
from sklearn.neighbors import KDTree
import pandas as pd
import matplotlib.pyplot as plt
import time
from sklearn.metrics import accuracy_score, mean_squared_error

# Funkcja pomocnicza do liczenia odległosci
def euclideanDist(p1, p2):
    p1 = np.asarray(p1)
    p2 = np.asarray(p2)
    return np.sqrt(np.sum((p1 - p2) ** 2))

# Implementacja klasy KNN:
class KNN:
    def __init__(self, n_neighbors=1, use_kdtree=False, task_type='classification'):
        self.__n_neighbors = n_neighbors
        self.__use_kdtree = use_kdtree
        self.__task_type = task_type
        self.__data = None
        self.__classes = None
        self.__kdtree = None

    def fit(self, X, y):
        self.__data = X
        self.__classes = y

        if self.__use_kdtree:
            self.__kdtree = KDTree(X)

    def predict(self, X):
        if self.__data is None or self.__classes is None:
            raise RuntimeError("Model has not been fit yet.")

        n_test_samples = len(X)
        predictions = np.zeros(n_test_samples, dtype=self.__classes.dtype if self.__task_type == 'classification' else float)

        if self.__use_kdtree:
            distances, indices = self.__kdtree.query(X, k=self.__n_neighbors)

            for i in range(n_test_samples):
                neighbor_values = self.__classes[indices[i]]

                if self.__task_type == 'classification':
                    uniq, counts = np.unique(neighbor_values, return_counts=True)
                    predictions[i] = uniq[np.argmax(counts)]
                elif self.__task_type == 'regression':
                    predictions[i] = np.mean(neighbor_values)

        else:
            distances = np.zeros((n_test_samples, len(self.__data)))
            for i in range(n_test_samples):
                for j in range(len(self.__data)):
                    distances[i, j] = euclideanDist(X[i], self.__data[j])

            indices = np.argsort(distances, axis=1)[:, :self.__n_neighbors]

            for i in range(n_test_samples):
                neighbor_values = self.__classes[indices[i]]

                if self.__task_type == 'classification':
                    uniq, counts = np.unique(neighbor_values, return_counts=True)
                    predictions[i] = uniq[np.argmax(counts)]
                elif self.__task_type == 'regression':
                    predictions[i] = np.mean(neighbor_values)

        return predictions

    def score(self, X, y):
        predictions = self.predict(X)

        if self.__task_type == 'classification':
            return accuracy_score(y, predictions)
        elif self.__task_type == 'regression':
            return mean_squared_error(y, predictions)


# Klasyfikacja:
# 1. Generowanie danych uczących
X_clf, y_clf = make_classification(
    n_samples=100,
    n_features=2,
    n_informative=2,
    n_redundant=0,
    n_repeated=0,
    random_state=3
)

# 2. Klasyfikacja
knn_classifier_make = KNN(n_neighbors=5, task_type='classification')
knn_classifier_make.fit(X_clf, y_clf)

# 3. Wizualizacja danych
x_min, x_max = X_clf[:, 0].min() - 1, X_clf[:, 0].max() + 1
y_min, y_max = X_clf[:, 1].min() - 1, X_clf[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.1),
                     np.arange(y_min, y_max, 0.1))

Z = knn_classifier_make.predict(np.c_[xx.ravel(), yy.ravel()])
Z = Z.reshape(xx.shape)

plt.figure(figsize=(8, 6))
plt.contourf(xx, yy, Z, alpha=0.3, cmap='viridis')
plt.scatter(X_clf[:, 0], X_clf[:, 1], c=y_clf, cmap='viridis', marker='o', edgecolors='k')
plt.title("Dane z make_classification oraz granice decyzyjne")
plt.xlabel("Cecha 1")
plt.ylabel("Cecha 2")
plt.colorbar(label="Klasa")
plt.savefig("plots/LAB08_plot1.png")
plt.show()


# 4. Wczytywanie iris
iris = load_iris()
X_iris, y_iris = iris.data, iris.target

scaler = StandardScaler()
X_scaled = scaler.fit_transform(X_iris)

pca = PCA(n_components=2)
X_pca = pca.fit_transform(X_scaled)

# Klasyfikacja
knn_classifier_iris = KNN(n_neighbors=5, task_type='classification')
knn_classifier_iris.fit(X_pca, y_iris)

# 5. Wizualizacja iris oraz granic decyzyjnych
# a)
x_min_pca, x_max_pca = X_pca[:, 0].min() - 1, X_pca[:, 0].max() + 1
y_min_pca, y_max_pca = X_pca[:, 1].min() - 1, X_pca[:, 1].max() + 1
xx_pca, yy_pca = np.meshgrid(np.arange(x_min_pca, x_max_pca, 0.1),
                             np.arange(y_min_pca, y_max_pca, 0.1))

# b)
grid_points_pca = np.c_[xx_pca.ravel(), yy_pca.ravel()]
grid_points_original = scaler.inverse_transform(pca.inverse_transform(grid_points_pca))

# c)
Z_iris = knn_classifier_iris.predict(pca.transform(scaler.transform(grid_points_original)))
Z_iris = Z_iris.reshape(xx_pca.shape)

plt.figure(figsize=(8, 6))
plt.contourf(xx_pca, yy_pca, Z_iris, alpha=0.3, cmap='viridis')
scatter = plt.scatter(X_pca[:, 0], X_pca[:, 1], c=y_iris, cmap='viridis', marker='o', edgecolors='k')
plt.title("Granica decyzyjna KNN (dane Iris po PCA)")
plt.xlabel("Główna składowa 1")
plt.ylabel("Główna składowa 2")
plt.colorbar(scatter, label="Klasa")
plt.savefig("plots/LAB08_plot2.png")
plt.show()

# 6. Kroswalidacja leave-one-out
print("\nLeave-One-Out Cross-Validation dla danych Iris:")
k_values = range(1, 31)
loo_results = {}

for k in k_values:
    knn_loo = KNN(n_neighbors=k, task_type='classification')
    loo = LeaveOneOut()
    scores = []
    for train_index, test_index in loo.split(X_iris):
        X_train, X_test = X_iris[train_index], X_iris[test_index]
        y_train, y_test = y_iris[train_index], y_iris[test_index]
        knn_loo.fit(X_train, y_train)
        score = knn_loo.score(X_test, y_test)
        scores.append(score)
    loo_results[k] = np.mean(scores)

print("k | Średnia dokładność")
print("---|------------------")
for k, accuracy in loo_results.items():
    print(f"{k:<2}| {accuracy:.4f}")

# 7. Porównanie szybkości KNN z werjsą z KD-drzewami oraz bez
print("\nPorównanie czasu działania KNN z/bez KD-Tree:")
X_large, y_large = make_classification(n_samples=10000, n_features=10, random_state=42)

# Bez KD-Drzew
start_time = time.time()
knn_no_kdtree = KNN(n_neighbors=5, use_kdtree=False, task_type='classification')
knn_no_kdtree.fit(X_large, y_large)
knn_no_kdtree.predict(X_large[:100])
end_time = time.time()
time_no_kdtree = end_time - start_time
print(f"Czas działania bez KD-Tree (predykcja na 100 próbkach): {time_no_kdtree:.6f} sekundy")

# Z KD-Drzewami
start_time = time.time()
knn_with_kdtree = KNN(n_neighbors=5, use_kdtree=True, task_type='classification')
knn_with_kdtree.fit(X_large, y_large)
knn_with_kdtree.predict(X_large[:100])
end_time = time.time()
time_with_kdtree = end_time - start_time
print(f"Czas działania z KD-Tree (predykcja na 100 próbkach): {time_with_kdtree:.6f} sekundy")

# Regresja
# 1. Generowanie danych uczących
X_reg, y_reg = make_regression(n_samples=100, n_features=1, n_informative=1, noise=10, random_state=42)

# 2. Regresja
knn_regressor_make = KNN(n_neighbors=5, task_type='regression', use_kdtree=True)
knn_regressor_make.fit(X_reg, y_reg)

y_pred_reg = knn_regressor_make.predict(X_reg)

mse_reg = knn_regressor_make.score(X_reg, y_reg)
print(f"\nBłąd średniokwadratowy (MSE) dla danych make_regression: {mse_reg:.4f}")

# 3. Wizualizacja
plt.figure(figsize=(8, 6))
plt.scatter(X_reg[:, 0], y_reg, color='blue', label='Dane rzeczywiste')
sort_indices = np.argsort(X_reg[:, 0])
plt.plot(X_reg[sort_indices, 0], y_pred_reg[sort_indices], color='red', linewidth=2, label='Odpowiedź modelu KNN')
plt.title("Regresja KNN (dane make_regression)")
plt.xlabel("Cecha")
plt.ylabel("Wartość docelowa")
plt.legend()
plt.savefig("plots/LAB08_plot3.png")
plt.show()

# 4. Wczytanie danych boston
# Alternatywne pobranie danych boston, ze względu na usunięcie funkcji load_boston ze sklearn.
data_url = "http://lib.stat.cmu.edu/datasets/boston"
raw_df = pd.read_csv(data_url, sep="\s+", skiprows=22, header=None)
X_boston = np.hstack([raw_df.values[::2, :], raw_df.values[1::2, :2]])
y_boston = raw_df.values[1::2, 2]

# 5. 10-krotna krzyżowa walidajcja
print("\n10-krotna walidacja krzyżowa dla danych Boston:")
k_values_reg = range(1, 21)
kfold_results = {}

for k in k_values_reg:
    knn_kfold = KNN(n_neighbors=k, task_type='regression')
    kfold = KFold(n_splits=10, shuffle=True, random_state=42)
    scores = []
    for train_index, test_index in kfold.split(X_boston):
        X_train, X_test = X_boston[train_index], X_boston[test_index]
        y_train, y_test = y_boston[train_index], y_boston[test_index]
        knn_kfold.fit(X_train, y_train)
        score = knn_kfold.score(X_test, y_test)
        scores.append(score)
    kfold_results[k] = np.mean(scores)

print("k | Średni błąd średniokwadratowy (MSE)")
print("---|-----------------------------------")
for k, mse in kfold_results.items():
    print(f"{k:<2}| {mse:.4f}")