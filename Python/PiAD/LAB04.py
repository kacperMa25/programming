import numpy as np
import matplotlib.pyplot as plt


def genSin(f, fs):
    t = np.linspace(0, 1, fs)
    return t, np.sin(2 * np.pi * f * t)


f = 5
fs = np.array((20, 21, 30, 45, 50, 100, 150, 200, 250, 1000))
t, y_t = genSin(f, 20)

for i, fs2 in enumerate(fs):
    plt.subplot(5, 2, i + 1)
    t, y_t = genSin(f, fs2)
    plt.plot(t, y_t)

plt.tight_layout()
plt.show()

# zadanie 4
# Twierdzenie to nazywa się Twierdzeniem o próbkowaniu i mówi,
# że częstotliwość próbkowania musi być co najmniej 2 razy większa
# niż częstotliwość sygnału


# zadanie 5
# zjawisko to nazywa się aliasingiem co sprawia
# pojawianie się fałszywych częstotliwości


methods = [
    None,
    "none",
    "nearest",
    "bilinear",
    "bicubic",
    "spline16",
    "spline36",
    "hanning",
    "hamming",
    "hermite",
    "kaiser",
    "quadric",
    "catrom",
    "gaussian",
    "bessel",
    "mitchell",
    "sinc",
    "lanczos",
]

# Fixing random state for reproducibility
np.random.seed(19680801)

grid = np.random.rand(4, 4)
grid = plt.imread("Moire_pattern_of_bricks.jpg")

fig, axs = plt.subplots(
    nrows=3, ncols=6, figsize=(9, 6), subplot_kw={"xticks": [], "yticks": []}
)

for ax, interp_method in zip(axs.flat, methods):
    ax.imshow(grid, interpolation=interp_method, cmap="viridis")
    ax.set_title(str(interp_method))

plt.tight_layout()
plt.show()

print(grid.shape)
