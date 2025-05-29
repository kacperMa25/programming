import numpy as np
import soundfile as sf
import sounddevice as sd
import matplotlib.pyplot as plt
import scipy.fftpack
import librosa
from scipy.signal import find_peaks

# 1. Wczytanie i normalizacja sygnału audio
s, fs = sf.read("Jestem studentem Informatyki.wav", dtype="float32")

if len(s.shape) > 1 and s.shape[1] == 2:
    s = s[:, 0]

# Normalizacja sygnału do zakresu [-1, 1]
s_normalized = s / np.max(np.abs(s))
s = s_normalized

duration_sec = len(s) / fs  # Czas trwania w sekundach
duration_ms = duration_sec * 1000  # Czas trwania w milisekundach

# Wektor czasu w milisekundach
t_milisec = np.linspace(0, duration_ms, len(s))


# 2. Analiza okien kroczących (bez nakładania)
def analyze_windows(window_size_ms, overlap_ratio=0.0):
    window_size_samples = int(window_size_ms * fs / 1000)
    hop_size = (
        window_size_samples
        if overlap_ratio == 0
        else int(window_size_samples * (1 - overlap_ratio))
    )
    num_windows = (
        int(np.floor((len(s) - window_size_samples) / hop_size)) + 1
        if overlap_ratio > 0
        else int(np.floor(len(s) / window_size_samples))
    )

    E = np.zeros(num_windows)
    Z = np.zeros(num_windows - 1)

    # Obliczenie energii
    for i in range(num_windows):
        start_idx = i * hop_size
        end_idx = min(start_idx + window_size_samples, len(s))
        window = s[start_idx:end_idx]
        E[i] = np.sum(window**2)

    # Obliczenie przejść przez zero
    for i in range(num_windows - 1):
        start_idx = i * hop_size
        end_idx = min(start_idx + 2 * window_size_samples, len(s))
        Z[i] = np.sum(np.abs(np.diff(np.sign(s[start_idx:end_idx]))) > 0) / (
            2 * window_size_samples
        )

    # Normalizacja energii
    E_normalized = E / np.max(np.abs(E))

    # Wektory czasu dla wykresów
    t_energy = np.linspace(0, duration_ms, num_windows)
    t_zerocross = np.linspace(0, duration_ms, num_windows - 1)

    return E_normalized, Z, t_energy, t_zerocross


# Analiza dla różnych długości okien
window_sizes_ms = [5, 20, 50]
results = {}

for window_size_ms in window_sizes_ms:
    results[window_size_ms] = {
        "non_overlap": analyze_windows(window_size_ms, overlap_ratio=0),
        "overlap": analyze_windows(window_size_ms, overlap_ratio=0.5),
    }

# Wizualizacja sygnału i analizy okien
plt.figure(figsize=(12, 10))
plt.subplot(4, 1, 1)
plt.plot(t_milisec, s)
segment_start_ms = 450
segment_end_ms = segment_start_ms + 2048 / fs * 1000
plt.axvspan(segment_start_ms, segment_end_ms, color="red", alpha=0.3)
plt.title("Znormalizowany sygnał audio")
plt.xlabel("Czas [ms]")
plt.ylabel("Amplituda")

# Wykresy dla różnych długości okien
for idx, window_size_ms in enumerate(window_sizes_ms, 2):
    E_norm, Z, t_energy, t_zerocross = results[window_size_ms]["non_overlap"]
    plt.subplot(4, 1, idx)
    plt.plot(t_energy, E_norm, label="Energia")
    plt.plot(t_zerocross, Z, label="Przejścia przez zero")
    plt.title(f"Okna bez nakładania ({window_size_ms} ms)")
    plt.xlabel("Czas [ms]")
    plt.ylabel("Wielkość")
    plt.legend()
plt.tight_layout()
plt.show()

