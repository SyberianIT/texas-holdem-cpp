#ifndef KOLODA_H
#define KOLODA_H

#include "karta.h"
#include <vector>

class Koloda {
private:
    vector<Karta> karty;

public:
    Koloda();
    void peretasovka();
    Karta vydatKarty();
    int kartOstalos() const;
};

#endif