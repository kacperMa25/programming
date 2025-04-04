import numpy as np
import matplotlib.pyplot as plt


def dft(x):
    """Oblicza Dyskretną Transformatę Fouriera (DFT) sygnału x"""
    N = len(x)
    X = np.zeros(N, dtype=complex)
    for k in range(N):
        for n in range(N):
            X[k] += x[n] * np.exp(-2j * np.pi * k * n / N)
    return X


# **1. Generacja sygnału sinusoidalnego**
fs = 100  # Częstotliwość próbkowania (16 kHz)
T = 0.012  # Czas trwania sygnału (2s)
N = fs * T  # Liczba próbek
t = np.linspace(0, T, N)  # Próbkowanie sygnału

f1 = 500  # Hz
f2 = 1000  # Hz
A1 = 1.0
A2 = 0.5

x_t = A1 * np.sin(2 * np.pi * f1 * t) + A2 * np.sin(2 * np.pi * f2 * t)  # Sygnał

# **2. Obliczenie DFT**
X_k = dft(x_t)

# **3. Wyznaczenie widma amplitudowego**
M_k = np.abs(X_k)[: N // 2]  # Widmo dla dodatnich częstotliwości

# **4. Wyznaczenie skali częstotliwości**
freqs = np.arange(N // 2) * fs / N  # Skala częstotliwości

# **5. Wygenerowanie wykresów**

plt.figure(figsize=(12, 6))

# Wykres sygnału w dziedzinie czasu
plt.subplot(2, 1, 1)
plt.plot(t[:1000], x_t[:1000])  # 1000 próbek dla lepszej widoczności
plt.title("Sygnał x(t) w dziedzinie czasu")
plt.xlabel("Czas [s]")
plt.ylabel("Amplituda")

# Wykres widma amplitudowego
plt.subplot(2, 1, 2)
plt.plot(freqs, M_k)
plt.title("Widmo amplitudowe M(k)")
plt.xlabel("Częstotliwość [Hz]")
plt.ylabel("Amplituda")

plt.tight_layout()
plt.show()
