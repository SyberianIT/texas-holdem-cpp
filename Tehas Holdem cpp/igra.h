#ifndef IGRA_H
#define IGRA_H

#include "igrok.h"
#include "koloda.h"
#include "ocenka_ruki.h"
#include <vector>

class TexasHoldem {
private:
    vector<Igrok> igroki;
    vector<Karta> obshchieKarty;
    Koloda koloda;
    int pot;
    int malayaSleptaya;

    void pokazatObshchieKarty() const;
    void pokazatPot() const;
    void rasstavitSleptye();
    void vydatSkrytieKarty();
    void vydatFlop();
    void vydatTernIliRivey();
    int roundStavok(int minStavka);
    void opredelitPobeditelya();
    void ochistiRuku();

public:
    TexasHoldem(int kolvoIgrokov = 2, int nachalnieFishi = 1000, int malayaSleptaya = 10);
    void sygratRuku();
    void igrat();
};

#endif