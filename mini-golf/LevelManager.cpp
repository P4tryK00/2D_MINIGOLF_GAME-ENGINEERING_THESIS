#include "LevelManager.h"

// Inicjalizacja pozycji startowej (wartość domyślna)
sf::Vector2f LevelManager::startPosition = sf::Vector2f(400.f, 300.f);

std::vector<int> LevelManager::loadLevelFromImage(int levelNumber) {
    std::vector<int> levelData;
    sf::Image mapImage;

    std::string path = "Resources/levels/level" + std::to_string(levelNumber) + ".png";

    if (!mapImage.loadFromFile(path)) {
        std::cerr << "BLAD: Nie mozna zaladowac mapy: " << path << std::endl;
        // Fallback: Zwróć pustą trawę, aby uniknąć crasha
        return std::vector<int>(25 * 19, 0);
    }

    // Iteracja po pikselach obrazu (oś Y, potem oś X - row-major order)
    for (unsigned int y = 0; y < mapImage.getSize().y; y++) {
        for (unsigned int x = 0; x < mapImage.getSize().x; x++) {
            sf::Color pixelColor = mapImage.getPixel(x, y);

            // --- WYKRYWANIE STARTU (Biały piksel) ---
            if (pixelColor == COLOR_BALL) {
                // 1. Obliczamy pozycję w świecie gry: (indeks * rozmiar_kafelka) + przesunięcie do środka
                startPosition = sf::Vector2f(x * 32.f + 16.f, y * 32.f + 16.f);

                // 2. Pod piłką kładziemy trawę (ID 0), żeby wizualnie było poprawnie
                levelData.push_back(0);
            }
            else {
                // Konwersja koloru na ID kafelka
                int tileID = colorToTileID(pixelColor);
                levelData.push_back(tileID);
            }
        }
    }

    return levelData;
}

int LevelManager::colorToTileID(sf::Color color) {
    // Prosta mapa kolorów na identyfikatory (zdefiniowane w DEFINITIONS.h)
    if (color == COLOR_GRASS )return 0;
    if (color == COLOR_HOLE)return 1;
    if (color == COLOR_SAND)return 2;
    if (color == COLOR_WALL_L)return 3;
    if (color == COLOR_WALL_R)return 4;
    if (color == COLOR_WALL_UP)return 5;
    if (color == COLOR_WALL_DOWN)return 6;
    if (color == COLOR_CORNER_LD)return 7;
    if (color == COLOR_CORNER_RD)return 8;
    if (color == COLOR_CORNER_LU)return 9;
    if (color == COLOR_CORNER_RU)return 10;
    if (color == COLOR_WATER)return 11;
    if (color == COLOR_CONCRETE)return 12;
    if (color == COLOR_RAMP_UP)return 13;
    if (color == COLOR_RAMP_DOWN)return 14;
    if (color == COLOR_RAMP_R)return 15;
    if (color == COLOR_RAMP_L)return 16;

    // Domyślna wartość (trawa) w przypadku nieznanego koloru
    return 0;
}