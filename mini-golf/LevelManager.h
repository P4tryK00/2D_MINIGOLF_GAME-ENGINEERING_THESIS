#ifndef MINI_GOLF_LEVELMANAGER_H
#define MINI_GOLF_LEVELMANAGER_H

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "DEFINITIONS.h"
#include <iostream>

/**
 * @class LevelManager
 * @brief Klasa statyczna odpowiedzialna za wczytywanie poziomów.
 *
 * Implementuje parsowanie plików graficznych (PNG) na strukturę mapy kafelkowej.
 * Mapuje kolory pikseli na identyfikatory kafelków (Tile ID).
 */
class LevelManager {
public:
    /**
     * @brief Wczytuje poziom z pliku obrazu.
     *
     * Iteruje po pikselach obrazu i na podstawie ich koloru generuje tablicę ID kafelków.
     * Wykrywa również pozycję startową gracza (biały piksel).
     *
     * @param levelNumber Numer poziomu do wczytania (np. 1 dla "level1.png").
     * @return Wektor liczb całkowitych reprezentujący mapę kafelków.
     */
    static std::vector<int> loadLevelFromImage(int levelNumber);

    /**
     * @brief Pozycja startowa gracza wykryta podczas ładowania ostatniego poziomu.
     */
    static sf::Vector2f startPosition;

private:
    /**
     * @brief Konwertuje kolor piksela na ID kafelka.
     * @param color Kolor piksela z mapy źródłowej.
     * @return ID kafelka zgodne z definicjami w DEFINITIONS.h.
     */
    static int colorToTileID(sf::Color color);
};

#endif //MINI_GOLF_LEVELMANAGER_H