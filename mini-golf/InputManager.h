#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SFML/Graphics.hpp>
#include "Ball.h"

/**
 * @class InputManager
 * @brief Klasa obsługująca sterowanie (mechanika Drag & Shoot).
 *
 * Odpowiada za wykrywanie kliknięć na piłce, przeciąganie myszką
 * (celowanie) oraz obliczanie wektora siły uderzenia.
 */
class InputManager {
private:
    bool isGrabbed;             ///< Czy gracz trzyma piłkę (lewy przycisk myszy wciśnięty).
    bool ready;                 ///< Czy strzał jest gotowy do wykonania (puszczenie przycisku).
    sf::Vector2f ballStartPos;  ///< Pozycja piłki w momencie rozpoczęcia celowania.
    sf::Vector2f currentMousePos; ///< Aktualna pozycja myszy.
    float force;                ///< Obliczona siła uderzenia (nieużywana bezpośrednio, obliczana w wektorze).

public:
    /**
     * @brief Konstruktor inicjalizujący stan wejścia.
     */
    InputManager();

    /**
     * @brief Przetwarza zdarzenia wejścia SFML.
     *
     * Obsługuje MouseButtonPressed (chwycenie), MouseMoved (celowanie)
     * i MouseButtonReleased (strzał).
     *
     * @param event Zdarzenie SFML.
     * @param ball Referencja do obiektu piłki (do sprawdzenia czy kliknięto w nią).
     * @param window Referencja do okna (do mapowania współrzędnych).
     */
    void handleEvent(const sf::Event& event, Ball& ball, sf::RenderWindow& window);

    /**
     * @brief Sprawdza, czy gracz wykonał strzał (puścił przycisk myszy).
     * @return true jeśli strzał jest gotowy.
     */
    bool isReady() const;

    /**
     * @brief Oblicza i zwraca wektor siły uderzenia.
     *
     * Wektor jest obliczany na podstawie różnicy między pozycją startową a pozycją myszy.
     * Siła jest limitowana do maksymalnej wartości.
     * Po pobraniu wektora flaga 'ready' jest resetowana.
     *
     * @return Wektor siły (sf::Vector2f).
     */
    sf::Vector2f getForceVector();

    /**
     * @brief Oblicza aktualny wektor siły (do wizualizacji linii celowania).
     * Nie resetuje stanu 'ready'.
     *
     * @param window Okno gry.
     * @return Wektor siły.
     */
    sf::Vector2f getCurrentForce(const sf::RenderWindow& window) const;

    /**
     * @brief Sprawdza, czy gracz jest w trakcie celowania (trzyma przycisk).
     * @return true jeśli trwa celowanie.
     */
    bool isDragging() const;
};

#endif // INPUTMANAGER_H