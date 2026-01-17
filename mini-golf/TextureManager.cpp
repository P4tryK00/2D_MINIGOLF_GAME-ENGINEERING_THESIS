#include "TextureManager.h"

// Definicja statycznego pola
std::map<std::string, sf::Texture> TextureManager::m_Textures;

void TextureManager::load(const std::string& key, const std::string& path) {
    sf::Texture texture;
    if (texture.loadFromFile(path)) {
        texture.setSmooth(true);
        m_Textures[key] = texture;
    } else {
        std::cerr << "Error: Could not load texture from " << path << std::endl;
    }
}

const sf::Texture& TextureManager::get(const std::string& key) {
    // Zwraca referencję lub rzuca wyjątek, jeśli klucz nie istnieje
    return m_Textures.at(key);
}

void TextureManager::cleanup() {
    m_Textures.clear();
}