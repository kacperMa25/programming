#include "matplotlibcpp.h"
#include <cmath>
#include <math.h>
#include <vector>

int main() {
    // f = 2.0 Hz
    // Tc = 2s
    // phi = 0
    // fs = 8000 Hz
    double f = 2.0;
    double Tc = 2;
    double phi = 0;
    double fs = 8000;

    auto fun1 = [](double t, double phi, double f) {
        return std::cos(2 * M_PI * t * f + phi) *
               std::cos(2.5 * std::pow(t, 0.2) * M_PI);
    };

    double *tab = new double[std::floor(Tc * fs)];

    int j = 0;
    for (double i = 0; i <= std::floor(Tc * fs); i += 1 / fs, ++j) {
        tab[j] = i;
        std::cout << tab[j];
    }
    return 0;
}
