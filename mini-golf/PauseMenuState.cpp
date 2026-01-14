//
// Created by patry on 14.01.2026.
//

#include "PauseMenuState.h"
#include "MainMenuState.h"
#include "ResourceManager.h" // Dla czcionek
#include <iostream>

PauseMenuState::PauseMenuState(GameDataRef data, sf::Sprite bgSprite, std::shared_ptr<sf::Texture> bgTexture)
    : m_data(data), m_backgroundSprite(bgSprite), m_bgTexture(bgTexture)
{
}

void PauseMenuState::init() {
    // 1. Przyciemnienie tła (stały element interfejsu)
    m_dimmer.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_dimmer.setFillColor(sf::Color(0, 0, 0, 150));

    const sf::Font& font = FontManager::get("font");

    // 2. Tytuł PAUSED
    m_pauseTitle.setFont(font);
    m_pauseTitle.setString("PAUSED");
    m_pauseTitle.setCharacterSize(60);
    m_pauseTitle.setFillColor(sf::Color::White);
    sf::FloatRect titleRect = m_pauseTitle.getLocalBounds();
    m_pauseTitle.setOrigin(titleRect.left + titleRect.width/2.0f, titleRect.top + titleRect.height/2.0f);
    m_pauseTitle.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 4.0f);

    // 3. Przycisk RESUME
    m_resumeButton.setFont(font);
    m_resumeButton.setString("RESUME");
    m_resumeButton.setCharacterSize(40);
    m_resumeButton.setFillColor(sf::Color::White);
    sf::FloatRect resRect = m_resumeButton.getLocalBounds();
    m_resumeButton.setOrigin(resRect.left + resRect.width/2.0f, resRect.top + resRect.height/2.0f);
    m_resumeButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

    // 4. Przycisk MENU
    m_menuButton.setFont(font);
    m_menuButton.setString("BACK TO MENU");
    m_menuButton.setCharacterSize(40);
    m_menuButton.setFillColor(sf::Color::White);
    sf::FloatRect menuRect = m_menuButton.getLocalBounds();
    m_menuButton.setOrigin(menuRect.left + menuRect.width/2.0f, menuRect.top + menuRect.height/2.0f);
    m_menuButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 70.f);

    // --- 5. KONFIGURACJA PRZEJŚCIA (FADE OUT) ---
    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 0)); // Startujemy od przezroczystego
    m_alpha = 0.0f;
    m_isGoingToMenu = false;
}

void PauseMenuState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            // Jeśli nie trwa animacja wyjścia, ESC wznawia grę
            if (!m_isGoingToMenu) {
                m_data->machine.removeState();
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        // Blokujemy klikanie podczas animacji wyjścia
        if (!m_isGoingToMenu) {
            if (m_resumeButton.getGlobalBounds().contains(mousePosF)) {
                m_data->machine.removeState(); // Wracamy do gry
            }

            if (m_menuButton.getGlobalBounds().contains(mousePosF)) {
                // Zamiast zmieniać stan od razu, uruchamiamy animację
                m_isGoingToMenu = true;
            }
        }
    }
}

void PauseMenuState::update(float dt) {
    // Hover effect (tylko gdy nie wychodzimy)
    if (!m_isGoingToMenu) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        if (m_resumeButton.getGlobalBounds().contains(mousePosF)) m_resumeButton.setFillColor(sf::Color::Green);
        else m_resumeButton.setFillColor(sf::Color::White);

        if (m_menuButton.getGlobalBounds().contains(mousePosF)) m_menuButton.setFillColor(sf::Color::Red);
        else m_menuButton.setFillColor(sf::Color::White);
    }

    // --- ANIMACJA WYJŚCIA DO MENU ---
    if (m_isGoingToMenu) {
        m_alpha += 500.0f * dt;
        if (m_alpha >= 255.0f) {
            m_alpha = 255.0f;
            m_data->machine.switchState(std::make_unique<MainMenuState>(m_data));
            return;
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
}

void PauseMenuState::draw(float dt) {
    m_data->window.clear();

    // 1. Rysujemy "zamrożoną" grę
    m_data->window.draw(m_backgroundSprite);

    // 2. Rysujemy przyciemnienie interfejsu
    m_data->window.draw(m_dimmer);

    // 3. Rysujemy przyciski
    m_data->window.draw(m_pauseTitle);
    m_data->window.draw(m_resumeButton);
    m_data->window.draw(m_menuButton);

    // 4. Rysujemy efekt przejścia NA WIERZCHU (jeśli aktywny)
    if (m_isGoingToMenu) {
        m_data->window.draw(m_fadeRect);
    }

    m_data->window.display();
}