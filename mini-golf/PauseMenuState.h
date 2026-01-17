#ifndef MINI_GOLF_PAUSEMENUSTATE_H
#define MINI_GOLF_PAUSEMENUSTATE_H

#include "State.h"
#include "Game.h"
#include <memory>

/**
 * @class PauseMenuState
 * @brief Stan pauzy nakładany na stan gry.
 *
 * Jest to stan typu "overlay" (nakładka). Nie usuwa stanu gry ze stosu,
 * lecz rysuje się "na nim", wykorzystując zrzut ekranu jako tło.
 */
class PauseMenuState : public State {
public:
    /**
     * @brief Konstruktor stanu pauzy.
     *
     * @param data Dane gry.
     * @param bgSprite Sprite ze zrzutem ekranu gry w momencie pauzy.
     * @param bgTexture Wskaźnik do tekstury zrzutu (shared_ptr zapewnia, że tekstura nie zostanie usunięta).
     * @param levelNumber Numer aktualnego poziomu (potrzebny do opcji Restart).
     */
    PauseMenuState(GameDataRef data, sf::Sprite bgSprite, std::shared_ptr<sf::Texture> bgTexture, int levelNumber);

    /**
     * @brief Inicjalizuje przyciski Resume, Reset, Menu.
     */
    void init() override;

    /**
     * @brief Obsługuje kliknięcia w menu pauzy oraz klawisz ESC (wznowienie).
     * @param event Zdarzenie SFML.
     */
    void handleInput(sf::Event& event) override;

    /**
     * @brief Aktualizuje efekty wizualne przycisków.
     * @param dt Czas klatki.
     */
    void update(float dt) override;

    /**
     * @brief Rysuje zamrożone tło gry, przyciemnienie i menu pauzy.
     * @param dt Interpolacja.
     */
    void draw(float dt) override;

private:
    GameDataRef m_data;

    std::shared_ptr<sf::Texture> m_bgTexture; ///< Przechowywana tekstura tła.
    sf::Sprite m_backgroundSprite;            ///< Sprite tła (zamrożona gra).
    sf::RectangleShape m_dimmer;              ///< Półprzezroczysty prostokąt przyciemniający.

    sf::Text m_pauseTitle;
    sf::Text m_resumeButton; ///< Wznawia grę (dejmuje stan ze stosu).
    sf::Text m_resetButton;  ///< Resetuje poziom (podmienia stan gry).
    sf::Text m_menuButton;   ///< Wraca do menu głównego.

    sf::RectangleShape m_fadeRect;
    float m_alpha;
    bool m_isGoingToMenu;
    bool m_isReseting;
    int m_currentLevel;
};

#endif //MINI_GOLF_PAUSEMENUSTATE_H