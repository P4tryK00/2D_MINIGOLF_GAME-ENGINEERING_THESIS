//
// Created by patry on 04.11.2025.
//

#include "ResourceManager.h"

void ResourceManager::loadAllResources() {
    // Tekstury
    TextureManager::load("ball", BALL_TEXTURE_PATH);
    TextureManager::load("tileset", TILESET_PATH);
    // Czcionki
    FontManager::load("font", FONT_PATH);
    // Dzwieki
    SoundManager::load("win", WIN_PATH);
    SoundManager::load("hit", HIT_PATH);
    SoundManager::load("splash", SPLASH_PATH);
    SoundManager::load ("wall_hit", WALL_HIT_PATH);
}

void ResourceManager::cleanUpAllResources() {
    TextureManager::cleanup();
    FontManager::cleanup();
    SoundManager::cleanup();
}

