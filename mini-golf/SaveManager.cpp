#include "SaveManager.h"
#include <fstream>
#include <iostream>

// Inicjalizacja zmiennych statycznych
std::string SaveManager::fileName = "savegame.txt";
int SaveManager::unlockedLevel = 1;
std::vector<int> SaveManager::bestScores = {0, 0, 0, 0, 0}; // 5 poziomów

void SaveManager::init() {
    std::ifstream file(fileName);
    if (!file.good()) {
        // Jeśli plik nie istnieje (pierwsze uruchomienie), tworzymy nowy z domyślnymi wartościami
        save();
    } else {
        // Jeśli istnieje, wczytujemy stan
        load();
    }
}

void SaveManager::save() {
    std::ofstream file(fileName);
    if (file.is_open()) {
        // Zapis: 1 linia = odblokowany poziom
        file << unlockedLevel << "\n";
        // Kolejne linie = wyniki dla poziomów
        for (int score : bestScores) {
            file << score << "\n";
        }
        file.close();
    }
}

void SaveManager::load() {
    std::ifstream file(fileName);
    if (file.is_open()) {
        // Odczyt poziomu
        file >> unlockedLevel;

        bestScores.clear();
        int score;
        // Wczytujemy 5 wyników
        for(int i=0; i<5; i++) {
            if(file >> score) {
                bestScores.push_back(score);
            } else {
                bestScores.push_back(0); // Zabezpieczenie przed błędami formatu
            }
        }
        file.close();
    }
}

int SaveManager::getUnlockedLevel() {
    // Upewniamy się, że dane są świeże (odczyt z pliku)
    load();
    return unlockedLevel;
}

int SaveManager::getBestScore(int level) {
    load();
    if (level < 1 || level > 5) return 0;
    return bestScores[level - 1]; // Indeksowanie wektora od 0
}

void SaveManager::submitScore(int level, int score) {
    load(); // Wczytaj aktualny stan przed modyfikacją

    // 1. Aktualizacja High Score (Best Score)
    // Jeśli stary wynik to 0 (brak gry) LUB nowy wynik jest lepszy (mniejszy), nadpisz.
    int currentBest = bestScores[level - 1];
    if (currentBest == 0 || score < currentBest) {
        bestScores[level - 1] = score;
    }

    // 2. Odblokowanie następnego poziomu
    // Jeśli ukończyliśmy poziom równy aktualnie odblokowanemu, zwiększamy limit.
    if (level == unlockedLevel && unlockedLevel < 5) {
        unlockedLevel++;
    }

    save(); // Zapisz zmiany na dysk
}