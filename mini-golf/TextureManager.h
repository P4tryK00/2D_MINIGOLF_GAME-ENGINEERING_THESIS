#ifndef MINI_GOLF_TEXTUREMANAGER_H
#define MINI_GOLF_TEXTUREMANAGER_H

#include <string>
#include <map>
#include "SFML/Graphics/Texture.hpp"
#include <iostream>

/**
 * @class TextureManager
 * @brief Statyczny menedżer zasobów dla tekstur SFML.
 *
 * Implementuje wzorzec Pyłek (Flyweight) poprzez przechowywanie załadowanych tekstur w mapie.
 * Zapobiega to wielokrotnemu ładowaniu tego samego zasobu z dysku, oszczędzając pamięć.
 */
class TextureManager {
public:
    TextureManager() = delete; // Klasa statyczna, brak możliwości instancjonowania

    /**
     * @brief Ładuje teksturę z pliku i zapisuje ją pod kluczem tekstowym.
     * @param key Unikalny identyfikator tekstury.
     * @param path Ścieżka do pliku graficznego.
     */
    static void load(const std::string& key, const std::string& path);

    /**
     * @brief Pobiera referencję do przechowywanej tekstury.
     * @param key Identyfikator tekstury do pobrania.
     * @return Stała referencja do sf::Texture.
     */
    static const sf::Texture& get(const std::string&key);

    /**
     * @brief Czyści wszystkie tekstury z pamięci.
     */
    static void cleanup();

private:
    static std::map<std::string, sf::Texture> m_Textures; ///< Kontener przechowujący tekstury.
};

#endif //MINI_GOLF_TEXTUREMANAGER_H