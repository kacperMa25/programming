import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import time
from sklearn.datasets import make_classification
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.naive_bayes import GaussianNB
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis
from sklearn.neighbors import KNeighborsClassifier
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import (
    accuracy_score,
    recall_score,
    precision_score,
    f1_score,
    roc_auc_score,
    roc_curve,
)
from sklearn.preprocessing import StandardScaler

np.random.seed(42)

# --- Zadanie 1: Badanie klasyfikatorów ---

# Krok 1: Wygeneruj syntetyczne dane
X_task1, y_task1 = make_classification(
    n_samples=1000,
    n_features=2,
    n_classes=2,
    n_clusters_per_class=2,
    n_redundant=0,
    random_state=42,
)

# Krok 2: Wizualizuj dane (Rysunek 1)
plt.figure(figsize=(8, 6))
plt.scatter(
    X_task1[y_task1 == 0][:, 0],
    X_task1[y_task1 == 0][:, 1],
    color="blue",
    label="Klasa 0",
    alpha=0.6,
)
plt.scatter(
    X_task1[y_task1 == 1][:, 0],
    X_task1[y_task1 == 1][:, 1],
    color="orange",
    label="Klasa 1",
    alpha=0.6,
)
plt.title("Wizualizacja syntetycznych danych (2 klasy)")
plt.xlabel("Cecha 1")
plt.ylabel("Cecha 2")
plt.legend()
plt.show()

# Krok 3: Zdefiniuj klasyfikatory
classifiers = {
    "GaussianNB": GaussianNB(),
    "QuadraticDiscriminantAnalysis": QuadraticDiscriminantAnalysis(),
    "KNeighborsClassifier": KNeighborsClassifier(),
    "SVC": SVC(probability=True),
    "DecisionTreeClassifier": DecisionTreeClassifier(),
}

# Krok 4: Oceń klasyfikatory w 100 podziałach danych
n_splits = 100
results_task1 = []

for name, clf in classifiers.items():
    for i in range(n_splits):
        # Podziel dane na zbiór uczący i testowy
        X_train, X_test, y_train, y_test = train_test_split(
            X_task1, y_task1, test_size=0.3, random_state=i
        )

        # Skaluj cechy
        scaler = StandardScaler()
        X_train_scaled = scaler.fit_transform(X_train)
        X_test_scaled = scaler.transform(X_test)

        # Czas uczenia
        start_train = time.time()
        clf.fit(X_train_scaled, y_train)
        train_time = time.time() - start_train

        # Czas testowania
        start_test = time.time()
        y_pred = clf.predict(X_test_scaled)
        test_time = time.time() - start_test

        # Oblicz metryki
        accuracy = accuracy_score(y_test, y_pred)
        recall = recall_score(y_test, y_pred)
        precision = precision_score(y_test, y_pred)
        f1 = f1_score(y_test, y_pred)
        roc_auc = roc_auc_score(y_test, clf.predict_proba(X_test_scaled)[:, 1])

        results_task1.append(
            {
                "Classifier": name,
                "Split": i,
                "Train_Time": train_time,
                "Test_Time": test_time,
                "Accuracy": accuracy,
                "Recall": recall,
                "Precision": precision,
                "F1": f1,
                "ROC_AUC": roc_auc,
            }
        )

        if i == n_splits - 1:
            last_X_test_task1 = X_test_scaled
            last_y_test_task1 = y_test
            last_y_pred_task1 = y_pred
            last_X_test_unscaled_task1 = X_test
            last_clf_task1 = clf
            last_scaler_task1 = scaler

# Krok 5: Agreguj wyniki
results_task1_df = pd.DataFrame(results_task1)
avg_results_task1 = results_task1_df.groupby("Classifier").mean().drop(columns="Split")
print("\nZadanie 1: Średnie wyniki dla każdego klasyfikatora:")
print(avg_results_task1)

metrics = [
    "Train_Time",
    "Test_Time",
    "Accuracy",
    "Recall",
    "Precision",
    "F1",
    "ROC_AUC",
]
for metric in metrics:
    plt.figure(figsize=(8, 6))
    avg_results_task1[metric].plot(kind="bar", color="skyblue")
    plt.title(f"Średnia {metric} dla każdego klasyfikatora")
    plt.ylabel(metric)
    plt.xlabel("Klasyfikator")
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.show()

