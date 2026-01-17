#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "DEFINITIONS.h"
#include "ResourceManager.h"
#include <cmath>

/**
 * @class Ball
 * @brief Klasa reprezentująca piłkę golfową (obiekt gracza).
 *
 * Obsługuje fizykę ruchu, kolizje (odbicia), renderowanie oraz
 * logikę interakcji z otoczeniem.
 */
class Ball {
private:
    sf::Sprite player;    ///< Sprite graficzny piłki.
    sf::Vector2f velocity; ///< Aktualny wektor prędkości piłki.

public:
    /**
     * @brief Domyślny konstruktor. Inicjalizuje teksturę i origin piłki.
     */
    Ball();

    /**
     * @brief Aktualizuje pozycję piłki na podstawie prędkości i czasu.
     * * @param dt Czas, który upłynął od ostatniej klatki (Delta Time).
     * @param friction Współczynnik tarcia (zwalnia piłkę w każdej klatce).
     */
    void update(float dt, float friction);

    /**
     * @brief Rysuje piłkę w oknie gry.
     * @param window Referencja do okna renderowania.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Nadaje piłce natychmiastowy wektor prędkości (uderzenie).
     * @param impulse Wektor siły uderzenia.
     */
    void applyImpulse(const sf::Vector2f& impulse);

    /**
     * @brief Sprawdza, czy dany punkt (np. myszka) znajduje się w obrębie piłki.
     * @param point Współrzędne punktu.
     * @return true Jeśli punkt zawiera się w granicach sprite'a.
     */
    bool contains(const sf::Vector2f& point) const;

    /**
     * @brief Pobiera aktualną pozycję piłki.
     * @return Wektor pozycji (x, y).
     */
    sf::Vector2f getPosition() const;

    /**
     * @brief Ustawia piłkę w konkretnym miejscu (np. na starcie poziomu).
     * @param pos Nowa pozycja.
     */
    void setPosition(const sf::Vector2f& pos);

    /**
     * @brief Całkowicie zatrzymuje piłkę (zeruje prędkość).
     */
    void stop();

    /**
     * @brief Pobiera aktualną skalę obiektu.
     * @return Wektor skali (x, y).
     */
    sf::Vector2f getScale() const { return player.getScale(); }

    /**
     * @brief Obsługuje fizykę odbicia od pionowej ściany (oś X).
     * Odwraca wektor X i redukuje prędkość (symulacja utraty energii).
     */
    void bounceX();

    /**
     * @brief Obsługuje fizykę odbicia od poziomej ściany (oś Y).
     * Odwraca wektor Y i redukuje prędkość.
     */
    void bounceY();

    /**
     * @brief Odwraca całkowicie wektor prędkości.
     */
    void invertVelocity();

    /**
     * @brief Pobiera aktualny wektor prędkości.
     * @return Wektor prędkości (vx, vy).
     */
    sf::Vector2f getVelocity() const;

    /**
     * @brief Ustawia skalę sprite'a (używane np. przy animacji skoku lub wpadania do wody).
     * @param factor Współczynnik skali (1.0 = normalny rozmiar).
     */
    void setScale(float factor);
};

#endif // BALL_H