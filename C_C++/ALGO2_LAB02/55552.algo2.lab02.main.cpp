//ALGO2 IS1 223A LAB02
//Kacper Małecki
//mk55552@zut.edu.pl

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <random>
#include <chrono>
#include "DynamicArray.h"


using namespace std::chrono;

int main() {
    DynamicArray<int, Less> ar;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<>dis(0, 10000);

    for(int i = 0; i < MAX; ++i){
        int n = std::pow(2, i);

        auto begin = steady_clock::now();
        for(int j = 0; j < n; ++j){
            ar.pushBack(dis(gen));
        }
        auto end = steady_clock::now();

        auto duration = duration_cast<milliseconds>(end - begin);

        std::cout << "===============================" << std::endl
        << ar.toString() << "Dodawanie do listy trwalo (koszt zamortyzowany): " << (static_cast<double>(duration.count()) / 1000) / n << std::endl;

        int m = pow(1.2, i);
        begin = steady_clock::now();
        for(int j = 0; j < 1; ++j){
            ar.sort();
        }
        end = steady_clock::now();

        duration = duration_cast<milliseconds>(end - begin);
        std::cout << "Sortowanie listy trwalo: ";
        std::cout <<  static_cast<double>(duration.count()) / 1000 << "s" << std::endl;
    }
}
