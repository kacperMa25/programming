import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from sklearn.datasets import make_classification
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.linear_model import LogisticRegression, Perceptron
from sklearn.multiclass import OneVsOneClassifier, OneVsRestClassifier
from sklearn.metrics import (
    accuracy_score,
    precision_score,
    recall_score,
    f1_score,
    roc_curve,
    auc,
)

# 1. Generowanie zestawu danych
X, y = make_classification(
    n_samples=2000,
    n_classes=4,
    n_features=2,
    n_informative=2,
    n_redundant=0,
    n_clusters_per_class=1,
    random_state=42,
)

# 2. Podział danych na 50/50
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.5, random_state=42
)

# 3. Definiowanie klasyfikatorów
classifiers = [
    ("SVC_linear", SVC(kernel="linear", probability=True)),
    ("SVC_rbf", SVC(kernel="rbf", probability=True)),
    ("LogisticRegression", LogisticRegression(max_iter=1000)),
    ("Perceptron", Perceptron()),
]

# Opakowanie klasyfikatorów w strategie OvO i OvR
ovo_classifiers = [
    (f"{name}_OvO", OneVsOneClassifier(clf)) for name, clf in classifiers
]
ovr_classifiers = [
    (f"{name}_OvR", OneVsRestClassifier(clf)) for name, clf in classifiers
]
all_classifiers = ovo_classifiers + ovr_classifiers

# 4. Przechowywanie metryk i siatka dla granic decyzyjnych
results = {name: {} for name, _ in all_classifiers}
h = 0.02  # Rozmiar kroku w siatce
x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))

# Kolory do wizualizacji
colors = ["red", "blue", "green", "purple"]
cmap = ListedColormap(colors[:4])

# 5. Trenowanie, predykcja i wizualizacja dla każdego klasyfikatora
for name, clf in all_classifiers:
    # Trenowanie klasyfikatora
    clf.fit(X_train, y_train)
    y_pred = clf.predict(X_test)

    # Obliczanie metryk
    results[name]["accuracy"] = accuracy_score(y_test, y_pred)
    results[name]["precision"] = precision_score(y_test, y_pred, average="weighted")
    results[name]["recall"] = recall_score(y_test, y_pred, average="weighted")
    results[name]["f1"] = f1_score(y_test, y_pred, average="weighted")

    # Wizualizacja poprawnych i niepoprawnych predykcji
    plt.figure(figsize=(8, 6))
    correct = y_test == y_pred
    plt.scatter(
        X_test[correct, 0],
        X_test[correct, 1],
        c=[colors[i] for i in y_test[correct]],
        label="Poprawne",
        alpha=0.6,
    )
    plt.scatter(
        X_test[~correct, 0],
        X_test[~correct, 1],
        c="black",
        marker="x",
        label="Niepoprawne",
        alpha=0.6,
    )
    plt.title(f"Poprawne vs Niepoprawne Predykcje: {name}")
    plt.legend()
    plt.savefig(f"poprawne_niepoprawne_{name}.png")
    plt.close()

    # Krzywa ROC (średnia dla wieloklasowej)
    y_score = clf.predict_proba(X_test) if hasattr(clf, "predict_proba") else None
    if y_score is not None:
        plt.figure(figsize=(8, 6))
        mean_fpr = np.linspace(0, 1, 100)
        mean_tpr = 0.0
        n_classes = len(np.unique(y))
        for i in range(n_classes):
            fpr, tpr, _ = roc_curve(y_test == i, y_score[:, i])
            roc_auc = auc(fpr, tpr)
            results[name][f"roc_auc_class_{i}"] = roc_auc
            plt.plot(fpr, tpr, label=f"Klasa {i} (AUC = {roc_auc:.2f})")
            mean_tpr += np.interp(mean_fpr, fpr, tpr)
        mean_tpr /= n_classes
        results[name]["roc_auc_mean"] = auc(mean_fpr, mean_tpr)
        plt.plot(
            mean_fpr,
            mean_tpr,
            "k--",
            label=f"Średnia ROC (AUC = {results[name]['roc_auc_mean']:.2f})",
        )
        plt.xlabel("Wskaźnik Fałszywie Pozytywnych")
        plt.ylabel("Wskaźnik Prawdziwie Pozytywnych")
        plt.title(f"Krzywe ROC: {name}")
        plt.legend()
        plt.savefig(f"krzywa_roc_{name}.png")
        plt.close()

    # Granica decyzyjna
    Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    plt.figure(figsize=(8, 6))
    plt.contourf(xx, yy, Z, cmap=cmap, alpha=0.3)
    plt.scatter(
        X_test[:, 0],
        X_test[:, 1],
        c=[colors[i] for i in y_test],
        edgecolors="k",
        cmap=cmap,
    )
    plt.title(f"Granica Decyzyjna: {name}")
    plt.savefig(f"granica_decyzyjna_{name}.png")
    plt.close()

