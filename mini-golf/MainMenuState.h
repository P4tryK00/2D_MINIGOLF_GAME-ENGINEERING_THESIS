//
// Created by patry on 13.01.2026.
//

#ifndef MINI_GOLF_MAINMENUSTATE_H
#define MINI_GOLF_MAINMENUSTATE_H


#include "State.h"
#include "Game.h"
#include "LevelSelectState.h"
#include "SaveManager.h"
#include "LeaderBoardState.h"

class MainMenuState : public State {
public:
    MainMenuState(GameDataRef data);

    void init() override;
    void handleInput(sf::Event& event) override;
    void update(float dt) override;
    void draw(float dt) override;

private:
    GameDataRef m_data;

    sf::Sprite m_background;

    sf::Text m_title;
    sf::Text m_playButton;
    sf::Text m_exitButton;
    sf::Text m_leaderboardButton;
    sf::RectangleShape m_fadeRect;
    float m_alpha;
    bool m_isTransitioning;
    bool m_isFadingIn;
    bool m_goToLeaderboard;
};


#endif //MINI_GOLF_MAINMENUSTATE_H