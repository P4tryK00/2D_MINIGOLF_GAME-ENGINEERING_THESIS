#include "MainMenuState.h"
#include "GameState.h"
#include "DEFINITIONS.h"

MainMenuState::MainMenuState(GameDataRef data) : m_data(data) {
}

void MainMenuState::init() {
    m_data->window.setView(m_data->window.getDefaultView());

    // 1. Ustawienie Tytułu
    const sf::Font& font = FontManager::get("font");

    m_title.setFont(font);
    m_title.setString("MINI GOLF");
    m_title.setCharacterSize(80);
    m_title.setFillColor(sf::Color::Yellow);
    m_title.setStyle(sf::Text::Bold);

    sf::FloatRect titleRect = m_title.getLocalBounds();
    m_title.setOrigin(titleRect.left + titleRect.width / 2.0f,
                      titleRect.top  + titleRect.height / 2.0f);
    m_title.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 4.0f);

    // 2. Przycisk PLAY
    m_playButton.setFont(font);
    m_playButton.setString("PLAY");
    m_playButton.setCharacterSize(50);
    m_playButton.setFillColor(sf::Color::White);

    sf::FloatRect playRect = m_playButton.getLocalBounds();
    m_playButton.setOrigin(playRect.left + playRect.width / 2.0f,
                           playRect.top  + playRect.height / 2.0f);
    m_playButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

    // 3. Przycisk EXIT
    m_exitButton.setFont(font);
    m_exitButton.setString("EXIT");
    m_exitButton.setCharacterSize(50);
    m_exitButton.setFillColor(sf::Color::White);

    sf::FloatRect exitRect = m_exitButton.getLocalBounds();
    m_exitButton.setOrigin(exitRect.left + exitRect.width / 2.0f,
                           exitRect.top  + exitRect.height / 2.0f);
    m_exitButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 100.f);

    // --- KONFIGURACJA PRZEJŚCIA ---
    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 255)); // Startujemy od czerni

    m_alpha = 255.0f;     // Pełna czerń
    m_isFadingIn = true;  // Włączamy animację wejścia
    m_isTransitioning = false;
}

void MainMenuState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
            sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

            // Blokujemy klikanie, jeśli trwa animacja (żeby nie klikać "przez czerń")
            if (!m_isFadingIn && !m_isTransitioning) {
                if (m_playButton.getGlobalBounds().contains(mousePosF)) {
                    m_isTransitioning = true; // Rozpoczynamy wyjście
                }

                if (m_exitButton.getGlobalBounds().contains(mousePosF)) {
                    m_data->window.close();
                }
            }
        }
    }
}

void MainMenuState::update(float dt) {
    // 1. ANIMACJA WEJŚCIA (Rozjaśnianie z czerni)
    if (m_isFadingIn) {
        m_alpha -= 500.0f * dt; // Szybkość rozjaśniania
        if (m_alpha <= 0.0f) {
            m_alpha = 0.0f;
            m_isFadingIn = false;
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
    //
    else if (m_isTransitioning) {
        m_alpha += 500.0f * dt;
        if (m_alpha >= 255.0f) {
            m_alpha = 255.0f;
            // Zmiana stanu na Grę
            m_data->machine.addState(std::make_unique<LevelSelectState>(m_data), true);
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }

    if (!m_isFadingIn && !m_isTransitioning) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        if (m_playButton.getGlobalBounds().contains(mousePosF)) {
            m_playButton.setFillColor(sf::Color::Red);
        } else {
            m_playButton.setFillColor(sf::Color::White);
        }

        if (m_exitButton.getGlobalBounds().contains(mousePosF)) {
            m_exitButton.setFillColor(sf::Color::Red);
        } else {
            m_exitButton.setFillColor(sf::Color::White);
        }
    }
}

void MainMenuState::draw(float dt) {
    // Czyścimy raz
    m_data->window.clear(sf::Color(30, 30, 30));

    // Rysujemy elementy
    m_data->window.draw(m_title);
    m_data->window.draw(m_playButton);
    m_data->window.draw(m_exitButton);

    if (m_alpha > 0.0f) {
        m_data->window.draw(m_fadeRect);
    }

    // Wyświetlamy raz
    m_data->window.display();
}