# 6. Wizualizacja jakości klasyfikacji (wykres słupkowy)
plt.figure(figsize=(12, 6))
bar_width = 0.2
index = np.arange(len(all_classifiers))
plt.bar(
    index,
    [results[name]["accuracy"] for name, _ in all_classifiers],
    bar_width,
    label="Dokładność",
)
plt.bar(
    index + bar_width,
    [results[name]["precision"] for name, _ in all_classifiers],
    bar_width,
    label="Precyzja",
)
plt.bar(
    index + 2 * bar_width,
    [results[name]["recall"] for name, _ in all_classifiers],
    bar_width,
    label="Czułość",
)
plt.bar(
    index + 3 * bar_width,
    [results[name]["f1"] for name, _ in all_classifiers],
    bar_width,
    label="F1 Score",
)
plt.xlabel("Klasyfikator")
plt.ylabel("Wynik")
plt.title("Porównanie Wyników Klasyfikatorów")
plt.xticks(index + 1.5 * bar_width, [name for name, _ in all_classifiers], rotation=45)
plt.legend()
plt.tight_layout()
plt.savefig("porownanie_wynikow.png")
plt.close()

# 7. Wyświetlenie wyników dla sprawozdania
print("Wyniki Klasyfikacji:")
for name, _ in all_classifiers:
    print(f"\nKlasyfikator: {name}")
    print(f"Dokładność: {results[name]['accuracy']:.3f}")
    print(f"Precyzja: {results[name]['precision']:.3f}")
    print(f"Czułość: {results[name]['recall']:.3f}")
    print(f"F1 Score: {results[name]['f1']:.3f}")
    if "roc_auc_mean" in results[name]:
        print(f"Średnie AUC: {results[name]['roc_auc_mean']:.3f}")
        for i in range(4):
            if f"roc_auc_class_{i}" in results[name]:
                print(f"Klasa {i} AUC: {results[name][f'roc_auc_class_{i}']:.3f}")

# 8. Podsumowanie sprawozdania
print("\nPodsumowanie Sprawozdania:")
print("Wpływ Strategii OvO vs OvR:")
print(
    "- OvO trenuje klasyfikator dla każdej pary klas (6 dla 4 klas), co pozwala na uchwycenie złożonych granic, ale jest kosztowne obliczeniowo."
)
print(
    "- OvR trenuje jeden klasyfikator na klasę przeciwko wszystkim pozostałym (4 dla 4 klas), jest prostsze, ale wrażliwe na niezbalansowane klasy."
)
print(
    "Granice Liniowe: SVC_linear i LogisticRegression generują liniowe granice decyzyjne; SVC_rbf tworzy nieliniowe granice; Perceptron generuje liniowe, ale mniej niezawodne granice."
)
print("Wpływ Rozkładu Danych:")
print(
    "- Klasy z AUC > 0.95 są dobrze oddzielone, co zmniejsza nakładanie się i poprawia wyniki."
)
print(
    "- Klasy z AUC < 0.9 prawdopodobnie się nakładają, obniżając metryki takie jak precyzja i czułość, szczególnie dla OvR."
)
print("Wydajność Klasyfikatorów:")
print(
    "- SVC_rbf często przewyższa inne w danych nieliniowych dzięki elastyczności jądra."
)
print(
    "- LogisticRegression i SVC_linear działają dobrze dla danych liniowo rozdzielnych."
)
print(
    "- Perceptron może działać słabiej z powodu wrażliwości na szum i braku estymacji prawdopodobieństwa."
)
print("Rekomendacje:")
print("- Używaj OvO z SVC_rbf dla złożonych, nieliniowych danych.")
print("- Używaj OvR z LogisticRegression dla efektywnej klasyfikacji liniowej.")
print(
    "- Analizuj rozrzut danych, aby przewidzieć wydajność w przypadku nakładających się klas."
)
