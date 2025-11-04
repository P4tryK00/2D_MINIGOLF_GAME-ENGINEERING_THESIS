//
// Created by patry on 04.11.2025.
//

#ifndef MINI_GOLF_FONTMANAGER_H
#define MINI_GOLF_FONTMANAGER_H
#include <string>
#include <map>
#include "SFML/Graphics/Font.hpp"
#include <iostream>

class FontManager {
public:
    FontManager() = delete;
    static void load(const std::string& key, const std::string& path);
    static const sf::Font& get(const std::string&key);

    static void cleanup();

private:
    static std::map<std::string, sf::Font> m_Font;
};


#endif //MINI_GOLF_FONTMANAGER_H