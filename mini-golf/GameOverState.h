#ifndef MINI_GOLF_GAMEOVERTSTATE_H
#define MINI_GOLF_GAMEOVERTSTATE_H

#include "State.h"
#include "Game.h"
#include <memory>

/**
 * @class GameOverState
 * @brief Stan gry wyświetlany po przegranej (Game Over).
 *
 * Klasa dziedzicząca po State. Wyświetla komunikat o końcu gry,
 * przyciemnia tło i pozwala na restart poziomu lub powrót do menu.
 */
class GameOverState : public State {
public:
    /**
     * @brief Konstruktor stanu GameOver.
     * * @param data Współdzielone dane gry.
     * @param bgSprite Zrzut ekranu z momentu przegranej (tło).
     * @param bgTexture Tekstura tła (przechowywana, aby wskaźnik był ważny).
     * @param level Numer poziomu, na którym nastąpiła przegrana (do restartu).
     */
    GameOverState(GameDataRef data, sf::Sprite bgSprite, std::shared_ptr<sf::Texture> bgTexture, int level);

    /**
     * @brief Inicjalizuje elementy interfejsu (teksty, przyciski, tło).
     */
    void init() override;

    /**
     * @brief Obsługuje wejście od użytkownika (kliknięcia myszką).
     * @param event Zdarzenie SFML.
     */
    void handleInput(sf::Event& event) override;

    /**
     * @brief Aktualizuje logikę stanu (np. animacje przycisków, efekt fade-in).
     * @param dt Czas klatki (Delta Time).
     */
    void update(float dt) override;

    /**
     * @brief Rysuje wszystkie elementy stanu na ekranie.
     * @param dt Interpolacja klatki (nieużywana w tym stanie, ale wymagana przez interfejs).
     */
    void draw(float dt) override;

private:
    GameDataRef m_data; ///< Wskaźnik do danych gry.
    sf::Sprite m_backgroundSprite; ///< Sprite tła (screenshot gry).
    std::shared_ptr<sf::Texture> m_bgTexture; ///< Tekstura tła.
    int m_level; ///< Numer poziomu do ewentualnego restartu.

    sf::RectangleShape m_dimmer; ///< Prostokąt przyciemniający tło.

    sf::Text m_title;       ///< Tekst "GAME OVER".
    sf::Text m_retryButton; ///< Przycisk "TRY AGAIN".
    sf::Text m_menuButton;  ///< Przycisk "MAIN MENU".

    // Zmienne do efektu przejścia (Fader)
    sf::RectangleShape m_fadeRect; ///< Prostokąt do efektu zanikania/pojawiania.
    float m_alpha;                 ///< Aktualna przezroczystość fadera.
    bool m_isTransitioning;        ///< Flaga czy trwa przejście między stanami.
    bool m_goToMenu;               ///< Flaga wyboru powrotu do menu.
    bool m_retryLevel;             ///< Flaga wyboru restartu poziomu.
};

#endif //MINI_GOLF_GAMEOVERTSTATE_H