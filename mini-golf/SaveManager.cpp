#include "SaveManager.h"
#include <fstream>
#include <iostream>

std::string SaveManager::fileName = "savegame.txt";
int SaveManager::unlockedLevel = 1;
std::vector<int> SaveManager::bestScores = {0, 0, 0, 0, 0}; // 5 poziomów

void SaveManager::init() {
    std::ifstream file(fileName);
    if (!file.good()) {
        // Jeśli plik nie istnieje, tworzymy domyślny
        save();
    } else {
        load();
    }
}

void SaveManager::save() {
    std::ofstream file(fileName);
    if (file.is_open()) {
        file << unlockedLevel << "\n";
        for (int score : bestScores) {
            file << score << "\n";
        }
        file.close();
    }
}

void SaveManager::load() {
    std::ifstream file(fileName);
    if (file.is_open()) {
        file >> unlockedLevel;
        
        bestScores.clear();
        int score;
        // Wczytujemy 5 wyników
        for(int i=0; i<5; i++) {
            if(file >> score) {
                bestScores.push_back(score);
            } else {
                bestScores.push_back(0);
            }
        }
        file.close();
    }
}

int SaveManager::getUnlockedLevel() {
    // Upewniamy się, że dane są świeże
    load(); 
    return unlockedLevel;
}

int SaveManager::getBestScore(int level) {
    load();
    if (level < 1 || level > 5) return 0;
    return bestScores[level - 1];
}

void SaveManager::submitScore(int level, int score) {
    load(); // Wczytaj aktualny stan

    // 1. Aktualizacja High Score
    // Jeśli stary wynik to 0 LUB nowy wynik jest lepszy (mniejszy), nadpisz.
    int currentBest = bestScores[level - 1];
    if (currentBest == 0 || score < currentBest) {
        bestScores[level - 1] = score;
    }

    if (level == unlockedLevel && unlockedLevel < 5) {
        unlockedLevel++;
    }

    save(); // Zapisz zmiany na dysk
}