#include "igrok.h"
#include <iostream>
#include <algorithm>

Igrok::Igrok(string name, int nachalnieFishi, TipIgorka tipIgorka) 
    : imya(name),
      fishi(nachalnieFishi),
      stavka(0),
      slozhilsya(false),
      skrytieKarty(),
      tip(tipIgorka),
      vyigrannoRuk(0),
      proigrannoRuk(0),
      posledniyResultat(0),
      vsegoRuk(0),
      obshayaStavka(0) {}

void Igrok::dobavitKarty(Karta karta) {
    if (skrytieKarty.size() < 2) {
        skrytieKarty.push_back(karta);
    }
}

void Igrok::ochistiRuku() {
    skrytieKarty.clear();
    slozhilsya = false;
    stavka = 0;
}

vector<Karta> Igrok::getSkrytieKarty() const {
    return skrytieKarty;
}

string Igrok::getImya() const {
    return imya;
}

int Igrok::getFishi() const {
    return fishi;
}

void Igrok::addFishi(int amount) {
    fishi += amount;
}

void Igrok::subtractFishi(int amount) {
    fishi -= amount;
}

int Igrok::getStavka() const {
    return stavka;
}

void Igrok::setStavka(int amount) {
    stavka = amount;
}

void Igrok::resetStavka() {
    stavka = 0;
}

bool Igrok::isSlozhilsya() const {
    return slozhilsya;
}

void Igrok::slozhis() {
    slozhilsya = true;
}

TipIgorka Igrok::getTip() const {
    return tip;
}

bool Igrok::isChelovek() const {
    return tip == TipIgorka::Chelovek;
}

void Igrok::sbrositResultatRuki() {
    posledniyResultat = 0;
}

void Igrok::zapisatResultatRuki(int deltaFishi) {
    posledniyResultat = deltaFishi;
    if (deltaFishi > 0) {
        vyigrannoRuk++;
    } else if (deltaFishi < 0) {
        proigrannoRuk++;
    }
}

int Igrok::getPosledniyResultat() const {
    return posledniyResultat;
}

int Igrok::getVyigrannoRuk() const {
    return vyigrannoRuk;
}

int Igrok::getProigrannoRuk() const {
    return proigrannoRuk;
}

void Igrok::uvelichitRuki() {
    vsegoRuk++;
}

void Igrok::dobavitKStavkeStatistiki(int amount) {
    obshayaStavka += amount;
}

int Igrok::getVsegoRuk() const {
    return vsegoRuk;
}

long long Igrok::getObshayaStavka() const {
    return obshayaStavka;
}

void Igrok::showSkrytieKarty() const {
    cout << imya << " - скрытые карты: ";
    for (const auto& karta : skrytieKarty) {
        karta.display();
    }
    cout << endl;
}

void Igrok::showFishi() const {
    cout << imya << " имеет " << fishi << " фишек" << endl;
}