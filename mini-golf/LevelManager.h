//
// Created by patry on 16.01.2026.
//

#ifndef MINI_GOLF_LEVELMANAGER_H
#define MINI_GOLF_LEVELMANAGER_H


#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "DEFINITIONS.h"
#include <iostream>

class LevelManager {
    public:
    static std::vector<int> loadLevelFromImage(int levelNumber);
    static sf::Vector2f startPosition;

    private:
    static int colorToTileID(sf::Color color);

};

#endif //MINI_GOLF_LEVELMANAGER_H