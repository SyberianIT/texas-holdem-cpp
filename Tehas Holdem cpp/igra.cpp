#include "igra.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

TexasHoldem::TexasHoldem(int kolvoIgrokov, int nachalnieFishi, int sleptaya)
    : pot(0), malayaSleptaya(sleptaya) {
    for (int i = 1; i <= kolvoIgrokov; ++i) {
        igroki.push_back(Igrok("Игрок " + to_string(i), nachalnieFishi));
    }
}

void TexasHoldem::pokazatObshchieKarty() const {
    cout << "╔════════════════════════════════════╗" << endl;
    cout << "║ Общие карты: ";
    for (const auto& karta : obshchieKarty) {
        karta.display();
    }
    cout << endl << "╚════════════════════════════════════╝" << endl;
}

void TexasHoldem::pokazatPot() const {
    cout << "╔════════════════════════════════════╗" << endl;
    cout << "║ БАНК: " << setw(28) << pot << " фишек" << endl;
    cout << "╚════════════════════════════════════╝" << endl;
}

void TexasHoldem::rasstavitSleptye() {
    if (igroki[0].getFishi() >= malayaSleptaya) {
        igroki[0].subtractFishi(malayaSleptaya);
        pot += malayaSleptaya;
        igroki[0].setStavka(malayaSleptaya);
    }
    if (igroki[1].getFishi() >= malayaSleptaya * 2) {
        igroki[1].subtractFishi(malayaSleptaya * 2);
        pot += malayaSleptaya * 2;
        igroki[1].setStavka(malayaSleptaya * 2);
    }

    cout << "\n┌────────────────────────────────────┐" << endl;
    cout << "│ Малая слепая: " << igroki[0].getImya() << " ставит " << malayaSleptaya << endl;
    cout << "│ Большая слепая: " << igroki[1].getImya() << " ставит " << malayaSleptaya * 2 << endl;
    cout << "└────────────────────────────────────┘\n" << endl;
}

void TexasHoldem::vydatSkrytieKarty() {
    for (int i = 0; i < 2; ++i) {
        for (auto& igrok : igroki) {
            if (igrok.getFishi() > 0) {
                igrok.dobavitKarty(koloda.vydatKarty());
            }
        }
    }
}

void TexasHoldem::vydatFlop() {
    koloda.vydatKarty(); // Сжженная карта
    for (int i = 0; i < 3; ++i) {
        obshchieKarty.push_back(koloda.vydatKarty());
    }
}

void TexasHoldem::vydatTernIliRivey() {
    koloda.vydatKarty(); // Сожженная карта
    obshchieKarty.push_back(koloda.vydatKarty());
}

int TexasHoldem::roundStavok(int minStavka) {
    cout << "\n╔════════════════════════════════════╗" << endl;
    cout << "║      РАУНД СТАВОК" << endl;
    cout << "╚════════════════════════════════════╝" << endl;
    pokazatPot();
    int roundBet = 0;

    for (auto& igrok : igroki) {
        if (igrok.isSlozhilsya() || igrok.getFishi() == 0) continue;

        cout << "\n┌────────────────────────────────────┐" << endl;
        cout << "│ " << igrok.getImya() << " (" << igrok.getFishi() << " фишек)" << endl;
        cout << "├────────────────────────────────────┤" << endl;
        cout << "│ Ваши карты: ";
        auto skrytieKarty = igrok.getSkrytieKarty();
        for (const auto& karta : skrytieKarty) {
            karta.display();
        }
        cout << endl;
        cout << "└────────────────────────────────────┘" << endl;
        pokazatObshchieKarty();

        int action = -1;
        while (action < 0 || action > 2) {
            cout << "Выберите действие:" << endl;
            cout << "  [0] Фолд (сбросить)" << endl;
            cout << "  [1] Чек/Колл (уравнять)" << endl;
            cout << "  [2] Рейз (повысить ставку)" << endl;
            cout << "➜ ";
            cin >> action;
            if (action < 0 || action > 2) {
                cout << "❌ Неверный выбор! Попробуйте ещё раз.\n" << endl;
            }
        }

        if (action == 0) {
            igrok.slozhis();
            cout << "\n✓ " << igrok.getImya() << " сбросил карты.\n" << endl;
        } else if (action == 1) {
            int callAmount = minStavka - igrok.getStavka();
            int actualBet = min(callAmount, igrok.getFishi());
            igrok.subtractFishi(actualBet);
            igrok.setStavka(igrok.getStavka() + actualBet);
            pot += actualBet;
            cout << "\n✓ " << igrok.getImya() << " уравнял ставку на " << actualBet << " фишек.\n" << endl;
        } else if (action == 2) {
            int raiseAmount = -1;
            while (raiseAmount <= minStavka || raiseAmount > igrok.getFishi()) {
                cout << "Введите сумму рейза (минимум " << minStavka + 1 << "): ";
                cin >> raiseAmount;
                if (raiseAmount <= minStavka || raiseAmount > igrok.getFishi()) {
                    cout << "❌ Неверная сумма! Попробуйте ещё раз.\n" << endl;
                }
            }
            int actualBet = min(raiseAmount, igrok.getFishi());
            igrok.subtractFishi(actualBet);
            igrok.setStavka(igrok.getStavka() + actualBet);
            pot += actualBet;
            minStavka = actualBet;
            roundBet++;
            cout << "\n✓ " << igrok.getImya() << " повысил ставку до " << actualBet << " фишек.\n" << endl;
        }
    }

    return roundBet;
}

