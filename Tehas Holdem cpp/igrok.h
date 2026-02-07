#ifndef IGROK_H
#define IGROK_H

#include "karta.h"
#include <vector>
#include <string>

class Igrok {
private:
    string imya;
    int fishi;
    int stavka;
    bool slozhilsya;
    vector<Karta> skrytieKarty;

public:
    Igrok(string name, int nachalnieFishi);
    
    void dobavitKarty(Karta karta);
    void ochistiRuku();
    vector<Karta> getSkrytieKarty() const;
    string getImya() const;
    int getFishi() const;
    void addFishi(int amount);
    void subtractFishi(int amount);
    int getStavka() const;
    void setStavka(int amount);
    void resetStavka();
    bool isSlozhilsya() const;
    void slozhis();
    void showSkrytieKarty() const;
    void showFishi() const;
};

#endif