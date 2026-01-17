#ifndef MINI_GOLF_LEVELSELECTSTATE_H
#define MINI_GOLF_LEVELSELECTSTATE_H

#include "State.h"
#include "Game.h"
#include <vector>
#include "GameState.h"
#include "MainMenuState.h"
#include "ResourceManager.h"
#include "DEFINITIONS.h"

/**
 * @class LevelSelectState
 * @brief Stan wyboru poziomu.
 *
 * Wyświetla listę dostępnych poziomów. Komunikuje się z SaveManagerem,
 * aby zablokować poziomy, do których gracz nie ma jeszcze dostępu (oznaczone jako LOCKED).
 */
class LevelSelectState : public State {
public:
    /**
     * @brief Konstruktor stanu wyboru poziomu.
     * @param data Dane gry.
     */
    LevelSelectState(GameDataRef data);

    /**
     * @brief Tworzy listę przycisków na podstawie liczby poziomów i postępu gracza.
     */
    void init() override;

    /**
     * @brief Obsługuje wybór poziomu lub powrót do menu.
     * Blokuje kliknięcia w nieodblokowane poziomy.
     * @param event Zdarzenie SFML.
     */
    void handleInput(sf::Event& event) override;

    /**
     * @brief Aktualizuje logikę podświetlania dostępnych poziomów.
     * @param dt Czas klatki.
     */
    void update(float dt) override;

    /**
     * @brief Rysuje listę poziomów.
     * @param dt Interpolacja.
     */
    void draw(float dt) override;

private:
    GameDataRef m_data;

    sf::Text m_title;      ///< Nagłówek "SELECT LEVEL".
    sf::Text m_backButton; ///< Przycisk powrotu.

    std::vector<sf::Text> m_levelButtons; ///< Dynamiczna lista przycisków poziomów.

    // Zmienne do przejścia (Fader)
    sf::RectangleShape m_fadeRect;
    float m_alpha;
    bool m_isFadingIn;
    bool m_isTransitioning;
    int m_nextLevelToLoad; ///< Numer wybranego poziomu (-1 oznacza powrót).
};

#endif //MINI_GOLF_LEVELSELECTSTATE_H