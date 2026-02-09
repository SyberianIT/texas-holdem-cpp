#ifndef IGRA_H
#define IGRA_H

#include "igrok.h"
#include "koloda.h"
#include "ocenka_ruki.h"
#include <vector>

enum class Slozhnost {
    Legkaya = 1,
    Srednyaya = 2,
    Slozhnaya = 3
};

class TexasHoldem {
private:
    vector<Igrok> igroki;
    vector<Karta> obshchieKarty;
    Koloda koloda;
    int pot;
    int malayaSleptaya;
    Slozhnost slozhnost;

    void pokazatObshchieKarty() const;
    void pokazatPot() const;
    void rasstavitSleptye();
    void vydatSkrytieKarty();
    void vydatFlop();
    void vydatTernIliRivey();
    int roundStavok(int minStavka);
    OcenkaRuki opredelitPobeditelya(int& winnerIndex);
    void ochistiRuku();

public:
    TexasHoldem(int kolvoIgrokov = 2,
                int nachalnieFishi = 1000,
                int malayaSleptaya = 10,
                Slozhnost slozh = Slozhnost::Legkaya);
    void sygratRuku();
    void igrat();
};

#endif