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
    if (!file.is_open()) return;

    // Zmienne tymczasowe (zgodnie z opisem w pracy - zabezpieczenie przed corruption)
    int tempLevel = 1;
    std::vector<int> tempScores;
    bool isCorrupted = false;

    // 1. Wczytanie i walidacja poziomu
    if (file >> tempLevel) {
        // Sanity Check: Zakres 1-5
        if (tempLevel < 1 || tempLevel > 5) {
            std::cout << "[WARNING] Save file corrupted (Level out of bounds: " << tempLevel << "). Resetting to 1.\n";
            tempLevel = 1;
            isCorrupted = true;
        }
    } else {
        isCorrupted = true;
    }

    // 2. Wczytanie wyników
    int score;
    for (int i = 0; i < 5; i++) {
        if (file >> score) {
            // Walidacja ujemnych wyników
            if (score < 0) {
                std::cout << "[WARNING] Negative score detected. Resetting to 0.\n";
                score = 0;
                isCorrupted = true;
            }
            tempScores.push_back(score);
        } else {
            // Plik się skończył za wcześnie
            std::cout << "[WARNING] Save file corrupted (Missing data). Filling with 0.\n";
            tempScores.push_back(0);
            isCorrupted = true;
        }
    }

    file.close();

    // 3. Zatwierdzenie danych (Commit)
    // Przepisujemy dane tylko po przetworzeniu całości.
    // Nawet jak plik był uszkodzony, mamy bezpieczne wartości w zmiennych temp.
    unlockedLevel = tempLevel;
    bestScores = tempScores;

    if (isCorrupted) {
        // Opcjonalnie: Napraw plik na dysku natychmiast
        save();
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