//
// Created by patry on 16.01.2026.
//

#ifndef MINI_GOLF_LEVELCOMPLETESTATE_H
#define MINI_GOLF_LEVELCOMPLETESTATE_H


#include "State.h"
#include "Game.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "ResourceManager.h"
#include "DEFINITIONS.h"
#include <memory>

class LevelCompleteState : public State {
public:
    LevelCompleteState(GameDataRef data, sf::Sprite bgSprite, std::shared_ptr<sf::Texture> bgTexture, int strokes, int currentLevel);

    void init() override;
    void handleInput(sf::Event& event) override;
    void update(float dt) override;
    void draw(float dt) override;

private:
    GameDataRef m_data;
    sf::Sprite m_backgroundSprite;
    std::shared_ptr<sf::Texture> m_bgTexture;

    int m_strokes;
    int m_currentLevel;

    sf::RectangleShape m_dimmer;

    sf::Text m_title;
    sf::Text m_scoreText;

    sf::Text m_nextLevelButton;
    sf::Text m_menuButton;
    sf::Text m_retryButton;

    // Zmienne do płynnego wejścia
    sf::RectangleShape m_fadeRect;
    float m_alpha;
    bool m_isTransitioning;
    bool m_goToMenu;
    bool m_retryLevel;
};


#endif //MINI_GOLF_LEVELCOMPLETESTATE_H