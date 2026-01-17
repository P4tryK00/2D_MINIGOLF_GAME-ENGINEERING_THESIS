#ifndef MINI_GOLF_SAVEMANAGER_H
#define MINI_GOLF_SAVEMANAGER_H

#include <vector>
#include <string>

/**
 * @class SaveManager
 * @brief Klasa statyczna obsługująca trwałość danych gry (system zapisu/odczytu).
 *
 * Zarządza odczytem i zapisem postępów gracza (odblokowane poziomy, najlepsze wyniki)
 * do lokalnego pliku tekstowego.
 */
class SaveManager {
public:
    /**
     * @brief Inicjalizuje system zapisu.
     * Sprawdza, czy plik zapisu istnieje; jeśli nie, tworzy domyślny.
     */
    static void init();

    /**
     * @brief Pobiera numer najwyższego odblokowanego poziomu przez gracza.
     * @return Numer poziomu (1-5).
     */
    static int getUnlockedLevel();

    /**
     * @brief Pobiera najlepszy wynik (najniższa liczba uderzeń) dla danego poziomu.
     * @param level Numer poziomu do sprawdzenia.
     * @return Najlepszy wynik lub 0, jeśli brak zapisu.
     */
    static int getBestScore(int level);

    /**
     * @brief Przesyła nowy wynik ukończenia poziomu.
     * Aktualizuje najlepszy wynik, jeśli nowy jest lepszy, oraz odblokowuje kolejny poziom.
     * Automatycznie zapisuje zmiany na dysku.
     * @param level Numer ukończonego poziomu.
     * @param score Liczba wykonanych uderzeń.
     */
    static void submitScore(int level, int score);

private:
    /**
     * @brief Zapisuje bieżące dane do pliku.
     */
    static void save();

    /**
     * @brief Wczytuje dane z pliku do pamięci.
     */
    static void load();

    static std::string fileName; ///< Nazwa pliku zapisu.
    static int unlockedLevel;    ///< Indeks aktualnie odblokowanego poziomu.
    static std::vector<int> bestScores; ///< Lista najlepszych wyników dla każdego poziomu.
};

#endif //MINI_GOLF_SAVEMANAGER_H