#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

/**
 * @class TileMap
 * @brief Klasa odpowiedzialna za renderowanie poziomu gry przy użyciu Vertex Array.
 *
 * Klasa ta optymalizuje wyświetlanie mapy poprzez rysowanie całego poziomu jako jednego
 * obiektu (jedno wywołanie draw) przy użyciu sf::VertexArray. Jest to znacznie wydajniejsze
 * niż rysowanie oddzielnych sprite'ów dla każdego kafelka.
 */
class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    /**
     * @brief Wczytuje dane mapy i generuje tablicę wierzchołków.
     * @param tileset Ścieżka do pliku tekstury (tileset) lub klucz w ResourceManagerze.
     * @param tileSize Wymiary pojedynczego kafelka (np. 32x32).
     * @param tiles Wektor zawierający identyfikatory kafelków (indeksy w tilesecie).
     * @param width Szerokość mapy w kafelkach.
     * @param height Wysokość mapy w kafelkach.
     * @return true jeśli wczytywanie się powiodło, false w przeciwnym razie.
     */
    bool load(const std::string& tileset, sf::Vector2u tileSize, std::vector<int>tiles, unsigned int width, unsigned int height);

private:
    /**
     * @brief Nadpisana metoda draw z sf::Drawable.
     * Obsługuje renderowanie tablicy wierzchołków z przypisaną teksturą.
     *
     * @param target Cel renderowania (okno).
     * @param states Stany renderowania (transformacja, tekstura).
     */
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices; ///< Kolekcja wierzchołków reprezentująca kafelki (Quads).
    const sf::Texture* m_tileset; ///< Wskaźnik do tekstury źródłowej (tileset).
};

#endif // TILEMAP_H