# Wizualizacja dla okien z nakładaniem (20 ms)
E_norm_overlap, Z_overlap, t_energy_overlap, t_zerocross_overlap = results[20][
    "overlap"
]
plt.figure(figsize=(12, 4))
plt.plot(t_energy_overlap, E_norm_overlap, label="Energia")
plt.plot(t_zerocross_overlap, Z_overlap, label="Przejścia przez zero")
plt.title("Okna z nakładaniem 50% (20 ms)")
plt.xlabel("Czas [ms]")
plt.ylabel("Wielkość")
plt.legend()
plt.show()

# 3. Analiza częstotliwościowa
# Ręczny wybór fragmentu samogłoski (450 ms, "e")
start_sample = int(segment_start_ms / 1000 * fs)
end_sample = start_sample + 2048
fragment = s[start_sample:end_sample]

# Zastosowanie okna Hamminga
window = np.hamming(len(fragment))
fragment_windowed = fragment * window

# Obliczenie widma FFT
yf = scipy.fftpack.fft(fragment_windowed)
xf = np.linspace(0, fs, len(yf))
spectrum = 20 * np.log10(np.abs(yf))

# Wykres widma
plt.figure(figsize=(10, 5))
plt.plot(xf[: len(xf) // 2], spectrum[: len(yf) // 2])
plt.title("Logarytmiczne widmo amplitudowe (samogłoska @ 450ms)")
plt.xlabel("Częstotliwość [Hz]")
plt.ylabel("Amplituda [dB]")
plt.xlim(0, 10000)
plt.grid()
plt.show()
# Detekcja F0
peaks, _ = find_peaks(spectrum[: len(yf) // 2], height=np.max(spectrum) * 0.2)
f0 = xf[peaks[0]]
print(f"Szacowana częstotliwość podstawowa (F0): {f0:.2f} Hz")


# 4. Analiza LPC i formantów
def analyze_lpc(fragment, p):
    a = librosa.lpc(fragment, order=p)
    a_padded = np.concatenate([a, np.zeros(2048 - len(a))])
    spectrum_lpc = -20 * np.log10(np.abs(np.fft.fft(a_padded)))
    spectrum_signal = 20 * np.log10(
        np.abs(np.fft.fft(fragment * np.hamming(len(fragment))))
    )
    return spectrum_lpc, spectrum_signal


# Testowanie różnych wartości p
p_values = [10, 20, 30]
plt.figure(figsize=(10, 8))
for i, p in enumerate(p_values, start=1):
    spectrum_lpc, spectrum_signal = analyze_lpc(fragment, p)
    plt.subplot(3, 1, i)
    plt.plot(xf[:1024], spectrum_signal[:1024], label="Widmo FFT", color="blue")
    plt.plot(xf[:1024], spectrum_lpc[:1024], label=f"Widmo LPC (p={p})", color="red")
    plt.title(f"Porównanie widma FFT i LPC (samogłoska @ 450ms, p={p})")
    plt.xlabel("Częstotliwość [Hz]")
    plt.ylabel("Amplituda [dB]")
    plt.xlim(0, 5000)
    plt.legend()
    plt.grid()
plt.tight_layout()
plt.show()

print("Odtwarzanie dźwięku...")
sd.play(s, fs)
status = sd.wait()
print("Dźwięk zakończony.")

# Analiza formantów (F1, F2) dla samogłosek
s, fs = sf.read("A e i o u y.wav", dtype="float32")
s_normalized = s / np.max(np.abs(s))
s = s_normalized
timestamps = [1600, 2150, 2650, 3250, 3840, 4350]
p = 20
for timestamp in timestamps:
    start_sample = int(timestamp / 1000 * fs)
    end_sample = start_sample + 2048
    fragment = s[start_sample:end_sample]
    spectrum_lpc, signal_lpc = analyze_lpc(fragment, p)
    peaks, _ = find_peaks(spectrum_lpc[:1024], height=np.max(spectrum_lpc[:1024]) * 0.5)
    formants = signal_lpc[peaks[:2]]  # Pierwsze dwa formanty
    print(
        f"Formanty: F1 = {formants[0]:.2f} Hz, F2 = {formants[1]:.2f} Hz, czas: {timestamp} ms"
    )

# Komentarze analityczne
"""
Analiza okien bez nakładania:
- Maksima energii (E) wskazują na fragmenty o dużej energii akustycznej, np. samogłoski i spółgłoski dźwięczne.
- Minima energii wskazują na ciszę lub dźwięki bezdźwięczne (np. 's', 'f', 'sz').
- Maksima przejść przez zero (Z) wskazują na dźwięki bezdźwięczne z częstą zmianą znaku (np. spółgłoski szczelinowe).
- Minima Z wskazują na dźwięki dźwięczne z małą liczbą przejść przez zero.

Możliwość podziału na segmenty:
- Dźwięczne: wysoka E, niski Z (np. samogłoski, spółgłoski sonorne: m, n, l, r).
- Bezdźwięczne: niska E, wysoki Z (np. 's', 'f', 'sz').
- Cisza/pauzy: bardzo niska E.

Typy głosek do rozróżnienia:
- Samogłoski: wysoka E, niski Z.
- Spółgłoski szczelinowe bezdźwięczne: niska-średnia E, wysoki Z.
- Spółgłoski zwarto-wybuchowe: impulsy w E.
- Spółgłoski sonorne: średnia E, niski Z.
- Cisza: bardzo niska E.

Nierozdzielne sekwencje:
- Głoski o podobnym miejscu artykulacji (p/t/k, s/ś/sz, m/n/ń).
- Zbitki spółgłoskowe (str, szcz, trz).
- Afrykaty (c, cz, ć, dz, dż, dź).
- Dyftongi i przejścia między samogłoskami.
- Szybka mowa z koartykulacją.

Wpływ długości okna:
- 5 ms: Wysoka rozdzielczość czasowa, ale duża zmienność E i Z; dobre dla krótkich zjawisk (np. spółgłosek wybuchowych).
- 20 ms: Standardowy kompromis, dobry do wykrywania samogłosek i spółgłosek.
- 50 ms: Wygładzenie sygnału, ale utrata detali krótkich dźwięków; lepszy dla dłuższych samogłosek.

Analiza okien z nakładaniem (50%):
- Wyższa rozdzielczość czasowa (dwukrotnie więcej punktów analizy).
- Wygładzenie E i Z, płynniejsza reprezentacja.
- Lepsza detekcja krótkich zjawisk akustycznych.
- Precyzyjniejsze granice między fonemami.
- Lepsza obserwacja subtelnych zmian w sygnale.
- Większa redundancja danych i wyższe wymagania obliczeniowe.

Liniowe Kodowanie Predykcyjne (LPC):
- LPC modeluje sygnał mowy jako wyjście filtra liniowego wzbudzanego przez impulsy (dla dźwięków dźwięcznych) lub szum (dla bezdźwięcznych).
- Służy do analizy widma sygnału mowy, identyfikacji formantów (rezonansów krtani) i kompresji sygnału.
- W naszym przypadku LPC wygładza widmo, ułatwiając identyfikację formantów (F1, F2).

Analiza formantów:
- Formanty F1 i F2 określają położenie samogłosek na wykresie 2D.
- Przykład: dla samogłoski @ 1600 ms, F1 ≈ 800 Hz, F2 ≈ 1120 Hz sugerują samogłoskę a.
- Różne wartości p (10, 20, 30) wpływają na wygładzenie widma LPC:
  - p=10: Zbyt ogólne, może pominąć formanty.
  - p=20: Dobry kompromis, wyraźne formanty.
  - p=30: Bardzo szczegółowe, ale może wprowadzać szum.

Różnice w widmach:
- Dźwięczne (np. samogłoski): Struktura harmoniczna z wyraźnymi pikami (F0 i formanty).
- Bezdźwięczne (np. 's', 'f'): Brak harmonicznych, widmo przypomina szum.
- F0 nie jest stała dla różnych głosek w tym samym nagraniu, różni się w zależności od samogłoski i intonacji.
"""

# Odtwarzanie dźwięku
print("Odtwarzanie dźwięku...")
sd.play(s, fs)
status = sd.wait()
print("Dźwięk zakończony.")
