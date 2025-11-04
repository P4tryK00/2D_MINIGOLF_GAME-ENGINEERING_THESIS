//
// Created by patry on 04.11.2025.
//

#include "FontManager.h"

std::map<std::string, sf::Font> FontManager::m_Font;

void FontManager::load(const std::string& key, const std::string& path) {
    sf::Font font;
    if (font.loadFromFile(path)) {
        m_Font[key] = font;
    } else {
        std::cerr << "Error: Could not load font from " << path << std::endl;
    }
}

const sf::Font& FontManager::get(const std::string& key) {
    return m_Font.at(key);
}

void FontManager::cleanup() {
    m_Font.clear();
}