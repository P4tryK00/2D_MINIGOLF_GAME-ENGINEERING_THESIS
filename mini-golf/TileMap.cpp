/**
 * @file TileMap.cpp
 * @brief Implementacja renderowania mapy kafelkowej z użyciem sf::VertexArray.
 *
 * Uwaga o źródle inspiracji:
 * Fragment dotyczący budowy tablicy wierzchołków (quady + współrzędne tekstury) jest
 * inspirowany oficjalnym poradnikiem SFML „Vertex Arrays / TileMap”.
 * Kod został dostosowany do architektury projektu (TextureManager, format mapy, rozmiary).
 *
 * Źródło: https://www.sfml-dev.org/tutorials/2.6/graphics-vertex-array.php
 */


#include "TileMap.h"
#include "TextureManager.h"

bool TileMap::load(const std::string& textureKey, sf::Vector2u tileSize, std::vector<int>tiles, unsigned int width, unsigned int height)
{
    // Pobranie tekstury z menedżera
    m_tileset = &TextureManager::get(textureKey);

    // Konfiguracja tablicy wierzchołków na typ Quads (4 wierzchołki na kafelek)
    m_vertices.setPrimitiveType(sf::Quads);
    // Rezerwacja pamięci: szerokość * wysokość * 4 wierzchołki
    m_vertices.resize(width * height * 4);

    // Iteracja po siatce mapy w celu ustawienia wierzchołków
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // Pobranie numeru kafelka z danych mapy
            int tileNumber = tiles[i + j * width];

            // Obliczenie pozycji kafelka w teksturze tilesetu (współrzędne UV)
            int tu = tileNumber % (m_tileset->getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset->getSize().x / tileSize.x);

            // Pobranie wskaźnika do obecnego czworokąta (Quad) w tablicy wierzchołków
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            // Definicja 4 rogów kafelka na ekranie (Pozycja)
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            // Definicja 4 rogów kafelka w teksturze (Współrzędne Tekstury)
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Aplikacja transformacji obiektu (pozycja, rotacja, skala)
    states.transform *= getTransform();

    // Przypisanie tekstury tilesetu
    states.texture = m_tileset;

    // Rysowanie całej tablicy wierzchołków w jednym wywołaniu (Draw Call)
    target.draw(m_vertices, states);
}