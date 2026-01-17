#ifndef MINI_GOLF_LEVELCOMPLETESTATE_H
#define MINI_GOLF_LEVELCOMPLETESTATE_H

#include "State.h"
#include "Game.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "ResourceManager.h"
#include "DEFINITIONS.h"
#include <memory>

/**
 * @class LevelCompleteState
 * @brief Stan gry wyświetlany po pomyślnym ukończeniu poziomu.
 *
 * Klasa ta odpowiada za:
 * - Wyświetlenie podsumowania poziomu (wynik/ilość uderzeń).
 * - Zapisanie postępów gracza (odblokowanie kolejnego poziomu).
 * - Nawigację do następnego poziomu, powtórzenia obecnego lub powrotu do menu.
 */
class LevelCompleteState : public State {
public:
    /**
     * @brief Konstruktor stanu ukończenia poziomu.
     * @param data Współdzielone dane gry (okno, maszyna stanów).
     * @param bgSprite Zrzut ekranu z gry (tło).
     * @param bgTexture Tekstura zrzutu ekranu.
     * @param strokes Liczba uderzeń wykonana w zakończonym poziomie.
     * @param currentLevel Numer ukończonego poziomu.
     */
    LevelCompleteState(GameDataRef data, sf::Sprite bgSprite, std::shared_ptr<sf::Texture> bgTexture, int strokes, int currentLevel);

    /**
     * @brief Inicjalizuje elementy interfejsu (teksty, przyciski) oraz zapisuje wynik.
     */
    void init() override;

    /**
     * @brief Obsługuje interakcję użytkownika z przyciskami (Next Level, Retry, Menu).
     * @param event Zdarzenie SFML.
     */
    void handleInput(sf::Event& event) override;

    /**
     * @brief Aktualizuje logikę stanu (animacje przycisków, efekt przejścia).
     * @param dt Czas klatki (Delta Time).
     */
    void update(float dt) override;

    /**
     * @brief Renderuje elementy stanu na ekranie.
     * @param dt Interpolacja (nieużywana).
     */
    void draw(float dt) override;

private:
    GameDataRef m_data;             ///< Wskaźnik do danych gry.
    sf::Sprite m_backgroundSprite;  ///< Tło (screenshot z gry).
    std::shared_ptr<sf::Texture> m_bgTexture; ///< Tekstura tła.

    int m_strokes;      ///< Wynik gracza.
    int m_currentLevel; ///< Numer ukończonego poziomu.

    sf::RectangleShape m_dimmer; ///< Prostokąt przyciemniający tło.

    sf::Text m_title;           ///< Nagłówek "LEVEL COMPLETE".
    sf::Text m_scoreText;       ///< Tekst wyświetlający liczbę uderzeń.

    sf::Text m_nextLevelButton; ///< Przycisk przejścia do kolejnego poziomu.
    sf::Text m_menuButton;      ///< Przycisk powrotu do menu.
    sf::Text m_retryButton;     ///< Przycisk powtórzenia poziomu.

    // Zmienne do efektu przejścia (Fader)
    sf::RectangleShape m_fadeRect; ///< Prostokąt do efektu zanikania.
    float m_alpha;                 ///< Aktualna przezroczystość fadera.
    bool m_isTransitioning;        ///< Flaga trwania animacji wyjścia.
    bool m_goToMenu;               ///< Cel przejścia: Menu.
    bool m_retryLevel;             ///< Cel przejścia: Restart.
};

#endif //MINI_GOLF_LEVELCOMPLETESTATE_H