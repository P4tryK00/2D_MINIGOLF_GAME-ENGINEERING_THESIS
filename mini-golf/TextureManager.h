//
// Created by patry on 29.10.2025.
//

#ifndef MINI_GOLF_TEXTUREMANAGER_H
#define MINI_GOLF_TEXTUREMANAGER_H
#include <string>
#include <map>
#include "SFML/Graphics/Texture.hpp"
#include <iostream>
class TextureManager {
    public:
    TextureManager() = delete;
    static void load(const std::string& key, const std::string& path);
    static const sf::Texture& get(const std::string&key);

    static void cleanup();

    private:
    static std::map<std::string, sf::Texture> m_Textures;

};


#endif //MINI_GOLF_TEXTUREMANAGER_H