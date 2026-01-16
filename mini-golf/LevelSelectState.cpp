#include "LevelSelectState.h"


LevelSelectState::LevelSelectState(GameDataRef data) : m_data(data) {
}

void LevelSelectState::init() {
    m_data->window.setView(m_data->window.getDefaultView());

    const sf::Font& font = FontManager::get("font");

    // Tytuł
    m_title.setFont(font);
    m_title.setString("SELECT LEVEL");
    m_title.setCharacterSize(60);
    m_title.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleRect = m_title.getLocalBounds();
    m_title.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    m_title.setPosition(SCREEN_WIDTH / 2.0f, 100.f);

    // Generowanie przycisków 1-5
    for (int i = 1; i <= 5; i++) {
        sf::Text btn;
        btn.setFont(font);
        btn.setString("LEVEL " + std::to_string(i));
        btn.setCharacterSize(40);
        btn.setFillColor(sf::Color::White);
        
        sf::FloatRect rect = btn.getLocalBounds();
        btn.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
        
        // Układamy je w pionie
        btn.setPosition(SCREEN_WIDTH / 2.0f, 200.f + (i * 70.f));
        
        m_levelButtons.push_back(btn);
    }

    // Przycisk BACK
    m_backButton.setFont(font);
    m_backButton.setString("BACK");
    m_backButton.setCharacterSize(40);
    m_backButton.setFillColor(sf::Color::White);
    sf::FloatRect backRect = m_backButton.getLocalBounds();
    m_backButton.setOrigin(backRect.left + backRect.width / 2.0f, backRect.top + backRect.height / 2.0f);
    m_backButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 100.f);

    // Fader setup
    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 255));
    m_alpha = 255.0f;
    m_isFadingIn = true;
    m_isTransitioning = false;
    m_nextLevelToLoad = -1;
}

void LevelSelectState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        if (!m_isFadingIn && !m_isTransitioning) {
            
            // Sprawdzamy przyciski poziomów
            for (int i = 0; i < m_levelButtons.size(); i++) {
                if (m_levelButtons[i].getGlobalBounds().contains(mousePosF)) {
                    m_nextLevelToLoad = i + 1; // Poziomy są 1-5, indexy 0-4
                    m_isTransitioning = true;
                    return;
                }
            }

            // Przycisk BACK
            if (m_backButton.getGlobalBounds().contains(mousePosF)) {
                m_nextLevelToLoad = -1; // -1 oznacza powrót do Menu
                m_isTransitioning = true;
            }
        }
    }
}

void LevelSelectState::update(float dt) {
    // Hover Effects
    if (!m_isFadingIn && !m_isTransitioning) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        // Poziomy
        for (auto& btn : m_levelButtons) {
            if (btn.getGlobalBounds().contains(mousePosF)) btn.setFillColor(sf::Color::Red);
            else btn.setFillColor(sf::Color::White);
        }
        // Back
        if (m_backButton.getGlobalBounds().contains(mousePosF)) m_backButton.setFillColor(sf::Color::Red);
        else m_backButton.setFillColor(sf::Color::White);
    }

    // Animacja Wejścia
    if (m_isFadingIn) {
        m_alpha -= 500.0f * dt;
        if (m_alpha <= 0.0f) {
            m_alpha = 0.0f;
            m_isFadingIn = false;
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
    // Animacja Wyjścia
    else if (m_isTransitioning) {
        m_alpha += 500.0f * dt;
        if (m_alpha >= 255.0f) {
            m_alpha = 255.0f;
            
            if (m_nextLevelToLoad != -1) {
                // Ładujemy wybrany poziom
                m_data->machine.addState(std::make_unique<GameState>(m_data, m_nextLevelToLoad), true);
            } else {
                // Wracamy do Menu
                m_data->machine.addState(std::make_unique<MainMenuState>(m_data), true);
            }
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
}

void LevelSelectState::draw(float dt) {
    m_data->window.clear(sf::Color(30, 30, 30));

    m_data->window.draw(m_title);
    for (const auto& btn : m_levelButtons) {
        m_data->window.draw(btn);
    }
    m_data->window.draw(m_backButton);

    if (m_alpha > 0.0f) {
        m_data->window.draw(m_fadeRect);
    }

    m_data->window.display();
}