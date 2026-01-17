//
// Created by patry on 17.01.2026.
//

#ifndef MINI_GOLF_LEADERBOARDSTATE_H
#define MINI_GOLF_LEADERBOARDSTATE_H


#include "State.h"
#include "Game.h"
#include <vector>

class LeaderboardState : public State {
public:
    LeaderboardState(GameDataRef data);

    void init() override;
    void handleInput(sf::Event& event) override;
    void update(float dt) override;
    void draw(float dt) override;

private:
    GameDataRef m_data;
    sf::Text m_title;
    sf::Text m_backButton;
    std::vector<sf::Text> m_scoreTexts;

    sf::RectangleShape m_fadeRect;
    float m_alpha;
    bool m_isFadingIn;
    bool m_isGoingBack;
};


#endif //MINI_GOLF_LEADERBOARDSTATE_H