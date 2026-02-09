#ifndef IGROK_H
#define IGROK_H

#include "karta.h"
#include <vector>
#include <string>

enum class TipIgorka {
    Chelovek,
    BotPassivnyi,
    BotSrednii,
    BotAggressivnyi
};

class Igrok {
private:
    string imya;
    int fishi;
    int stavka;
    bool slozhilsya;
    vector<Karta> skrytieKarty;
    TipIgorka tip;
    int vyigrannoRuk;
    int proigrannoRuk;
    int posledniyResultat; // изменение фишек за последнюю раздачу
    int vsegoRuk;
    long long obshayaStavka; // суммарно вложено фишек во все раздачи

public:
    Igrok(string name, int nachalnieFishi, TipIgorka tipIgorka = TipIgorka::Chelovek);
    
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
    TipIgorka getTip() const;
    bool isChelovek() const;
    void sbrositResultatRuki();
    void zapisatResultatRuki(int deltaFishi);
    int getPosledniyResultat() const;
    int getVyigrannoRuk() const;
    int getProigrannoRuk() const;
    void uvelichitRuki();
    void dobavitKStavkeStatistiki(int amount);
    int getVsegoRuk() const;
    long long getObshayaStavka() const;
    void showSkrytieKarty() const;
    void showFishi() const;
};

#endif