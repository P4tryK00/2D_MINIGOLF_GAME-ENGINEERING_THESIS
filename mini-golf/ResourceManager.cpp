//
// Created by patry on 04.11.2025.
//

#include "ResourceManager.h"

void ResourceManager::loadAllResources() {
    // Tekstury
    TextureManager::load("ball", BALL_TEXTURE_PATH);
    TextureManager::load("hole", HOLE_TEXTURE_PATH);
    TextureManager::load("grass", GRASS_TEXTURE_PATH);
    // Czcionki

    // Dzwieki

}

void ResourceManager::cleanUpAllResources() {
    TextureManager::cleanup();
    FontManager::cleanup();
    SoundManager::cleanup();
}

