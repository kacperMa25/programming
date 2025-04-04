import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import scipy as sp

# Zadanie 1
print("\nZadanie1\n")
df = pd.DataFrame({"x": [1, 2, 3, 4, 5], "y": ["a", "b", "a", "b", "b"]})

print(df.groupby(by="y").mean())

# Zadanie 2
print("\nZadanie2\n")
print(df.value_counts())

# Zadanie 3
print("\nZadanie3\n")
df = pd.read_csv("autos.csv")
print(df)
# Zadanie 4
print("\nZadanie4\n")
df["combined-mpg"] = (df["highway-mpg"] + df["city-mpg"]) / 2
print(df[["make", "city-mpg", "highway-mpg", "combined-mpg"]].groupby(["make"]).mean())

# Zadanie 5
print("\nZadanie5\n")
print(df[["make", "fuel-type"]].groupby("make").count())

# Zadanie 6
print("\nZadanie6\n")
poly1 = np.polyfit(df["length"].to_numpy(), df["city-mpg"].to_numpy(), 1)
poly2 = np.polyfit(df["length"].to_numpy(), df["city-mpg"].to_numpy(), 2)

pred_poly1 = np.polyval(poly1, np.unique(df["length"].to_numpy()))
pred_poly2 = np.polyval(poly2, np.unique(df["length"].to_numpy()))

print("\nPrzewidywane city-mpg (2 stopień):", pred_poly2)
print("\nPrzewidywane city-mpg (1 stopień):", pred_poly2)


# Zadanie 7
print("\nZadanie7\n")
print(
    "Współczynnik korelacji:",
    sp.stats.pearsonr(df["length"], df["city-mpg"]).correlation,
)

# Zadanie 8
print("\nZadanie8\n")
plt.figure()
plt.xlabel("length")
plt.ylabel("city-mpg")
plt.scatter(df["length"], df["city-mpg"])
plt.plot(
    np.unique(df["length"].to_numpy()),
    pred_poly1,
    label="Wielomian 1. stopnia",
    color="red",
)
plt.plot(
    np.unique(df["length"].to_numpy()),
    pred_poly2,
    label="Wielomian 2. stopnia",
    color="blue",
)

plt.legend()
plt.show()

# Zadanie 9
print("\nZadanie9\n")
x = df["length"]
gestosc = sp.stats.gaussian_kde(x)
x_val = np.linspace(x.min(), x.max(), 1000)
plt.figure()
plt.plot(x_val, gestosc(x_val), label="Funkcja gęstości")
plt.scatter(x, np.zeros_like(x), label="Rozłożenie próbek", color="red")
plt.legend()
plt.show()

# Zadanie 10
print("\nZadanie10\n")
x2 = df["width"]
gestosc2 = sp.stats.gaussian_kde(x2)
x_val2 = np.linspace(x2.min(), x2.max(), 1000)

_, axs = plt.subplots(1, 2)
axs[0].plot(x_val2, gestosc2(x_val2), label="Funkcja gęstości szerokości")
axs[0].scatter(x2, np.zeros_like(x2), label="Próbki", color="red")
axs[0].legend()
axs[1].plot(x_val, gestosc(x_val), label="Funkcja gęstości długości", color="green")
axs[1].scatter(x, np.zeros_like(x), label="Próbki", color="red")
axs[1].legend()

plt.tight_layout()
plt.show()

# Zadanie 11
print("\nZadanie11\n")
gestosc_2d = sp.stats.gaussian_kde([x, x2])
x_vals, x2_vals = np.meshgrid(
    np.linspace(x.min(), x.max(), 100), np.linspace(x2.min(), x2.max(), 100)
)

z_vals = gestosc_2d(np.vstack([x_vals.ravel(), x2_vals.ravel()])).reshape(100, 100)
plt.figure()
plt.contour(x_vals, x2_vals, z_vals)
plt.xlabel("Długość")
plt.ylabel("Szerokość")
plt.show()
