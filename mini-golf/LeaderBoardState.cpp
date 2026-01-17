#include "LeaderboardState.h"
#include "MainMenuState.h"
#include "ResourceManager.h"
#include "SaveManager.h"
#include "DEFINITIONS.h"

LeaderboardState::LeaderboardState(GameDataRef data) : m_data(data) {
}

void LeaderboardState::init() {
    m_data->window.setView(m_data->window.getDefaultView());
    const sf::Font& font = FontManager::get("font");

    // Tytuł
    m_title.setFont(font);
    m_title.setString("BEST SCORES");
    m_title.setCharacterSize(60);
    m_title.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleRect = m_title.getLocalBounds();
    m_title.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    m_title.setPosition(SCREEN_WIDTH / 2.0f, 80.f);

    // Wiersze z wynikami
    for (int i = 1; i <= 5; i++) {
        int score = SaveManager::getBestScore(i);
        std::string scoreStr = (score == 0) ? "-" : std::to_string(score);

        sf::Text text;
        text.setFont(font);
        text.setString("Level " + std::to_string(i) + ": " + scoreStr);
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::White);
        
        sf::FloatRect rect = text.getLocalBounds();
        text.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
        text.setPosition(SCREEN_WIDTH / 2.0f, 180.f + (i * 60.f));
        
        m_scoreTexts.push_back(text);
    }

    // Przycisk BACK
    m_backButton.setFont(font);
    m_backButton.setString("BACK");
    m_backButton.setCharacterSize(40);
    m_backButton.setFillColor(sf::Color::White);
    sf::FloatRect backRect = m_backButton.getLocalBounds();
    m_backButton.setOrigin(backRect.left + backRect.width / 2.0f, backRect.top + backRect.height / 2.0f);
    m_backButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 80.f);

    // Fader
    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 255));
    m_alpha = 255.0f;
    m_isFadingIn = true;
    m_isGoingBack = false;
}

void LeaderboardState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        if (!m_isFadingIn && !m_isGoingBack) {
            if (m_backButton.getGlobalBounds().contains(mousePosF)) {
                m_isGoingBack = true;
            }
        }
    }
}

void LeaderboardState::update(float dt) {
    if (!m_isFadingIn && !m_isGoingBack) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);
        if (m_backButton.getGlobalBounds().contains(mousePosF)) m_backButton.setFillColor(sf::Color::Red);
        else m_backButton.setFillColor(sf::Color::White);
    }

    if (m_isFadingIn) {
        m_alpha -= 500.0f * dt;
        if (m_alpha <= 0.0f) { m_alpha = 0.0f; m_isFadingIn = false; }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    } else if (m_isGoingBack) {
        m_alpha += 500.0f * dt;
        if (m_alpha >= 255.0f) {
            m_data->machine.switchState(std::make_unique<MainMenuState>(m_data));
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
}

void LeaderboardState::draw(float dt) {
    m_data->window.clear(sf::Color(30, 30, 30));
    m_data->window.draw(m_title);
    for (const auto& txt : m_scoreTexts) m_data->window.draw(txt);
    m_data->window.draw(m_backButton);
    if (m_alpha > 0.0f) m_data->window.draw(m_fadeRect);
    m_data->window.display();
}