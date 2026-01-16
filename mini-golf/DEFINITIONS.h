//
// Created by patry on 11.09.2025.
//

#ifndef MINI_GOLF_DEFINITIONS_H
#define MINI_GOLF_DEFINITIONS_H

#include "SFML/Graphics.hpp"
// Wymiary okna
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800

//Tytul
#define TITLE "Mini Golf by Patryk Jozwiak"

// Tekstury
#define BALL_TEXTURE_PATH "Resources/textures/ball.png"
#define TILESET_PATH "Resources/textures/tileset.png"
#define HEART_WHITE_PATH "Resources/textures/heart_white.png"
#define HEART_PATH "Resources/textures/heart.png"
// Czcionki
#define FONT_PATH "Resources/font/ByteBounce.ttf"

// Muzyka / dzwieki
#define WIN_PATH "Resources/sounds/win.wav"
#define HIT_PATH "Resources/sounds/hit.wav"
#define SPLASH_PATH "Resources/sounds/splash.wav"
#define WALL_HIT_PATH "Resources/sounds/wall_hit.wav"


//Levels
#define  LEVEL_WIDTH  19
#define LEVEL_HEIGHT  25
#define TILE_SIZE sf::Vector2u(32, 32)

#endif //MINI_GOLF_DEFINITIONS_H