# Krok 6: Wizualizuj błędy klasyfikacji dla ostatniej iteracji (Rysunek 3)
correct = last_y_test_task1 == last_y_pred_task1
incorrect = ~correct
plt.figure(figsize=(8, 6))
plt.scatter(
    last_X_test_unscaled_task1[correct, 0],
    last_X_test_unscaled_task1[correct, 1],
    color="green",
    label="Poprawne",
    alpha=0.6,
)
plt.scatter(
    last_X_test_unscaled_task1[incorrect, 0],
    last_X_test_unscaled_task1[incorrect, 1],
    color="red",
    label="Błędne",
    alpha=0.6,
)
plt.title(f"Błędy klasyfikacji (Ostatnia iteracja, {list(classifiers.keys())[-1]})")
plt.xlabel("Cecha 1")
plt.ylabel("Cecha 2")
plt.legend()
plt.show()

# Krok 7: Narysuj krzywą ROC dla ostatniej iteracji (Rysunek 4)
fpr, tpr, _ = roc_curve(
    last_y_test_task1, last_clf_task1.predict_proba(last_X_test_task1)[:, 1]
)
roc_auc = roc_auc_score(
    last_y_test_task1, last_clf_task1.predict_proba(last_X_test_task1)[:, 1]
)
plt.figure(figsize=(8, 6))
plt.plot(fpr, tpr, label=f"Krzywa ROC (AUC = {roc_auc:.2f})")
plt.plot([0, 1], [0, 1], linestyle="--", color="gray", label="Losowe (FPR = TPR)")
plt.title(f"Krzywa ROC (Ostatnia iteracja, {list(classifiers.keys())[-1]})")
plt.xlabel("Wskaźnik fałszywie pozytywnych")
plt.ylabel("Wskaźnik prawdziwie pozytywnych")
plt.legend()
plt.show()

# Krok 8: Narysuj granicę decyzyjną dla ostatniej iteracji (Rysunek 5)
h = 0.02
x_min, x_max = X_task1[:, 0].min() - 1, X_task1[:, 0].max() + 1
y_min, y_max = X_task1[:, 1].min() - 1, X_task1[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))

# Skaluj punkty siatki
mesh_points = np.c_[xx.ravel(), yy.ravel()]
mesh_points_scaled = last_scaler_task1.transform(mesh_points)
Z = last_clf_task1.predict(mesh_points_scaled)
Z = Z.reshape(xx.shape)

plt.figure(figsize=(8, 6))
plt.contourf(xx, yy, Z, cmap=plt.cm.coolwarm, alpha=0.3)
plt.scatter(
    last_X_test_unscaled_task1[:, 0],
    last_X_test_unscaled_task1[:, 1],
    c=last_y_test_task1,
    cmap=plt.cm.coolwarm,
    edgecolors="k",
)
plt.title(f"Granica decyzyjna (Ostatnia iteracja, {list(classifiers.keys())[-1]})")
plt.xlabel("Cecha 1")
plt.ylabel("Cecha 2")
plt.show()

# --- Zadanie 2: Badanie parametrów wybranego klasyfikatora ---

# Krok 1: Wygeneruj nowe dane
X_task2, y_task2 = make_classification(
    n_samples=200,
    n_features=2,
    n_classes=2,
    n_clusters_per_class=2,
    n_redundant=0,
    random_state=42,
)

# Krok 2: Wybierz klasyfikator (KNeighborsClassifier)
clf_task2 = KNeighborsClassifier()

# Krok 3: Zdefiniuj przestrzeń parametrów
param_grid = {
    "n_neighbors": [3, 5, 7, 9, 11],
    "p": [1, 2, 3],  # p=1 (Manhattan), p=2 (Euklidesowa), p=3 (Minkowski)
}

# Krok 4: Dopasowanie hiperparametrów za pomocą GridSearchCV
scaler_task2 = StandardScaler()
X_task2_scaled = scaler_task2.fit_transform(X_task2)
grid_search = GridSearchCV(clf_task2, param_grid, cv=5, scoring="roc_auc", n_jobs=-1)
grid_search.fit(X_task2_scaled, y_task2)

print("\nNajlepsze parametry z GridSearchCV:")
print(grid_search.best_params_)

results_task2_grid = pd.DataFrame(grid_search.cv_results_)
param_results = results_task2_grid[["param_n_neighbors", "param_p", "mean_test_score"]]

# Krok 5: Wizualizuj wpływ parametrów (Rysunki 6 i 7)
# Rysunek 6: Wizualizacja 1D (wpływ n_neighbors dla stałego p=2)
p_fixed = 2
subset = param_results[param_results["param_p"] == p_fixed]
plt.figure(figsize=(8, 6))
plt.plot(subset["param_n_neighbors"], subset["mean_test_score"], marker="o")
plt.title(f"Wpływ n_neighbors na ROC AUC (p={p_fixed})")
plt.xlabel("n_neighbors")
plt.ylabel("Średni ROC AUC")
plt.grid(True)
plt.show()

