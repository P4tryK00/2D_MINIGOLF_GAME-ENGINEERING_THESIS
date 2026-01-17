#ifndef MINI_GOLF_STATE_H
#define MINI_GOLF_STATE_H

#include <SFML/Graphics.hpp>

/**
 * @class State
 * @brief Abstrakcyjna klasa bazowa reprezentująca pojedynczy stan gry (np. Menu, Gra, Pauza).
 *
 * Klasa ta definiuje interfejs, który muszą zaimplementować wszystkie konkretne stany gry.
 * Wykorzystuje wzorzec projektowy "Stan" (State Pattern), pozwalając StateManagerowi
 * na jednolite zarządzanie różnymi etapami rozgrywki.
 */
class State {
public:
    /**
     * @brief Wirtualny destruktor.
     */
    virtual ~State() {}

    /**
     * @brief Inicjalizuje zasoby i zmienne stanu.
     * Wywoływana raz, gdy stan jest dodawany do StateManagera.
     */
    virtual void init() = 0;

    /**
     * @brief Obsługuje wejście od użytkownika (klawiatura, mysz, zdarzenia okna).
     * @param event Zdarzenie SFML do przetworzenia.
     */
    virtual void handleInput(sf::Event& event) = 0;

    /**
     * @brief Aktualizuje logikę stanu (fizyka, animacje, reguły gry).
     * @param dt Czas, który upłynął od ostatniej klatki (Delta Time) w sekundach.
     */
    virtual void update(float dt) = 0;

    /**
     * @brief Renderuje grafikę stanu na ekranie.
     * @param dt Współczynnik interpolacji (używany do płynnego renderowania między krokami fizyki).
     */
    virtual void draw(float dt) = 0;

    /**
     * @brief Wywoływana, gdy stan jest pauzowany (np. inny stan jest nakładany na wierzch).
     */
    virtual void pause() {}

    /**
     * @brief Wywoływana, gdy stan jest wznawiany (np. stan powyżej został zdjęty ze stosu).
     */
    virtual void resume() {}
};

#endif //MINI_GOLF_STATE_H