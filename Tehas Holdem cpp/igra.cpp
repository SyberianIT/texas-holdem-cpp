#include "igra.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <random>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;

static void clearScreen() {
    cout << "\033[2J\033[H";
}

TexasHoldem::TexasHoldem(int kolvoIgrokov, int nachalnieFishi, int sleptaya, Slozhnost slozh)
    : pot(0), malayaSleptaya(sleptaya), slozhnost(slozh) {
    // Первый игрок — человек, остальные по умолчанию боты среднего уровня
    for (int i = 1; i <= kolvoIgrokov; ++i) {
        TipIgorka tip = (i == 1) ? TipIgorka::Chelovek : TipIgorka::BotSrednii;
        igroki.push_back(Igrok("Игрок " + to_string(i), nachalnieFishi, tip));
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
    if (igroki.size() < 2) return;

    // Для простоты пока без вращения блайндов: первый — малая, второй — большая
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
    clearScreen();
    cout << "╔════════════════════════════════════╗" << endl;
    cout << "║      РАУНД СТАВОК" << endl;
    cout << "╚════════════════════════════════════╝" << endl;
    pokazatPot();
    int roundBet = 0;

    // Текущая максимальная ставка за столом
    int tekushayaMaxStavka = minStavka;
    for (const auto& igrok : igroki) {
        if (!igrok.isSlozhilsya())
            tekushayaMaxStavka = std::max(tekushayaMaxStavka, igrok.getStavka());
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    for (auto& igrok : igroki) {
        if (igrok.isSlozhilsya() || igrok.getFishi() == 0) continue;

        clearScreen();
        pokazatPot();

        cout << "\n┌────────────────────────────────────┐" << endl;
        cout << "│ " << igrok.getImya() << " (" << igrok.getFishi() << " фишек)" << endl;
        cout << "├────────────────────────────────────┤" << endl;
        cout << "│ Карты: ";
        auto skrytieKarty = igrok.getSkrytieKarty();
        for (const auto& karta : skrytieKarty) {
            karta.display();
        }
        cout << endl;
        cout << "└────────────────────────────────────┘" << endl;
        pokazatObshchieKarty();

        int callAmount = std::max(0, tekushayaMaxStavka - igrok.getStavka());

        int action = -1;
        if (igrok.isChelovek()) {
            while (action < 0 || action > 2) {
                cout << "Выберите действие:" << endl;
                cout << "  [0] Фолд (сбросить)" << endl;
                cout << "  [1] Чек/Колл (уравнять " << callAmount << ")" << endl;
                cout << "  [2] Рейз (повысить ставку)" << endl;
                cout << "➜ ";
                cin >> action;
                if (action < 0 || action > 2) {
                    cout << "❌ Неверный выбор! Попробуйте ещё раз.\n" << endl;
                }
            }
        } else {
            // Оценка силы руки + небольшой шанс блефа
            std::uniform_real_distribution<> dist(0.0, 1.0);

            bool mozhnoOcenitRuku = (skrytieKarty.size() + obshchieKarty.size() >= 5);
            int handRank = 0;
            if (mozhnoOcenitRuku) {
                vector<Karta> allCards = skrytieKarty;
                for (const auto& c : obshchieKarty) {
                    allCards.push_back(c);
                }
                OcenkaRuki h = oceniRuku(allCards);
                handRank = h.rang; // 0-8
            }

            double r = dist(gen);
            double bluffRoll = dist(gen);

            double bluffProb = 0.0;
            switch (igrok.getTip()) {
                case TipIgorka::BotPassivnyi:   bluffProb = 0.05; break;
                case TipIgorka::BotSrednii:     bluffProb = 0.15; break;
                case TipIgorka::BotAggressivnyi:bluffProb = 0.30; break;
                default:                         bluffProb = 0.10; break;
            }

            // Модификатор сложности: лёгкая = как сейчас,
            // средняя ≈ в 3 раза агрессивнее, сложная ≈ в 10 раз.
            double coef = 1.0;
            switch (slozhnost) {
                case Slozhnost::Legkaya:   coef = 1.0; break;
                case Slozhnost::Srednyaya: coef = 3.0; break;
                case Slozhnost::Slozhnaya: coef = 10.0; break;
            }
            bluffProb *= coef;
            if (bluffProb > 0.8) bluffProb = 0.8;

            // Если рука откровенно слабая, но сработал шанс блефа — играем агрессивно
            if (mozhnoOcenitRuku && handRank <= 1 && bluffRoll < bluffProb && igrok.getFishi() > 0) {
                action = 2; // блефовый рейз
            } else {
                // Базовое поведение по типу бота и силе руки
                switch (igrok.getTip()) {
                    case TipIgorka::BotPassivnyi:
                        if (mozhnoOcenitRuku && handRank >= 4) {
                            // сильные руки иногда рейзят
                            action = (r < 0.6 ? 1 : 2);
                        } else {
                            if (r < 0.6) action = 1;       // чаще колл
                            else if (r < 0.9) action = 0;  // иногда фолд
                            else action = 2;               // редко рейз
                        }
                        break;
                    case TipIgorka::BotSrednii:
                        if (mozhnoOcenitRuku && handRank >= 4) {
                            action = (r < 0.2 ? 1 : 2);   // со стритом+ чаще рейз
                        } else if (mozhnoOcenitRuku && handRank >= 2) {
                            if (r < 0.1) action = 0;
                            else if (r < 0.6) action = 1;
                            else action = 2;
                        } else {
                            if (r < 0.2) action = 0;
                            else if (r < 0.75) action = 1;
                            else action = 2;
                        }
                        break;
                    case TipIgorka::BotAggressivnyi:
                        if (mozhnoOcenitRuku && handRank >= 3) {
                            action = (r < 0.1 ? 1 : 2);   // сет и выше почти всегда рейз
                        } else {
                            if (r < 0.05) action = 0;
                            else if (r < 0.35) action = 1;
                            else action = 2;
                        }
                        break;
                    default:
                        if (r < 0.2) action = 0;
                        else if (r < 0.7) action = 1;
                        else action = 2;
                        break;
                }
            }
        }

        if (action == 0) {
            igrok.slozhis();
            cout << "\n✓ " << igrok.getImya() << " сбросил карты.\n" << endl;
        } else if (action == 1) {
            int actualBet = std::min(callAmount, igrok.getFishi());
            igrok.subtractFishi(actualBet);
            igrok.dobavitKStavkeStatistiki(actualBet);
            igrok.setStavka(igrok.getStavka() + actualBet);
            pot += actualBet;
            cout << "\n✓ " << igrok.getImya() << " уравнял ставку на " << actualBet << " фишек.\n" << endl;
        } else if (action == 2) {
            int raiseAmount = 0;
            int maxRaise = igrok.getFishi();
            if (igrok.isChelovek()) {
                while (raiseAmount <= 0 || raiseAmount > maxRaise) {
                    cout << "Введите сумму рейза (дополнительно к коллу, 1-" << maxRaise << "): ";
                    cin >> raiseAmount;
                    if (raiseAmount <= 0 || raiseAmount > maxRaise) {
                        cout << "❌ Неверная сумма! Попробуйте ещё раз.\n" << endl;
                    }
                }
            } else {
                // Боты ставят размер в зависимости от сложности
                int minRaise = 1;
                int maxR = std::max(1, maxRaise);
                if (slozhnost == Slozhnost::Srednyaya) {
                    minRaise = maxR / 3;
                } else if (slozhnost == Slozhnost::Slozhnaya) {
                    minRaise = maxR / 2;
                }
                if (minRaise < 1) minRaise = 1;
                if (minRaise > maxR) minRaise = maxR;
                std::uniform_int_distribution<> raiseDist(minRaise, maxR);
                raiseAmount = raiseDist(gen);
            }

            int totalBet = igrok.getStavka() + callAmount + raiseAmount;
            int chipsToPut = totalBet - igrok.getStavka();
            chipsToPut = std::min(chipsToPut, igrok.getFishi());

            igrok.subtractFishi(chipsToPut);
            igrok.dobavitKStavkeStatistiki(chipsToPut);
            igrok.setStavka(igrok.getStavka() + chipsToPut);
            pot += chipsToPut;
            tekushayaMaxStavka = std::max(tekushayaMaxStavka, igrok.getStavka());
            roundBet++;
            cout << "\n✓ " << igrok.getImya() << " повысил ставку, теперь его ставка " << igrok.getStavka() << " фишек.\n" << endl;
        }
    }

    return roundBet;
}

OcenkaRuki TexasHoldem::opredelitPobeditelya(int& winnerIndex) {
    cout << "\n╔════════════════════════════════════╗" << endl;
    cout << "║         ВСКРЫТИЕ КАРТ" << endl;
    cout << "╚════════════════════════════════════╝\n" << endl;
    pokazatObshchieKarty();

    OcenkaRuki bestHand;
    bestHand.rang = -1;
    Igrok* pobeditel = nullptr;
    winnerIndex = -1;

    for (auto& igrok : igroki) {
        if (igrok.isSlozhilsya()) continue;

        cout << igrok.getImya() << " показывает: ";
        igrok.showSkrytieKarty();

        vector<Karta> allCards = igrok.getSkrytieKarty();
        for (const auto& karta : obshchieKarty) {
            allCards.push_back(karta);
        }

        OcenkaRuki hand = oceniRuku(allCards);
        cout << "Комбинация: " << opisatRuku(hand) << endl << endl;

        if (bestHand.rang == -1 || hand > bestHand) {
            bestHand = hand;
            pobeditel = &igrok;
            // вычислим индекс победителя
            for (size_t i = 0; i < igroki.size(); ++i) {
                if (&igroki[i] == pobeditel) {
                    winnerIndex = static_cast<int>(i);
                    break;
                }
            }
        }
    }

    if (pobeditel) {
        cout << "╔════════════════════════════════════╗" << endl;
        cout << "║ 🎉 " << pobeditel->getImya() << " ВЫИГРАЛ!" << endl;
        cout << "║ Банк: " << pot << " фишек" << endl;
        cout << "╚════════════════════════════════════╝\n" << endl;
        pobeditel->addFishi(pot);
    }

    return bestHand;
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

    // Запомним стеки до раздачи и увеличим счётчик раздач
    std::vector<int> startFishi(igroki.size());
    for (size_t i = 0; i < igroki.size(); ++i) {
        startFishi[i] = igroki[i].getFishi();
        if (igroki[i].getFishi() > 0) {
            igroki[i].uvelichitRuki();
            igroki[i].sbrositResultatRuki();
        }
    }

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
    int winnerIndex = -1;
    OcenkaRuki bestHand = opredelitPobeditelya(winnerIndex);

    // Обновим результат раздачи и посчитаем стеки после
    std::vector<int> endFishi(igroki.size());
    for (size_t i = 0; i < igroki.size(); ++i) {
        endFishi[i] = igroki[i].getFishi();
        int delta = endFishi[i] - startFishi[i];
        igroki[i].zapisatResultatRuki(delta);
    }

    // Сохраним историю раздачи в файл
    std::ofstream history("history.txt", std::ios::app);
    if (history.is_open()) {
        history << "===== Новая раздача =====\n";
        for (size_t i = 0; i < igroki.size(); ++i) {
            history << igroki[i].getImya()
                    << " : было " << startFishi[i]
                    << ", стало " << endFishi[i]
                    << ", результат " << (endFishi[i] - startFishi[i])
                    << "\n";
        }
        if (winnerIndex >= 0) {
            history << "Победитель: " << igroki[winnerIndex].getImya()
                    << " (" << opisatRuku(bestHand) << ")\n";
        } else {
            history << "Победитель: нет (все сбросились)\n";
        }
        history << "\n";
    }

    cout << "───────────────────────────────────────" << endl;
    for (auto& igrok : igroki) {
        igrok.showFishi();
    }
    cout << "───────────────────────────────────────" << endl;

    // Показать базовую статистику
    cout << "\nСтатистика игроков:\n";
    for (const auto& igrok : igroki) {
        int ruk = igrok.getVsegoRuk();
        int vyig = igrok.getVyigrannoRuk();
        int proig = igrok.getProigrannoRuk();
        long long sumStavok = igrok.getObshayaStavka();
        double percentPobed = (ruk > 0) ? (100.0 * vyig / ruk) : 0.0;
        double avgBet = (ruk > 0) ? (static_cast<double>(sumStavok) / ruk) : 0.0;

        cout << igrok.getImya()
             << " | побед: " << vyig
             << ", поражений: " << proig
             << ", всего рук: " << ruk
             << ", % побед: " << std::fixed << std::setprecision(1) << percentPobed
             << ", средняя ставка: " << std::setprecision(1) << avgBet
             << ", результат этой раздачи: " << igrok.getPosledniyResultat()
             << endl;
    }
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