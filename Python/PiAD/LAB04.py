import numpy as np
import matplotlib.pyplot as plt

#Część 1. Dyskretyzacja

#zadanie 1-2
def genSin(f, fs):
    t = np.linspace(0, 1, fs)
    return t, np.sin(2 * np.pi * f * t)


# zadanie 3
f = 10
fs = np.array((20, 21, 30, 45, 50, 100, 150, 200, 250, 1000))
t, y_t = genSin(f, 20)

for i, fs2 in enumerate(fs):
    plt.subplot(5, 2, i + 1)
    t, y_t = genSin(f, fs2)
    plt.plot(t, y_t)
    plt.title(f"f = {f}Hz, Fs = {fs2}Hz", fontsize=4)
    plt.xlabel("Czas [s]", fontsize=4)
    plt.ylabel("Amplituda", fontsize=4)
    plt.grid(True)

plt.tight_layout()
plt.show()

# zadanie 4
# Twierdzenie to nazywa się Twierdzeniem o próbkowaniu i mówi,
# że częstotliwość próbkowania musi być co najmniej 2 razy większa
# niż częstotliwość sygnału


# zadanie 5
# zjawisko to nazywa się aliasingiem co sprawia
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


# zadanie 6-7
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

#Część 2. Kwantyzacja
# zadanie 2
print("Wymiary obrazu:", grid.shape)

# zadanie 3
print("Liczba kanałów (np. RGB):",
      grid.shape[2])

# zadanie 4
if len(grid.shape) == 3 and grid.shape[2] == 3:
    gray1 = ((np.max(grid, axis=2) + np.min(grid, axis=2)) / 2).astype(np.uint8)
    gray2 = np.mean(grid, axis=2).astype(np.uint8)
    gray3 = (0.21 * grid[:, :, 0] + 0.72 * grid[:, :, 1] + 0.07 * grid[:, :, 2]).astype(np.uint8)
else:
    gray1 = gray2 = gray3 = grid


# Zadanie 5
plt.figure(figsize=(12, 4))

for i, gray in enumerate([gray1, gray2, gray3]):
    plt.subplot(1, 3, i+1)
    plt.hist(gray.ravel(), bins=256, color='gray')
    plt.title(f"Histogram metody {i+1}")
    plt.xlabel("Wartość piksela")
    plt.ylabel("Liczba pikseli")


#zadanie 6
hist, bins = np.histogram(gray3, bins=16)
print("Przedziały histogramu:", bins)

#zadanie 7
midpoints = ((bins[:-1] + bins[1:]) / 2).astype(np.uint8)
print("Środki przedziałów:", midpoints)

gray_quantized = np.digitize(gray3, bins) - 1
gray_quantized = np.clip(gray_quantized, 0, len(midpoints) - 1)
quantized_img = midpoints[gray_quantized]

# zadanie 8
images = [gray1, gray2, gray3, quantized_img]
titles = ["Jasność", "Średnia", "Luminancja", "Zredukowana (16 kolorów)"]

plt.figure(figsize=(14, 8))
for i, img in enumerate(images):
    plt.subplot(2, 4, i+1)
    plt.imshow(img, cmap='gray')
    plt.title(titles[i])
    plt.axis('off')

    plt.subplot(2, 4, i+5)
    plt.hist(img.ravel(), bins=256, color='gray')
    plt.title(f"Histogram {titles[i]}")

plt.tight_layout()
plt.show()

# Część 3. Binaryzacja
#zadanie 1
img = plt.imread("niejednorodny.png")

#zadanie 2
if img.shape[-1] == 4:
    img = img[:, :, :3]

gray = 0.21 * img[:, :, 0] + 0.72 * img[:, :, 1] + 0.07 * img[:, :, 2]

plt.hist(gray.ravel(), bins=256, color="gray")
plt.title("Histogram luminancji")
plt.xlabel("Wartość piksela")
plt.ylabel("Liczba pikseli")
plt.show()

#zadanie 3
threshold = np.mean(gray)

#zadanie 4
binary_img = (gray > threshold).astype(np.uint8) * 255

plt.figure(figsize=(12, 5))

plt.subplot(1, 2, 1)
plt.imshow(img)
plt.title("Oryginalny obraz")
plt.axis("off")

plt.subplot(1, 2, 2)
plt.imshow(binary_img, cmap="gray")
plt.title("Binaryzacja")
plt.axis("off")

plt.show()