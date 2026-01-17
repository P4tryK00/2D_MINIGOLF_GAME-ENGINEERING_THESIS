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
const sf::Color COLOR_BALL (255,255,255);     // BALL
const sf::Color COLOR_GRASS(0, 255, 0);       // ID 0
const sf::Color COLOR_HOLE(255, 0, 0);        // ID 1
const sf::Color COLOR_SAND(255, 255, 0);      // ID 2
const sf::Color COLOR_WALL_L(0,0,0);          // ID 3
const sf::Color COLOR_WALL_R(2, 2, 2);        // ID 4
const sf::Color COLOR_WALL_UP(1, 1, 1);       // ID 5
const sf::Color COLOR_WALL_DOWN(3, 3, 3);     // ID 6
const sf::Color COLOR_CORNER_LD(7, 7, 7);     // ID 7
const sf::Color COLOR_CORNER_RD( 6, 6, 6 );   // ID 8
const sf::Color COLOR_CORNER_LU( 4, 4, 4);    // ID 9
const sf::Color COLOR_CORNER_RU( 5, 5, 5);    // ID 10
const sf::Color COLOR_WATER(0, 0, 255);       // ID 11
const sf::Color COLOR_CONCRETE(128, 128, 128);// ID 12
const sf::Color COLOR_RAMP_UP(25, 25, 25);    // ID 13
const sf::Color COLOR_RAMP_DOWN( 50, 50, 50); // ID 14
const sf::Color COLOR_RAMP_R( 100, 100, 100); // ID 15
const sf::Color COLOR_RAMP_L( 150, 150, 150); // ID 16


#endif //MINI_GOLF_DEFINITIONS_H