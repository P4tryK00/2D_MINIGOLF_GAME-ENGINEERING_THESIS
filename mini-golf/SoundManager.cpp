#include "SoundManager.h"

std::map<std::string, sf::SoundBuffer> SoundManager::m_Sounds;

void SoundManager::load(const std::string& key, const std::string& path) {
    sf::SoundBuffer sound;
    if (sound.loadFromFile(path)) {
        m_Sounds[key] = sound;
    } else {
        std::cerr << "Error: Could not load sound from " << path << std::endl;
    }
}

const sf::SoundBuffer& SoundManager::get(const std::string& key) {
    return m_Sounds.at(key);
}

void SoundManager::cleanup() {
    m_Sounds.clear();
}