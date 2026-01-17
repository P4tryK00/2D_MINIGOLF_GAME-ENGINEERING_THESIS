#include "Game.h"
#include "ResourceManager.h"

int main()
{
    // 1. Ładowanie zasobów globalnych
    ResourceManager::loadAllResources();

    // 2. Inicjalizacja silnika gry
    Game game(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);

    // 3. Uruchomienie głównej pętli
    game.run();

    // 4. Sprzątanie
    ResourceManager::cleanUpAllResources();

    return 0;
}