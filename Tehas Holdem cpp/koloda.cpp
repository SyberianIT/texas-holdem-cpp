#include "koloda.h"
#include <algorithm>
#include <random>

Koloda::Koloda() {
    for (int m = CHERVI; m <= PIKI; ++m) {
        for (int r = DVA; r <= TUZ; ++r) {
            karty.push_back(Karta((Rang)r, (Masty)m));
        }
    }
    peretasovka();
}

void Koloda::peretasovka() {
    random_device rd;
    mt19937 g(rd());
    shuffle(karty.begin(), karty.end(), g);
}

Karta Koloda::vydatKarty() {
    Karta vydannaya = karty.back();
    karty.pop_back();
    return vydannaya;
}

int Koloda::kartOstalos() const {
    return karty.size();
}