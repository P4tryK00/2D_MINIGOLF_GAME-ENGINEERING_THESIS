#include "Game.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "MainMenuState.h"

Game::Game(int width, int height, std::string title) {
    // Tworzenie okna z możliwością zamknięcia i paskiem tytułu
    m_data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);

    // Dodanie pierwszego stanu (Menu Główne) na stos
    m_data->machine.addState(std::make_unique<MainMenuState>(m_data));
}

void Game::run() {
    float newTime, frameTime, interpolation;
    float currentTime = this->clock.getElapsedTime().asSeconds();
    float accumulator = 0.0f;

    // Główna pętla gry (Game Loop) z "Fixed Time Step"
    while (m_data->window.isOpen()) {
        // 1. Przetwarzanie zmian stanów (dodawanie/usuwanie ze stosu)
        m_data->machine.processStateChanges();

        newTime = this->clock.getElapsedTime().asSeconds();
        frameTime = newTime - currentTime;

        // Ograniczenie czasu klatki, aby uniknąć "spirali śmierci" przy lagach
        if (frameTime > 0.25f) frameTime = 0.25f;

        currentTime = newTime;
        accumulator += frameTime;

        // Pętla aktualizacji fizyki/logiki (wykonywana stałą ilość razy na sekundę)
        while (accumulator >= timePerFrame.asSeconds()) {
            // Obsługa eventów (klawiatura, mysz)
            sf::Event event;
            while (m_data->window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) m_data->window.close();

                // Przekazanie eventu do aktywnego stanu
                m_data->machine.getActiveState()->handleInput(event);
            }

            // Aktualizacja stanu
            m_data->machine.getActiveState()->update(timePerFrame.asSeconds());
            accumulator -= timePerFrame.asSeconds();
        }

        // Obliczenie interpolacji dla płynnego renderowania między klatkami fizyki
        interpolation = accumulator / timePerFrame.asSeconds();
        m_data->machine.getActiveState()->draw(interpolation);
    }
}