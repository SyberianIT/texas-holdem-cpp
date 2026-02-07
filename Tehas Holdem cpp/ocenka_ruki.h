#ifndef OCENKA_RUKI_H
#define OCENKA_RUKI_H

#include "karta.h"
#include <vector>

struct OcenkaRuki {
    int rang; // 0-8
    vector<Rang> kickers;

    bool operator>(const OcenkaRuki& other) const;
    bool operator==(const OcenkaRuki& other) const;
};

string poluchitNazvanieRuki(int rang);
OcenkaRuki oceniRuku(vector<Karta> allCards);

#endif