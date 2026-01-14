#include "Game.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "MainMenuState.h"

Game::Game(int width, int height, std::string title) {
    m_data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);

    m_data->machine.addState(std::make_unique<MainMenuState>(m_data));
}

void Game::run() {
    float newTime, frameTime, interpolation;
    float currentTime = this->clock.getElapsedTime().asSeconds();
    float accumulator = 0.0f;

    while (m_data->window.isOpen()) {
        m_data->machine.processStateChanges();

        newTime = this->clock.getElapsedTime().asSeconds();
        frameTime = newTime - currentTime;
        if (frameTime > 0.25f) frameTime = 0.25f;
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= timePerFrame.asSeconds()) {
            // Obsługa eventów w pętli głównej
            sf::Event event;
            while (m_data->window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) m_data->window.close();

                // Przekazujemy event do aktywnego stanu
                m_data->machine.getActiveState()->handleInput(event);
            }

            m_data->machine.getActiveState()->update(timePerFrame.asSeconds());
            accumulator -= timePerFrame.asSeconds();
        }

        interpolation = accumulator / timePerFrame.asSeconds();
        m_data->machine.getActiveState()->draw(interpolation);
    }
}