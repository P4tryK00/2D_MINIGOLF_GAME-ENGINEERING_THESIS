//
// Created by patry on 17.01.2026.
//

#ifndef MINI_GOLF_GAMEOVERTSTATE_H
#define MINI_GOLF_GAMEOVERTSTATE_H



#include "State.h"
#include "Game.h"
#include <memory>

class GameOverState : public State {
public:
    // Potrzebujemy numeru poziomu, żeby wiedzieć co zresetować
    GameOverState(GameDataRef data, sf::Sprite bgSprite, std::shared_ptr<sf::Texture> bgTexture, int level);

    void init() override;
    void handleInput(sf::Event& event) override;
    void update(float dt) override;
    void draw(float dt) override;

private:
    GameDataRef m_data;
    sf::Sprite m_backgroundSprite;
    std::shared_ptr<sf::Texture> m_bgTexture;
    int m_level;

    sf::RectangleShape m_dimmer;

    sf::Text m_title;
    sf::Text m_retryButton;
    sf::Text m_menuButton;

    // Fader
    sf::RectangleShape m_fadeRect;
    float m_alpha;
    bool m_isTransitioning;
    bool m_goToMenu;
    bool m_retryLevel;
};



#endif //MINI_GOLF_GAMEOVERTSTATE_H