# Rysunek 7: Wizualizacja 2D (mapa ciepła n_neighbors vs. p)
pivot_table = param_results.pivot(
    index="param_n_neighbors", columns="param_p", values="mean_test_score"
)
plt.figure(figsize=(8, 6))
sns.heatmap(
    pivot_table,
    annot=True,
    fmt=".2f",
    cmap="viridis",
    cbar_kws={"label": "Średni ROC AUC"},
)
plt.title("Wpływ n_neighbors i p na ROC AUC")
plt.xlabel("p")
plt.ylabel("n_neighbors")
plt.show()

# Krok 6: Oceń zoptymalizowany klasyfikator w 100 podziałach
n_splits = 100
results_task2 = []
best_clf_task2 = grid_search.best_estimator_

for i in range(n_splits):
    # Podziel dane
    X_train, X_test, y_train, y_test = train_test_split(
        X_task2, y_task2, test_size=0.3, random_state=i
    )

    # Skaluj cechy
    X_train_scaled = scaler_task2.fit_transform(X_train)
    X_test_scaled = scaler_task2.transform(X_test)

    # Czas uczenia
    start_train = time.time()
    best_clf_task2.fit(X_train_scaled, y_train)
    train_time = time.time() - start_train

    # Czas testowania
    start_test = time.time()
    y_pred = best_clf_task2.predict(X_test_scaled)
    test_time = time.time() - start_test

    # Oblicz metryki
    accuracy = accuracy_score(y_test, y_pred)
    recall = recall_score(y_test, y_pred)
    precision = precision_score(y_test, y_pred)
    f1 = f1_score(y_test, y_pred)
    roc_auc = roc_auc_score(y_test, best_clf_task2.predict_proba(X_test_scaled)[:, 1])

    results_task2.append(
        {
            "Split": i,
            "Train_Time": train_time,
            "Test_Time": test_time,
            "Accuracy": accuracy,
            "Recall": recall,
            "Precision": precision,
            "F1": f1,
            "ROC_AUC": roc_auc,
        }
    )

    if i == n_splits - 1:
        last_X_test_task2 = X_test_scaled
        last_y_test_task2 = y_test
        last_y_pred_task2 = y_pred
        last_X_test_unscaled_task2 = X_test
        last_scaler_task2 = scaler_task2

# Krok 7: Agreguj wyniki
results_task2_df = pd.DataFrame(results_task2)
avg_results_task2 = results_task2_df.mean().drop("Split")
print("\nZadanie 2: Średnie wyniki dla zoptymalizowanego KNeighborsClassifier:")
print(avg_results_task2)

# Krok 8: Narysuj krzywą ROC dla ostatniej iteracji
fpr, tpr, _ = roc_curve(
    last_y_test_task2, best_clf_task2.predict_proba(last_X_test_task2)[:, 1]
)
roc_auc = roc_auc_score(
    last_y_test_task2, best_clf_task2.predict_proba(last_X_test_task2)[:, 1]
)
plt.figure(figsize=(8, 6))
plt.plot(fpr, tpr, label=f"Krzywa ROC (AUC = {roc_auc:.2f})")
plt.plot([0, 1], [0, 1], linestyle="--", color="gray", label="Losowe (FPR = TPR)")
plt.title("Krzywa ROC (Ostatnia iteracja, zoptymalizowany KNeighborsClassifier)")
plt.xlabel("Wskaźnik fałszywie pozytywnych")
plt.ylabel("Wskaźnik prawdziwie pozytywnych")
plt.legend()
plt.show()

# Krok 9: Narysuj granicę decyzyjną dla ostatniej iteracji
h = 0.02
x_min, x_max = X_task2[:, 0].min() - 1, X_task2[:, 0].max() + 1
y_min, y_max = X_task2[:, 1].min() - 1, X_task2[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))

mesh_points = np.c_[xx.ravel(), yy.ravel()]
mesh_points_scaled = last_scaler_task2.transform(mesh_points)
Z = best_clf_task2.predict(mesh_points_scaled)
Z = Z.reshape(xx.shape)

plt.figure(figsize=(8, 6))
plt.contourf(xx, yy, Z, cmap=plt.cm.coolwarm, alpha=0.3)
plt.scatter(
    last_X_test_unscaled_task2[:, 0],
    last_X_test_unscaled_task2[:, 1],
    c=last_y_test_task2,
    cmap=plt.cm.coolwarm,
    edgecolors="k",
)
plt.title("Granica decyzyjna (Ostatnia iteracja, zoptymalizowany KNeighborsClassifier)")
plt.xlabel("Cecha 1")
plt.ylabel("Cecha 2")
plt.show()
