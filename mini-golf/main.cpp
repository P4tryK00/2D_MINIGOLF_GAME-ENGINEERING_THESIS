#include "Game.h"
#include "ResourceManager.h"



int main()
{
    ResourceManager::loadAllResources();
    Game game;
    game.run();
    ResourceManager::cleanUpAllResources();
    return 0;
}
