#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "InputManager.h"
#include "DEFINITIONS.h"
#include <iostream>


class Game {

    public:
    Game();
    void run();

    private:
        sf::RenderWindow mWindow;
        Ball ball;
        InputManager inputManager;
        sf::Vector2f forceVector;
        sf::Texture backgroundTexture;
        sf::RectangleShape background;

    private:
        void processEvents();
        void update(sf::Time dt);
        void render();
        bool isReady;
        const static sf::Time timePerFrame;
};

#endif // GAME_H
