#ifndef MINI_GOLF_DEFINITIONS_H
#define MINI_GOLF_DEFINITIONS_H

/**
 * @file DEFINITIONS.h
 * @brief Plik nagłówkowy zawierający globalne definicje, stałe oraz makra używane w projekcie.
 *
 * Plik ten przechowuje ścieżki do zasobów, parametry okna oraz definicje kolorów
 * używanych do mapowania poziomów z plików graficznych.
 */

#include "SFML/Graphics.hpp"

/** @name Ustawienia Okna
 * Podstawowe parametry okna aplikacji.
 */
///@{
#define SCREEN_WIDTH 600  ///< Szerokość okna gry w pikselach.
#define SCREEN_HEIGHT 800 ///< Wysokość okna gry w pikselach.
#define TITLE "Mini Golf by Patryk Jozwiak" ///< Tytuł wyświetlany na belce okna.
///@}

/** @name Ścieżki do Tekstur
 * Lokalizacje plików graficznych.
 */
///@{
#define BALL_TEXTURE_PATH "Resources/textures/ball.png"
#define TILESET_PATH "Resources/textures/tileset.png"
#define HEART_WHITE_PATH "Resources/textures/heart_white.png"
#define HEART_PATH "Resources/textures/heart.png"
///@}

/** @name Ścieżki do Czcionek */
///@{
#define FONT_PATH "Resources/font/ByteBounce.ttf"
///@}

/** @name Ścieżki do Dźwięków
 * Lokalizacje efektów dźwiękowych.
 */
///@{
#define WIN_PATH "Resources/sounds/win.wav"
#define HIT_PATH "Resources/sounds/hit.wav"
#define SPLASH_PATH "Resources/sounds/splash.wav"
#define WALL_HIT_PATH "Resources/sounds/wall_hit.wav"
///@}

/** @name Konfiguracja Poziomów
 * Stałe dotyczące siatki i kolorów używanych w LevelManagerze.
 */
///@{
#define LEVEL_WIDTH  19   ///< Szerokość poziomu w kafelkach.
#define LEVEL_HEIGHT  25  ///< Wysokość poziomu w kafelkach.
#define TILE_SIZE sf::Vector2u(32, 32) ///< Rozmiar pojedynczego kafelka w pikselach.

// Definicje kolorów do parsowania mapy z obrazka
const sf::Color COLOR_BALL (255,255,255);     ///< Kolor oznaczający pozycję startową piłki.
const sf::Color COLOR_GRASS(0, 255, 0);       ///< Trawa (ID 0).
const sf::Color COLOR_HOLE(255, 0, 0);        ///< Dołek (ID 1).
const sf::Color COLOR_SAND(255, 255, 0);      ///< Piasek (ID 2).
const sf::Color COLOR_WALL_L(0,0,0);          ///< Ściana lewa (ID 3).
const sf::Color COLOR_WALL_R(2, 2, 2);        ///< Ściana prawa (ID 4).
const sf::Color COLOR_WALL_UP(1, 1, 1);       ///< Ściana górna (ID 5).
const sf::Color COLOR_WALL_DOWN(3, 3, 3);     ///< Ściana dolna (ID 6).
const sf::Color COLOR_CORNER_LD(7, 7, 7);     ///< Narożnik lewy-dół (ID 7).
const sf::Color COLOR_CORNER_RD( 6, 6, 6 );   ///< Narożnik prawy-dół (ID 8).
const sf::Color COLOR_CORNER_LU( 4, 4, 4);    ///< Narożnik lewy-góra (ID 9).
const sf::Color COLOR_CORNER_RU( 5, 5, 5);    ///< Narożnik prawy-góra (ID 10).
const sf::Color COLOR_WATER(0, 0, 255);       ///< Woda (ID 11).
const sf::Color COLOR_CONCRETE(128, 128, 128);///< Beton (ID 12).
const sf::Color COLOR_RAMP_UP(25, 25, 25);    ///< Rampa w górę (ID 13).
const sf::Color COLOR_RAMP_DOWN( 50, 50, 50); ///< Rampa w dół (ID 14).
const sf::Color COLOR_RAMP_R( 100, 100, 100); ///< Rampa w prawo (ID 15).
const sf::Color COLOR_RAMP_L( 150, 150, 150); ///< Rampa w lewo (ID 16).
const sf::Color COLOR_RAMP_TEST(200, 200, 200);///< Rampa testowa (ID 17).
const sf::Color COLOR_RAMP_TEST2(220, 220, 220);///< Rampa testowa 2 (ID 18).
const sf::Color COLOR_RAMP_UP_TEST1(240, 240, 240);///< Rampa w górę testowa (ID 19).
const sf::Color COLOR_RAMP_UP_TEST2(230, 230, 230);///< Rampa w górę testowa 2 (ID 20).
const sf::Color COLOR_RAMP_UP_TEST3(210, 210, 210);///< Rampa w górę testowa 3 (ID 21).

///@}

#endif //MINI_GOLF_DEFINITIONS_H