#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "InputManager.h"
#include "DEFINITIONS.h"
#include "ResourceManager.h" // Ważne dla czcionek
#include <iostream>
#include "TileMap.h"
#include <vector>
#include <cmath>

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow mWindow;
    Ball ball;
    InputManager inputManager;
    sf::Vector2f forceVector;

    // UI
    sf::Text scoreText;
    sf::Text winText;
    int strokes;
    bool ballInHole;

    // Mapa
    TileMap tileMap;
    std::vector<int> levelData;
    sf::Vector2f lastSafePos;

    // Metody
    void initUI();
    void processEvents();
    void update(sf::Time dt);
    void render();
    int getTileAt(sf::Vector2f position);
    bool isWall(int tileId);

    sf::RectangleShape aimLine;
    void updateAimLine();

    const static sf::Time timePerFrame;
};

#endif // GAME_H