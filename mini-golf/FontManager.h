//
// Created by patry on 04.11.2025.
//

#ifndef MINI_GOLF_FONTMANAGER_H
#define MINI_GOLF_FONTMANAGER_H

#include <string>
#include <map>
#include "SFML/Graphics/Font.hpp"
#include <iostream>

/**
 * @class FontManager
 * @brief Klasa statyczna zarządzająca zasobami czcionek w grze.
 *
 * Odpowiada za ładowanie, przechowywanie i udostępnianie obiektów sf::Font.
 * Zapobiega wielokrotnemu ładowaniu tych samych plików z dysku.
 */
class FontManager {
public:
    /**
     * @brief Konstruktor usunięty - klasa jest w pełni statyczna.
     */
    FontManager() = delete;

    /**
     * @brief Ładuje czcionkę z pliku i przypisuje ją do klucza.
     * * @param key Unikalny identyfikator (nazwa) czcionki, np. "MainFont".
     * @param path Ścieżka do pliku czcionki (np. .ttf).
     */
    static void load(const std::string& key, const std::string& path);

    /**
     * @brief Pobiera załadowaną wcześniej czcionkę.
     * * @param key Klucz pod którym czcionka została zapisana.
     * @return Stała referencja do obiektu sf::Font.
     * @throw std::out_of_range Jeśli klucz nie istnieje w mapie.
     */
    static const sf::Font& get(const std::string& key);

    /**
     * @brief Usuwa wszystkie załadowane czcionki i zwalnia pamięć.
     */
    static void cleanup();

private:
    /**
     * @brief Mapa przechowująca pary: nazwa czcionki -> obiekt czcionki.
     */
    static std::map<std::string, sf::Font> m_Font;
};

#endif //MINI_GOLF_FONTMANAGER_H