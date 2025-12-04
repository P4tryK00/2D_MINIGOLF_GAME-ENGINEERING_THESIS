#include "TileMap.h"
#include "TextureManager.h"

bool TileMap::load(const std::string& textureKey, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
{
    m_tileset = &TextureManager::get(textureKey);


    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);


    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // Pobieramy numer kafelka z tablicy
            int tileNumber = tiles[i + j * width];

            // Znajdujemy jego pozycję w teksturze tilesetu
            // UWAGA: Używamy strzałki '->', bo m_tileset jest teraz wskaźnikiem!
            int tu = tileNumber % (m_tileset->getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset->getSize().x / tileSize.x);

            // Wskaźnik do obecnego kafelka (4 wierzchołków)
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            // DEFINIUJEMY 4 ROGI KAFELKA NA EKRANIE
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            // DEFINIUJEMY 4 ROGI TEKSTURY
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = m_tileset;
    target.draw(m_vertices, states);
}