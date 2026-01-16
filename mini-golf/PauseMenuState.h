//
// Created by patry on 14.01.2026.
//

#ifndef MINI_GOLF_PAUSEMENUSTATE_H
#define MINI_GOLF_PAUSEMENUSTATE_H


#include "State.h"
#include "Game.h"
#include <memory> // Dla shared_ptr

class PauseMenuState : public State {
public:
    // Przyjmujemy Sprite ORAZ wskaźnik do tekstury, żeby ją utrzymać przy życiu
    PauseMenuState(GameDataRef data, sf::Sprite bgSprite, std::shared_ptr<sf::Texture> bgTexture);

    void init() override;
    void handleInput(sf::Event& event) override;
    void update(float dt) override;
    void draw(float dt) override;

private:
    GameDataRef m_data;

    std::shared_ptr<sf::Texture> m_bgTexture; // Przechowujemy teksturę
    sf::Sprite m_backgroundSprite;
    sf::RectangleShape m_dimmer;

    sf::Text m_pauseTitle;
    sf::Text m_resumeButton;
    sf::Text m_resetButton;
    sf::Text m_menuButton;
    sf::RectangleShape m_fadeRect;
    float m_alpha;
    bool m_isGoingToMenu;
    bool m_isReseting;
};

#endif //MINI_GOLF_PAUSEMENUSTATE_H