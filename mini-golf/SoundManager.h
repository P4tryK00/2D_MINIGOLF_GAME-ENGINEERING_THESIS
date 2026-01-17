#ifndef MINI_GOLF_SOUNDMANAGER_H
#define MINI_GOLF_SOUNDMANAGER_H
#include "SFML/Audio.hpp"
#include <map>
#include <string>
#include <iostream>

/**
 * @class SoundManager
 * @brief Statyczny menedżer zasobów dla buforów dźwiękowych.
 *
 * Zarządza obiektami sf::SoundBuffer, aby zapewnić jednokrotne ładowanie
 * danych audio do pamięci.
 */
class SoundManager {
public:
    SoundManager() = delete;

    /**
     * @brief Ładuje plik dźwiękowy do pamięci.
     * @param key Unikalny identyfikator dźwięku.
     * @param path Ścieżka do pliku audio (.wav, .ogg itp.).
     */
    static void load(const std::string& key, const std::string& path);

    /**
     * @brief Pobiera załadowany bufor dźwiękowy.
     * @param key Unikalny identyfikator.
     * @return Stała referencja do sf::SoundBuffer.
     */
    static const sf::SoundBuffer& get(const std::string&key);

    /**
     * @brief Czyści wszystkie bufory dźwiękowe.
     */
    static void cleanup();

private:
    static std::map<std::string, sf::SoundBuffer> m_Sounds;
};

#endif //MINI_GOLF_SOUNDMANAGER_H