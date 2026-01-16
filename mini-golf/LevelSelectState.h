//
// Created by patry on 16.01.2026.
//

#ifndef MINI_GOLF_LEVELSELECTSTATE_H
#define MINI_GOLF_LEVELSELECTSTATE_H

#include "State.h"
#include "Game.h"
#include <vector>
#include "GameState.h"
#include "MainMenuState.h"
#include "ResourceManager.h"
#include "DEFINITIONS.h"

class LevelSelectState : public State {
public:
    LevelSelectState(GameDataRef data);

    void init() override;
    void handleInput(sf::Event& event) override;
    void update(float dt) override;
    void draw(float dt) override;

private:
    GameDataRef m_data;

    sf::Text m_title;
    sf::Text m_backButton;

    std::vector<sf::Text> m_levelButtons;

    // Zmienne do przejścia (Fader)
    sf::RectangleShape m_fadeRect;
    float m_alpha;
    bool m_isFadingIn;
    bool m_isTransitioning;
    int m_nextLevelToLoad;
};


#endif //MINI_GOLF_LEVELSELECTSTATE_H