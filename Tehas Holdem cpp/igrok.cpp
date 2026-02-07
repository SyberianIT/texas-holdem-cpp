#include "igrok.h"
#include <iostream>
#include <algorithm>

Igrok::Igrok(string name, int nachalnieFishi) 
    : imya(name), fishi(nachalnieFishi), stavka(0), slozhilsya(false) {}

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