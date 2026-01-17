#ifndef MINI_GOLF_MAINMENUSTATE_H
#define MINI_GOLF_MAINMENUSTATE_H

#include "State.h"
#include "Game.h"
#include "LevelSelectState.h"
#include "SaveManager.h"
#include "LeaderBoardState.h"

/**
 * @class MainMenuState
 * @brief Stan początkowy gry (Menu Główne).
 *
 * Klasa odpowiada za wyświetlenie głównego interfejsu użytkownika po uruchomieniu aplikacji.
 * Umożliwia nawigację do:
 * - Wyboru poziomów (Play).
 * - Tabeli wyników (Leaderboard).
 * - Wyjścia z aplikacji (Exit).
 */
class MainMenuState : public State {
public:
    /**
     * @brief Konstruktor stanu Menu.
     * @param data Współdzielone dane gry.
     */
    MainMenuState(GameDataRef data);

    /**
     * @brief Inicjalizuje widok menu, ładuje czcionki i ustawia pozycje przycisków.
     * Inicjalizuje również system zapisu (SaveManager).
     */
    void init() override;

    /**
     * @brief Obsługuje kliknięcia w przyciski menu.
     * @param event Zdarzenie SFML.
     */
    void handleInput(sf::Event& event) override;

    /**
     * @brief Aktualizuje efekty wizualne (podświetlenie przycisków, animacje wejścia/wyjścia).
     * @param dt Czas klatki (Delta Time).
     */
    void update(float dt) override;

    /**
     * @brief Rysuje interfejs menu na ekranie.
     * @param dt Interpolacja (nieużywana).
     */
    void draw(float dt) override;

private:
    GameDataRef m_data; ///< Dane gry.

    sf::Sprite m_background; ///< Tło menu (opcjonalne, w kodzie .cpp używany jest jednolity kolor).

    sf::Text m_title;             ///< Tytuł gry "MINI GOLF".
    sf::Text m_playButton;        ///< Przycisk "PLAY".
    sf::Text m_exitButton;        ///< Przycisk "EXIT".
    sf::Text m_leaderboardButton; ///< Przycisk "LEADERBOARD".

    // Zmienne do efektu przejścia (Fader)
    sf::RectangleShape m_fadeRect; ///< Prostokąt przyciemniający ekran.
    float m_alpha;                 ///< Kanał alpha dla efektu fade.
    bool m_isTransitioning;        ///< Czy trwa animacja wyjścia do innego stanu.
    bool m_isFadingIn;             ///< Czy trwa animacja wejścia (rozjaśnianie).
    bool m_goToLeaderboard;        ///< Flaga decydująca o celu przejścia (Leaderboard vs LevelSelect).
};

#endif //MINI_GOLF_MAINMENUSTATE_H