#include "GameOverState.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "ResourceManager.h"
#include "DEFINITIONS.h"

GameOverState::GameOverState(GameDataRef data, sf::Sprite bgSprite, std::shared_ptr<sf::Texture> bgTexture, int level)
    : m_data(data), m_backgroundSprite(bgSprite), m_bgTexture(bgTexture), m_level(level)
{
}

void GameOverState::init() {
    // Reset widoku do domyślnego
    m_data->window.setView(m_data->window.getDefaultView());

    // 1. Konfiguracja przyciemnienia tła
    m_dimmer.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_dimmer.setFillColor(sf::Color(50, 0, 0, 150)); // Półprzezroczysty czerwony

    const sf::Font& font = FontManager::get("font");

    // 2. Tytuł GAME OVER - centrowanie
    m_title.setFont(font);
    m_title.setString("GAME OVER");
    m_title.setCharacterSize(80);
    m_title.setFillColor(sf::Color::Red);
    sf::FloatRect titleRect = m_title.getLocalBounds();
    m_title.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    m_title.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 4.0f);

    float centerX = SCREEN_WIDTH / 2.0f;
    float centerY = SCREEN_HEIGHT / 2.0f;

    // 3. Konfiguracja przycisków
    m_retryButton.setFont(font);
    m_retryButton.setString("TRY AGAIN");
    m_retryButton.setCharacterSize(50);
    m_retryButton.setFillColor(sf::Color::White);
    sf::FloatRect retryRect = m_retryButton.getLocalBounds();
    m_retryButton.setOrigin(retryRect.left + retryRect.width / 2.0f, retryRect.top + retryRect.height / 2.0f);
    m_retryButton.setPosition(centerX, centerY);

    m_menuButton.setFont(font);
    m_menuButton.setString("MAIN MENU");
    m_menuButton.setCharacterSize(40);
    m_menuButton.setFillColor(sf::Color::White);
    sf::FloatRect menuRect = m_menuButton.getLocalBounds();
    m_menuButton.setOrigin(menuRect.left + menuRect.width / 2.0f, menuRect.top + menuRect.height / 2.0f);
    m_menuButton.setPosition(centerX, centerY + 100.f);

    // Konfiguracja efektu przejścia (Fader)
    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
    m_alpha = 0.0f;
    m_isTransitioning = false;
    m_goToMenu = false;
    m_retryLevel = false;
}

void GameOverState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        if (!m_isTransitioning) {
            // Sprawdzenie kliknięcia w przycisk RETRY
            if (m_retryButton.getGlobalBounds().contains(mousePosF)) {
                m_isTransitioning = true;
                m_retryLevel = true;
            }

            // Sprawdzenie kliknięcia w przycisk MENU
            if (m_menuButton.getGlobalBounds().contains(mousePosF)) {
                m_isTransitioning = true;
                m_goToMenu = true;
            }
        }
    }
}

void GameOverState::update(float dt) {
    // Efekt podświetlenia (Hover)
    if (!m_isTransitioning) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        if (m_retryButton.getGlobalBounds().contains(mousePosF)) m_retryButton.setFillColor(sf::Color::Yellow);
        else m_retryButton.setFillColor(sf::Color::White);

        if (m_menuButton.getGlobalBounds().contains(mousePosF)) m_menuButton.setFillColor(sf::Color::Red);
        else m_menuButton.setFillColor(sf::Color::White);
    }

    // Obsługa animacji wyjścia (Fade out)
    if (m_isTransitioning) {
        m_alpha += 500.0f * dt;
        if (m_alpha >= 255.0f) {
            m_alpha = 255.0f;

            if (m_goToMenu) {
                m_data->machine.switchState(std::make_unique<MainMenuState>(m_data));
            } else if (m_retryLevel) {
                // Restart poziomu
                m_data->machine.switchState(std::make_unique<GameState>(m_data, m_level));
            }
            return;
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
}

void GameOverState::draw(float dt) {
    m_data->window.clear();

    // Rysowanie zrzutu ekranu gry w tle
    m_data->window.draw(m_backgroundSprite);
    // Rysowanie przyciemnienia
    m_data->window.draw(m_dimmer);

    m_data->window.draw(m_title);
    m_data->window.draw(m_retryButton);
    m_data->window.draw(m_menuButton);

    // Rysowanie efektu przejścia na wierzchu
    if (m_isTransitioning) {
        m_data->window.draw(m_fadeRect);
    }
    
    m_data->window.display();
}