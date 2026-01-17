#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "StateManager.h"
#include "DEFINITIONS.h"

/**
 * @struct GameData
 * @brief Struktura przechowująca współdzielone dane gry.
 *
 * Obiekt tej struktury jest przekazywany jako Smart Pointer (GameDataRef)
 * do wszystkich stanów gry, umożliwiając im dostęp do okna i maszyny stanów.
 */
struct GameData {
    StateManager machine; ///< Maszyna stanów zarządzająca przejściami (Menu -> Gra -> Pauza).
    sf::RenderWindow window; ///< Główne okno renderowania SFML.
};

/**
 * @typedef GameDataRef
 * @brief Alias dla std::shared_ptr<GameData>. Ułatwia przekazywanie danych gry.
 */
typedef std::shared_ptr<GameData> GameDataRef;

/**
 * @class Game
 * @brief Główna klasa silnika gry.
 *
 * Odpowiada za inicjalizację okna, zarządzanie czasem (Game Loop)
 * oraz uruchomienie maszyny stanów.
 */
class Game {
public:
    /**
     * @brief Konstruktor inicjalizujący grę.
     * * Tworzy okno aplikacji i ustawia pierwszy stan (MainMenuState).
     * * @param width Szerokość okna.
     * @param height Wysokość okna.
     * @param title Tytuł okna.
     */
    Game(int width, int height, std::string title);

    /**
     * @brief Uruchamia główną pętlę gry.
     *
     * Pętla obsługuje:
     * - Zmiany stanów (processStateChanges)
     * - Aktualizację logiki (update) ze stałym krokiem czasowym (fixed time step).
     * - Renderowanie (draw) z interpolacją.
     */
    void run();

private:
    /**
     * @brief Stały czas trwania jednej klatki logicznej (1/60 sekundy).
     */
    const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

    sf::Clock clock; ///< Zegar do mierzenia czasu rzeczywistego.

    /**
     * @brief Współdzielone dane gry (okno, maszyna stanów).
     */
    GameDataRef m_data = std::make_shared<GameData>();
};

#endif // GAME_H