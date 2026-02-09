#ifndef KARTA_H
#define KARTA_H

#include <iostream>
#include <string>

using namespace std;

enum Masty { CHERVI = 0, BUBNI = 1, TREFYI = 2, PIKI = 3 };
enum Rang { DVA = 2, TRI, CHETYRE, PYAT, SHEST, SEM, OSEM, DEVYAT, DESYAT, VALET, DAMA, KOROL, TUZ };

struct Karta {
    Rang rang;
    Masty mast;

    Karta() : rang(DVA), mast(CHERVI) {}
    Karta(Rang r, Masty m) : rang(r), mast(m) {}

    string toString() const {
        string rangStr[] = { "", "", "2", "3", "4", "5", "6", "7", "8", "9", "10", "В", "Д", "К", "Т" };
        string mastStr[] = { "♥", "♦", "♣", "♠" };
        return rangStr[rang] + mastStr[mast];
    }

    void display() const {
        // Цветной вывод: красный для червей/бубен, белый для треф/пик
        string card = toString();
        string colorStart;
        string colorReset = "\033[0m";
        if (mast == CHERVI || mast == BUBNI) {
            colorStart = "\033[31m"; // красный
        } else {
            colorStart = "\033[37m"; // белый/серый
        }
        cout << colorStart << card << colorReset << " ";
    }
};

#endif