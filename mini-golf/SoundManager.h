//
// Created by patry on 04.11.2025.
//

#ifndef MINI_GOLF_SOUNDMANAGER_H
#define MINI_GOLF_SOUNDMANAGER_H
#include "SFML/Audio.hpp"
#include <map>
#include <string>
#include <iostream>

class SoundManager {
public:
    SoundManager() = delete;
    static void load(const std::string& key, const std::string& path);
    static const sf::SoundBuffer& get(const std::string&key);

    static void cleanup();

private:
    static std::map<std::string, sf::SoundBuffer> m_Sounds;
};


#endif //MINI_GOLF_SOUNDMANAGER_H