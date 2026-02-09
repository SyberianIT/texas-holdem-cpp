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

static string nazvanieRangaDlyaFrazy(Rang r) {
    switch (r) {
        case DVA: return "двоек";
        case TRI: return "троек";
        case CHETYRE: return "четвёрок";
        case PYAT: return "пятёрок";
        case SHEST: return "шестёрок";
        case SEM: return "семёрок";
        case OSEM: return "восьмёрок";
        case DEVYAT: return "девяток";
        case DESYAT: return "десяток";
        case VALET: return "валетов";
        case DAMA: return "дам";
        case KOROL: return "королей";
        case TUZ: return "тузов";
        default: return "";
    }
}

static string nazvanieStarshiyRang(Rang r) {
    switch (r) {
        case DVA: return "двойка";
        case TRI: return "тройка";
        case CHETYRE: return "четвёрка";
        case PYAT: return "пятёрка";
        case SHEST: return "шестёрка";
        case SEM: return "семёрка";
        case OSEM: return "восьмёрка";
        case DEVYAT: return "девятка";
        case DESYAT: return "десятка";
        case VALET: return "валет";
        case DAMA: return "дама";
        case KOROL: return "король";
        case TUZ: return "туз";
        default: return "";
    }
}

string opisatRuku(const OcenkaRuki& hand) {
    switch (hand.rang) {
        case 0:
            if (!hand.kickers.empty())
                return "Старшая карта " + nazvanieStarshiyRang(hand.kickers[0]);
            return "Старшая карта";
        case 1:
            if (!hand.kickers.empty())
                return "Пара " + nazvanieRangaDlyaFrazy(hand.kickers[0]);
            return "Пара";
        case 2:
            if (hand.kickers.size() >= 2)
                return "Две пары: " + nazvanieRangaDlyaFrazy(hand.kickers[0]) +
                       " и " + nazvanieRangaDlyaFrazy(hand.kickers[1]);
            return "Две пары";
        case 3:
            if (!hand.kickers.empty())
                return "Тройка " + nazvanieRangaDlyaFrazy(hand.kickers[0]);
            return "Тройка";
        case 4:
            if (!hand.kickers.empty())
                return "Стрит до " + nazvanieStarshiyRang(hand.kickers[0]);
            return "Стрит";
        case 5:
            return "Флеш";
        case 6:
            return "Фулл-хаус";
        case 7:
            if (!hand.kickers.empty())
                return "Каре " + nazvanieRangaDlyaFrazy(hand.kickers[0]);
            return "Каре";
        case 8:
            if (!hand.kickers.empty())
                return "Стрит-флеш до " + nazvanieStarshiyRang(hand.kickers[0]);
            return "Стрит-флеш";
        default:
            return "Неизвестная комбинация";
    }
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