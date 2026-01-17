#ifndef MINI_GOLF_SAVEMANAGER_H
#define MINI_GOLF_SAVEMANAGER_H


#include <vector>
#include <string>

class SaveManager {
public:
    // Inicjalizacja (tworzy plik jeśli nie istnieje)
    static void init();

    // Pobiera numer najwyższego odblokowanego poziomu (1-5)
    static int getUnlockedLevel();

    static int getBestScore(int level);

    static void submitScore(int level, int score);

private:
    static void save();
    static void load();

    static std::string fileName;
    static int unlockedLevel;
    static std::vector<int> bestScores;
};




#endif //MINI_GOLF_SAVEMANAGER_H