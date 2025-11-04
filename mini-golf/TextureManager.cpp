//
// Created by patry on 29.10.2025.
//

#include "TextureManager.h"


std::map<std::string, sf::Texture> TextureManager::m_Textures;

void TextureManager::load(const std::string& key, const std::string& path) {
    sf::Texture texture;
    if (texture.loadFromFile(path)) {
        m_Textures[key] = texture;
    } else {
        std::cerr << "Error: Could not load texture from " << path << std::endl;
    }
}

const sf::Texture& TextureManager::get(const std::string& key) {
    return m_Textures.at(key);
}

void TextureManager::cleanup() {
    m_Textures.clear();
}