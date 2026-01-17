//
// Created by patry on 17.01.2026.
//

#ifndef MINI_GOLF_LEADERBOARDSTATE_H
#define MINI_GOLF_LEADERBOARDSTATE_H

#include "State.h"
#include "Game.h"
#include <vector>

/**
 * @class LeaderboardState
 * @brief Stan wyświetlający tabelę najlepszych wyników.
 *
 * Pobiera dane z SaveManager i prezentuje je w formie listy tekstowej.
 */
class LeaderboardState : public State {
public:
    /**
     * @brief Konstruktor stanu tabeli wyników.
     * @param data Dane gry.
     */
    LeaderboardState(GameDataRef data);

    /**
     * @brief Inicjalizuje widok, wczytuje wyniki i tworzy obiekty tekstowe.
     */
    void init() override;

    /**
     * @brief Obsługuje przycisk powrotu (BACK).
     */
    void handleInput(sf::Event& event) override;

    /**
     * @brief Aktualizuje efekty wizualne (fade-in, hover).
     */
    void update(float dt) override;

    /**
     * @brief Rysuje tabelę wyników.
     */
    void draw(float dt) override;

private:
    GameDataRef m_data;                 ///< Dane gry.
    sf::Text m_title;                   ///< Tytuł "BEST SCORES".
    sf::Text m_backButton;              ///< Przycisk powrotu.
    std::vector<sf::Text> m_scoreTexts; ///< Wektor tekstów z wynikami dla każdego poziomu.

    sf::RectangleShape m_fadeRect;      ///< Prostokąt przejścia.
    float m_alpha;                      ///< Przezroczystość przejścia.
    bool m_isFadingIn;                  ///< Czy trwa animacja wejścia.
    bool m_isGoingBack;                 ///< Czy trwa powrót do menu.
};

#endif //MINI_GOLF_LEADERBOARDSTATE_H