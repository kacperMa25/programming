// ALGO2 IS1
// Kacper Małecki
// mk55552@zut.edu.pl

#include "../LAB02/DynamicArray.h"
#include <chrono>
#include <cmath>
#include <complex.h>
#include <complex>
#include <iostream>

const std::complex<double> i_complex(0.0, 1.0);

DynamicArray<std::complex<double>>
DFT(const DynamicArray<std::complex<double>> &f) {
    int size = f.getSize();
    DynamicArray<std::complex<double>> c(size, 0);
    for (int k = 0; k < size; ++k) {
        for (int n = 0; n < size; ++n) {
            double exp = -2 * k * n * M_PI / size;
            c[k] += f[n] * std::exp(i_complex * exp);
        }
    }

    return c;
}

DynamicArray<std::complex<double>>
FFT(const DynamicArray<std::complex<double>> &f) {
    int size = f.getSize();
    if (size == 1) {
        return f;
    }

    DynamicArray<std::complex<double>> feven(size / 2), fodd(size / 2);
    for (int i = 0; i < size / 2; ++i) {
        feven[i] = f[i * 2];
        fodd[i] = f[i * 2 + 1];
    }

    auto ceven = FFT(feven);
    auto codd = FFT(fodd);

    DynamicArray<std::complex<double>> c(size);
    std::complex<double> omega = std::exp(-2.0 * M_PI / size * i_complex);
    std::complex<double> current = 1.0;

    for (int k = 0; k < size / 2; ++k) {
        std::complex<double> r = current * codd[k];
        c[k] = ceven[k] + r;
        c[k + size / 2] = ceven[k] - r;
        current *= omega;
    }

    return c;
}

double fun1(double x, double N) { return x / N; }

void testFun1() {
    DynamicArray<std::complex<double>> f;
    for (int i = 1; i < 4; ++i) {
        const int n = 1 << i;

        std::cout << "Rozmiar: " << n << std::endl;

        for (int j = 0; j < n; ++j) {
            f.pushBack(std::complex(fun1(j, n)));
        }

        auto begin = std::chrono::high_resolution_clock::now();
        auto result = DFT(f);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

        std::cout << "Czas DFT: "
                  << static_cast<double>(duration.count()) / 1000 << " s"
                  << std::endl;

        begin = std::chrono::high_resolution_clock::now();
        result = FFT(f);
        end = std::chrono::high_resolution_clock::now();

        duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

        std::cout << "Czas FFT: "
                  << static_cast<double>(duration.count()) / 1000 << " s"
                  << std::endl;
        std::cout << result << std::endl;

        f.clear();
    }
}

int main() { testFun1(); }
