#include "MainMenuState.h"
#include "GameState.h"
#include "DEFINITIONS.h"

MainMenuState::MainMenuState(GameDataRef data) : m_data(data) {
}

void MainMenuState::init() {
    m_data->window.setView(m_data->window.getDefaultView());

    // Inicjalizacja systemu zapisu
    SaveManager::init();

    const sf::Font& font = FontManager::get("font");

    // 1. TYTUŁ
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
    // Ustawiamy pozycję (wyżej, bo doszedł leaderboard)
    m_playButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 40.f);

    // 3. Przycisk LEADERBOARD
    m_leaderboardButton.setFont(font);
    m_leaderboardButton.setString("LEADERBOARD");
    m_leaderboardButton.setCharacterSize(50);
    m_leaderboardButton.setFillColor(sf::Color::White);

    sf::FloatRect lbRect = m_leaderboardButton.getLocalBounds();
    m_leaderboardButton.setOrigin(lbRect.left + lbRect.width/2.0f,
                                  lbRect.top + lbRect.height/2.0f);
    m_leaderboardButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 40.f);

    // 4. Przycisk EXIT
    m_exitButton.setFont(font);
    m_exitButton.setString("EXIT");
    m_exitButton.setCharacterSize(50);
    m_exitButton.setFillColor(sf::Color::White);

    // --- POPRAWKA: Brakowało ustawienia Origin dla Exit ---
    sf::FloatRect exitRect = m_exitButton.getLocalBounds();
    m_exitButton.setOrigin(exitRect.left + exitRect.width / 2.0f,
                           exitRect.top  + exitRect.height / 2.0f);
    m_exitButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 120.f);


    // --- KONFIGURACJA PRZEJŚCIA ---
    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 255)); // Startujemy od czerni

    m_alpha = 255.0f;
    m_isFadingIn = true;
    m_isTransitioning = false;
    m_goToLeaderboard = false;
}

void MainMenuState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
            sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

            // Blokujemy klikanie, jeśli trwa animacja
            if (!m_isFadingIn && !m_isTransitioning) {

                // KLIKNIĘCIE PLAY
                if (m_playButton.getGlobalBounds().contains(mousePosF)) {
                    m_goToLeaderboard = false; // Idziemy do wyboru poziomów
                    m_isTransitioning = true;
                }

                // KLIKNIĘCIE LEADERBOARD
                if (m_leaderboardButton.getGlobalBounds().contains(mousePosF)) {
                    m_goToLeaderboard = true; // Idziemy do tabeli wyników
                    m_isTransitioning = true;
                }

                // KLIKNIĘCIE EXIT
                if (m_exitButton.getGlobalBounds().contains(mousePosF)) {
                    m_data->window.close();
                }
            }
        }
    }
}

void MainMenuState::update(float dt) {
    // 1. ANIMACJA WEJŚCIA
    if (m_isFadingIn) {
        m_alpha -= 500.0f * dt;
        if (m_alpha <= 0.0f) {
            m_alpha = 0.0f;
            m_isFadingIn = false;
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
    // 2. ANIMACJA WYJŚCIA (Zmiana stanu)
    else if (m_isTransitioning) {
        m_alpha += 500.0f * dt;
        if (m_alpha >= 255.0f) {
            m_alpha = 255.0f;

            // Decyzja gdzie idziemy na podstawie flagi
            if (m_goToLeaderboard) {
                m_data->machine.addState(std::make_unique<LeaderboardState>(m_data), true);
            } else {
                m_data->machine.addState(std::make_unique<LevelSelectState>(m_data), true);
            }
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }

    // 3. HOVER EFFECTS (Podświetlanie)
    if (!m_isFadingIn && !m_isTransitioning) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        // Play
        if (m_playButton.getGlobalBounds().contains(mousePosF)) {
            m_playButton.setFillColor(sf::Color::Red);
        } else {
            m_playButton.setFillColor(sf::Color::White);
        }

        // Leaderboard
        if (m_leaderboardButton.getGlobalBounds().contains(mousePosF)) {
            m_leaderboardButton.setFillColor(sf::Color::Red);
        } else {
            m_leaderboardButton.setFillColor(sf::Color::White);
        }

        // Exit
        if (m_exitButton.getGlobalBounds().contains(mousePosF)) {
            m_exitButton.setFillColor(sf::Color::Red);
        } else {
            m_exitButton.setFillColor(sf::Color::White);
        }
    }
}

void MainMenuState::draw(float dt) {
    m_data->window.clear(sf::Color(30, 30, 30));

    m_data->window.draw(m_title);
    m_data->window.draw(m_playButton);
    m_data->window.draw(m_leaderboardButton); // Dodane rysowanie
    m_data->window.draw(m_exitButton);

    if (m_alpha > 0.0f) {
        m_data->window.draw(m_fadeRect);
    }

    m_data->window.display();
}