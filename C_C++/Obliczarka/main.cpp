#include <iostream>
using namespace std;

int main() {
    double czescWspolna = 0, czescG = 0, czescK = 0, polNaPol = 0, doOddania;
    int wybor0, wybor1;
    for (;;) {
        cout
            << "Wybierz: [40 - 60] (1), czesc Oli (2), czesc Kacpra (3), [50 - "
               "50] (4) lub koniec (0)\n";
        cin >> wybor0;
        if (wybor0 == 1) {
            double liczba;
            cout << "Liczba:\n";
            for (;;) {
                cin >> liczba;
                if (liczba != 0) {
                    czescWspolna += liczba;
                } else
                    break;
            }
            cout << czescWspolna << endl;
        } // Program liczy część wpólną [40-60]
        else if (wybor0 == 2) {
            double liczba;
            cout << "Liczba: \n";
            for (;;) {
                cin >> liczba;
                if (liczba != 0) {
                    czescG = czescG + liczba;
                } else
                    break;
            }
            cout << czescG << endl;
        } // Porgram liczy część Oli
        else if (wybor0 == 3) {
            double liczba;
            cout << "Liczba: \n";
            for (;;) {
                cin >> liczba;
                if (liczba != 0) {
                    czescK = czescK + liczba;
                } else
                    break;
            }
            cout << czescK << endl;
        } // Program liczy część moją
        else if (wybor0 == 4) {
            double liczba;
            cout << "Liczba: \n";
            for (;;) {
                cin >> liczba;
                if (liczba != 0) {
                    polNaPol = polNaPol + liczba;
                } else
                    break;
            }
            cout << polNaPol << endl;
        } // Program liczy część [50 - 50]
        else if (wybor0 == 0) {
            break;
        } // Program wychodzi z pętli głównej
    }
    cout << "Kto oddaje: Ola(1) czy Kacper(2)?\n";
    cin >> wybor1;
    if (wybor1 == 1) {
        doOddania = (czescWspolna * 0.4) + (polNaPol * 0.5) + czescG;
        cout << doOddania << endl;
    } else if (wybor1 == 2) {
        doOddania = (czescWspolna * 0.6) + (polNaPol * 0.5) + czescK;
        cout << doOddania << endl;
    }
    std::getchar();
}
