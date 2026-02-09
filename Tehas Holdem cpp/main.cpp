#include "igra.h"
#include <iostream>
#include <limits>

using std::cout;
using std::cin;
using std::endl;

static void clearScreenMain() {
    cout << "\033[2J\033[H";
}

static int safeReadInt(int defaultValue = 0) {
    int value = defaultValue;
    cin >> value;
    if (!cin) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return defaultValue;
    }
    return value;
}

int main() {
    while (true) {
        clearScreenMain();
        cout << "╔════════════════════════════════════╗\n";
        cout << "║        🃏 TEXAS HOLD'EM 🃏        ║\n";
        cout << "╠════════════════════════════════════╣\n";
        cout << "║  1. Новая игра                     ║\n";
        cout << "║  0. Выход                          ║\n";
        cout << "╚════════════════════════════════════╝\n";
        cout << "Выбор: ";

        int menuChoice = safeReadInt();
        if (menuChoice == 0) {
            break;
        }
        if (menuChoice != 1) {
            continue;
        }

        clearScreenMain();
        cout << "Сколько игроков будет за столом? (2-6): ";
        int kolvoIgrokov = safeReadInt(2);
        if (kolvoIgrokov < 2) kolvoIgrokov = 2;
        if (kolvoIgrokov > 6) kolvoIgrokov = 6;

        clearScreenMain();
        cout << "╔════════════════════════════════════╗\n";
        cout << "║        ВЫБОР СЛОЖНОСТИ ИИ          ║\n";
        cout << "╠════════════════════════════════════╣\n";
        cout << "║  1. Лёгкая   (как сейчас)          ║\n";
        cout << "║  2. Средняя  (сложнее ~в 3 раза)   ║\n";
        cout << "║  3. Сложная  (сложнее ~в 10 раз)   ║\n";
        cout << "╚════════════════════════════════════╝\n";
        cout << "Выбор: ";

        int diffChoice = safeReadInt(1);
        if (diffChoice < 1) diffChoice = 1;
        if (diffChoice > 3) diffChoice = 3;

        Slozhnost slozh = Slozhnost::Legkaya;
        if (diffChoice == 2) slozh = Slozhnost::Srednyaya;
        else if (diffChoice == 3) slozh = Slozhnost::Slozhnaya;

        TexasHoldem game(kolvoIgrokov, 1000, 10, slozh);
        game.igrat();
    }

    return 0;
}