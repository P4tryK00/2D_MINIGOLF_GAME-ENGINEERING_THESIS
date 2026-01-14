#include "Game.h"
#include "ResourceManager.h"



int main()
{
    ResourceManager::loadAllResources();
    Game game(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    game.run();
    ResourceManager::cleanUpAllResources();
    return 0;
}
