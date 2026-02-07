#include "ocenka_ruki.h"
#include <algorithm>
#include <map>

bool OcenkaRuki::operator>(const OcenkaRuki& other) const {
    if (rang != other.rang) return rang > other.rang;
    for (size_t i = 0; i < kickers.size(); ++i) {
        if (kickers[i] != other.kickers[i]) {
            return kickers[i] > other.kickers[i];
        }
    }
    return false;
}

bool OcenkaRuki::operator==(const OcenkaRuki& other) const {
    return rang == other.rang && kickers == other.kickers;
}

string poluchitNazvanieRuki(int rang) {
    string nazvania[] = {
        "Старшая карта",
        "Пара",
        "Две пары",
        "Тройка",
        "Стрит",
        "Флеш",
        "Фулл-хаус",
        "Каре",
        "Стрит-флеш"
    };
    return nazvania[rang];
}

OcenkaRuki oceniRuku(vector<Karta> allCards) {
    sort(allCards.begin(), allCards.end(), [](const Karta& a, const Karta& b) {
        return a.rang > b.rang;
    });

    map<Rang, int> schyotRang;
    map<Masty, int> schyotMast;

    for (const auto& karta : allCards) {
        schyotRang[karta.rang]++;
        schyotMast[karta.mast]++;
    }

    bool isFlesh = false;
    for (const auto& p : schyotMast) {
        if (p.second >= 5) {
            isFlesh = true;
            break;
        }
    }

    bool isStrit = false;
    vector<Rang> stritKarty;
    for (int i = 0; i < (int)allCards.size() - 4; ++i) {
        if (allCards[i].rang - allCards[i+4].rang == 4) {
            isStrit = true;
            for (int j = 0; j < 5; ++j) {
                stritKarty.push_back(allCards[i+j].rang);
            }
            break;
        }
    }

    if (!isStrit && schyotRang[TUZ] && schyotRang[PYAT] && schyotRang[CHETYRE] && 
        schyotRang[TRI] && schyotRang[DVA]) {
        isStrit = true;
        stritKarty = { PYAT, CHETYRE, TRI, DVA, TUZ };
    }

    vector<pair<Rang, int>> schioty;
    for (const auto& p : schyotRang) {
        schioty.push_back({ p.first, p.second });
    }
    sort(schioty.begin(), schioty.end(), [](const pair<Rang, int>& a, const pair<Rang, int>& b) {
        if (a.second != b.second) return a.second > b.second;
        return a.first > b.first;
    });

    OcenkaRuki result;
    result.kickers.clear();

    if (isStrit && isFlesh) {
        result.rang = 8;
        result.kickers = stritKarty;
    } else if (schioty[0].second == 4) {
        result.rang = 7;
        result.kickers.push_back(schioty[0].first);
        result.kickers.push_back(schioty[1].first);
    } else if (schioty[0].second == 3 && schioty[1].second == 2) {
        result.rang = 6;
        result.kickers.push_back(schioty[0].first);
        result.kickers.push_back(schioty[1].first);
    } else if (isFlesh) {
        result.rang = 5;
        for (const auto& karta : allCards) {
            if (result.kickers.size() < 5) {
                result.kickers.push_back(karta.rang);
            }
        }
    } else if (isStrit) {
        result.rang = 4;
        result.kickers = stritKarty;
    } else if (schioty[0].second == 3) {
        result.rang = 3;
        result.kickers.push_back(schioty[0].first);
        for (const auto& s : schioty) {
            if (s.second == 1 && result.kickers.size() < 3) {
                result.kickers.push_back(s.first);
            }
        }
    } else if (schioty[0].second == 2 && schioty[1].second == 2) {
        result.rang = 2;
        result.kickers.push_back(schioty[0].first);
        result.kickers.push_back(schioty[1].first);
        for (const auto& s : schioty) {
            if (s.second == 1) {
                result.kickers.push_back(s.first);
            }
        }
    } else if (schioty[0].second == 2) {
        result.rang = 1;
        result.kickers.push_back(schioty[0].first);
        for (const auto& s : schioty) {
            if (s.second == 1 && result.kickers.size() < 4) {
                result.kickers.push_back(s.first);
            }
        }
    } else {
        result.rang = 0;
        for (const auto& karta : allCards) {
            if (result.kickers.size() < 5) {
                result.kickers.push_back(karta.rang);
            }
        }
    }

    return result;
}