void TexasHoldem::opredelitPobeditelya() {
    cout << "\n╔════════════════════════════════════╗" << endl;
    cout << "║         ВСКРЫТИЕ КАРТ" << endl;
    cout << "╚════════════════════════════════════╝\n" << endl;
    pokazatObshchieKarty();

    OcenkaRuki bestHand;
    bestHand.rang = -1;
    Igrok* pobeditel = nullptr;

    for (auto& igrok : igroki) {
        if (igrok.isSlozhilsya()) continue;

        cout << igrok.getImya() << " показывает: ";
        igrok.showSkrytieKarty();

        vector<Karta> allCards = igrok.getSkrytieKarty();
        for (const auto& karta : obshchieKarty) {
            allCards.push_back(karta);
        }

        OcenkaRuki hand = oceniRuku(allCards);
        cout << "Комбинация: " << poluchitNazvanieRuki(hand.rang) << endl << endl;

        if (bestHand.rang == -1 || hand > bestHand) {
            bestHand = hand;
            pobeditel = &igrok;
        }
    }

    if (pobeditel) {
        cout << "╔════════════════════════════════════╗" << endl;
        cout << "║ 🎉 " << pobeditel->getImya() << " ВЫИГРАЛ!" << endl;
        cout << "║ Банк: " << pot << " фишек" << endl;
        cout << "╚════════════════════════════════════╝\n" << endl;
        pobeditel->addFishi(pot);
    }
}

void TexasHoldem::ochistiRuku() {
    koloda = Koloda();
    obshchieKarty.clear();
    pot = 0;
    for (auto& igrok : igroki) {
        igrok.ochistiRuku();
    }
}

void TexasHoldem::sygratRuku() {
    cout << "\n╔════════════════════════════════════╗" << endl;
    cout << "║     НОВАЯ РАЗДАЧА" << endl;
    cout << "╚════════════════════════════════════╝\n" << endl;

    ochistiRuku();
    rasstavitSleptye();

    // Pre-flop
    vydatSkrytieKarty();
    cout << "═══════════════════════════════════════" << endl;
    cout << "       ПРЕФЛОП (до общих карт)" << endl;
    cout << "═══════════════════════════════════════" << endl;
    roundStavok(malayaSleptaya * 2);

    // Flop
    vydatFlop();
    cout << "═══════════════════════════════════════" << endl;
    cout << "           ФЛОП (3 карты)" << endl;
    cout << "═══════════════════════════════════════" << endl;
    pokazatObshchieKarty();
    roundStavok(0);

    // Turn
    vydatTernIliRivey();
    cout << "═══════════════════════════════════════" << endl;
    cout << "           ТЕРН (4-я карта)" << endl;
    cout << "═══════════════════════════════════════" << endl;
    pokazatObshchieKarty();
    roundStavok(0);

    // River
    vydatTernIliRivey();
    cout << "═══════════════════════════════════════" << endl;
    cout << "           РИВЕР (5-я карта)" << endl;
    cout << "═══════════════════════════════════════" << endl;
    pokazatObshchieKarty();
    roundStavok(0);

    // Showdown
    opredelitPobeditelya();

    cout << "───────────────────────────────────────" << endl;
    for (auto& igrok : igroki) {
        igrok.showFishi();
    }
    cout << "───────────────────────────────────────" << endl;
}

void TexasHoldem::igrat() {
    cout << "\n";
    cout << "╔════════════════════════════════════╗" << endl;
    cout << "║   🃏 ТЕХАССКИЙ ХОЛДЕМ 🃏" << endl;
    cout << "║   Консольная версия" << endl;
    cout << "╚════════════════════════════════════╝" << endl;
    cout << "Игроков: " << igroki.size() << endl;
    cout << "Начальный банк: 1000 фишек" << endl;
    cout << "═══════════════════════════════════════\n" << endl;

    while (true) {
        bool anyPlayerActive = false;
        for (const auto& igrok : igroki) {
            if (igrok.getFishi() > 0) {
                anyPlayerActive = true;
                break;
            }
        }

        if (!anyPlayerActive) {
            cout << "\n╔════════════════════════════════════╗" << endl;
            cout << "║       ИГРА ЗАКОНЧЕНА!" << endl;
            cout << "╚════════════════════════════════════╝" << endl;
            break;
        }

        sygratRuku();

        cout << "Сыграть ещё раздачу?" << endl;
        cout << "  [1] Да" << endl;
        cout << "  [0] Нет" << endl;
        cout << "➜ ";
        int choice;
        cin >> choice;
        if (choice == 0) break;
        cout << endl;
    }
}