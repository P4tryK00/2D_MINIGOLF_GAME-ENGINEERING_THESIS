#ifndef MINI_GOLF_GAMESTATE_H
#define MINI_GOLF_GAMESTATE_H

#include "State.h"
#include "Game.h"
#include "Ball.h"
#include "TileMap.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "PauseMenuState.h"
#include <SFML/Audio.hpp>
#include <numbers>
#include "LevelManager.h"
#include "LevelCompleteState.h"
#include "GameOverState.h"

/**
 * @class GameState
 * @brief Główny stan rozgrywki.
 *
 * Klasa ta implementuje pętlę gry, w tym:
 * - Fizykę ruchu piłki (tarcie, odbicia, grawitacja przy skokach).
 * - Detekcję kolizji z mapą kafelkową (TileMap).
 * - Logikę interakcji z elementami specjalnymi (woda, piasek, rampy, dołek).
 * - Zarządzanie cyklem życia (śmierć, wygrana, pauza).
 */
class GameState : public State {
public:
    /**
     * @brief Konstruktor stanu gry.
     * @param data Dane gry.
     * @param level Numer poziomu do załadowania.
     */
    GameState(GameDataRef data, int level);

    /**
     * @brief Inicjalizuje poziom: ładuje mapę, ustawia piłkę, UI i dźwięki.
     */
    void init() override;

    /**
     * @brief Obsługuje wejście (strzał, pauza, reset).
     * @param event Zdarzenie SFML.
     */
    void handleInput(sf::Event& event) override;

    /**
     * @brief Główna pętla fizyki i logiki gry.
     * Wykonuje kroki symulacji fizycznej, sprawdza warunki zwycięstwa/porażki.
     * @param dt Czas klatki (Delta Time).
     */
    void update(float dt) override;

    /**
     * @brief Rysuje mapę, piłkę, UI i efekty.
     * @param dt Interpolacja klatki.
     */
    void draw(float dt) override;

private:
    GameDataRef m_data;         ///< Dane gry.
    Ball ball;                  ///< Obiekt gracza (piłka).
    InputManager inputManager;  ///< Menedżer sterowania (myszka).
    TileMap tileMap;            ///< Mapa kafelkowa poziomu.
    std::vector<int> levelData; ///< Surowe dane mapy (ID kafelków).
    sf::Vector2f lastSafePos;   ///< Ostatnia bezpieczna pozycja (do respawnu).
    sf::Vector2f forceVector;   ///< Wektor siły uderzenia.
    int m_level;                ///< Numer aktualnego poziomu.
    sf::CircleShape m_shadow;   ///< Cień pod piłką (używany przy skokach).

    // UI Elements
    sf::Text scoreText;         ///< Licznik uderzeń.
    sf::Text winText;           ///< Tekst wygranej (opcjonalny).
    sf::RectangleShape aimLine; ///< Linia celownicza.

    // Zmienne logiczne stanu gry
    int strokes;                ///< Liczba wykonanych uderzeń.
    bool ballInHole;            ///< Czy piłka wpadła do dołka.
    bool wasInWater;            ///< Czy odegrano dźwięk wpadnięcia do wody.
    bool isSinking;             ///< Czy trwa animacja tonięcia.
    bool isJumping;             ///< Czy piłka jest w powietrzu (skok z rampy).
    float jumpTimer;            ///< Czas pozostały do końca skoku.
    float levelFinishTimer;     ///< Opóźnienie po wpadnięciu do dołka przed zmianą stanu.
    float maxJumpTime;          ///< Całkowity czas trwania aktualnego skoku (do obliczeń skali).

    // System Audio
    sf::Sound m_hitSound;       ///< Dźwięk uderzenia.
    sf::Sound m_splashSound;    ///< Dźwięk wpadnięcia do wody.
    sf::Sound m_wallHitSound;   ///< Dźwięk uderzenia o ścianę.
    sf::Sound m_winSound;       ///< Dźwięk zwycięstwa.

    // Metody pomocnicze (Helper Methods)
    void initUI();              ///< Konfiguracja interfejsu użytkownika.
    void initSounds();          ///< Ładowanie i konfiguracja dźwięków.
    void updateAimLine();       ///< Aktualizacja wizualizacji linii celowania.

    /**
     * @brief Pobiera ID kafelka na danej pozycji w świecie gry.
     * @param position Współrzędne (x, y) w pikselach.
     * @return ID kafelka (int). Zwraca ID trawy (0) jeśli poza zakresem.
     */
    int getTileAt(sf::Vector2f position);

    /**
     * @brief Sprawdza, czy dane ID kafelka jest ścianą.
     * @param tileId ID kafelka.
     * @return true jeśli kafelek jest przeszkodą stałą.
     */
    bool isWall(int tileId);

    // Efekt przejścia (Fader)
    sf::RectangleShape m_fadeRect;
    float m_alpha;

    // System Żyć
    int lives;                          ///< Liczba żyć (serc).
    std::vector<sf::Sprite> heartSprites; ///< Graficzna reprezentacja żyć.
    void updateHearts();                ///< Aktualizacja tekstur serc (pełne/puste).
    void resetLevel();                  ///< Resetuje stan poziomu po utracie życia lub restarcie.
};

#endif //MINI_GOLF_GAMESTATE_H