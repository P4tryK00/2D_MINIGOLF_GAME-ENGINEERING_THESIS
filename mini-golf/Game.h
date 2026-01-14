#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "StateManager.h"
#include "DEFINITIONS.h"

struct GameData {
    StateManager machine;
    sf::RenderWindow window;
};

typedef std::shared_ptr<GameData> GameDataRef;

class Game {
public:
    Game(int width, int height, std::string title);
    void run();

private:
    const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
    sf::Clock clock;
    GameDataRef m_data = std::make_shared<GameData>();

};

#endif // GAME_H