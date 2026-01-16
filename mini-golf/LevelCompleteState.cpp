#include "LevelCompleteState.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "ResourceManager.h"
#include "DEFINITIONS.h"


LevelCompleteState::LevelCompleteState(GameDataRef data, sf::Sprite bgSprite, std::shared_ptr<sf::Texture> bgTexture, int strokes, int currentLevel)
    : m_data(data), m_backgroundSprite(bgSprite), m_bgTexture(bgTexture), m_strokes(strokes), m_currentLevel(currentLevel)
{
}

void LevelCompleteState::init() {
    m_data->window.setView(m_data->window.getDefaultView());

    m_dimmer.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_dimmer.setFillColor(sf::Color(0, 0, 0, 150));

    const sf::Font& font = FontManager::get("font");

    // 2. Tytuł
    m_title.setFont(font);
    m_title.setString("LEVEL COMPLETE!");
    m_title.setCharacterSize(60);
    m_title.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleRect = m_title.getLocalBounds();
    m_title.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    m_title.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 4.0f);

    // 3. Wynik
    m_scoreText.setFont(font);
    m_scoreText.setString("Strokes: " + std::to_string(m_strokes));
    m_scoreText.setCharacterSize(40);
    m_scoreText.setFillColor(sf::Color::White);
    sf::FloatRect scoreRect = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(scoreRect.left + scoreRect.width / 2.0f, scoreRect.top + scoreRect.height / 2.0f);
    m_scoreText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 50.f);

    // 4. Przycisk NEXT LEVEL (tylko jeśli nie jest to ostatni poziom, np. 5)
    if (m_currentLevel < 5) {
        m_nextLevelButton.setFont(font);
        m_nextLevelButton.setString("NEXT LEVEL");
        m_nextLevelButton.setCharacterSize(50);
        m_nextLevelButton.setFillColor(sf::Color::Green);
        sf::FloatRect nextRect = m_nextLevelButton.getLocalBounds();
        m_nextLevelButton.setOrigin(nextRect.left + nextRect.width / 2.0f, nextRect.top + nextRect.height / 2.0f);
        m_nextLevelButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 50.f);
    } else {
        // Jeśli to koniec gry, wyświetlamy komunikat zamiast przycisku
        m_nextLevelButton.setFont(font);
        m_nextLevelButton.setString("ALL LEVELS CLEARED!");
        m_nextLevelButton.setCharacterSize(40);
        m_nextLevelButton.setFillColor(sf::Color::Cyan);
        sf::FloatRect nextRect = m_nextLevelButton.getLocalBounds();
        m_nextLevelButton.setOrigin(nextRect.left + nextRect.width / 2.0f, nextRect.top + nextRect.height / 2.0f);
        m_nextLevelButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 50.f);
    }

    // 5. Przycisk MENU
    m_menuButton.setFont(font);
    m_menuButton.setString("MAIN MENU");
    m_menuButton.setCharacterSize(40);
    m_menuButton.setFillColor(sf::Color::White);
    sf::FloatRect menuRect = m_menuButton.getLocalBounds();
    m_menuButton.setOrigin(menuRect.left + menuRect.width / 2.0f, menuRect.top + menuRect.height / 2.0f);
    m_menuButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 130.f);

    // Fader
    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
    m_alpha = 0.0f;
    m_isTransitioning = false;
    m_goToMenu = false;
}

void LevelCompleteState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        if (!m_isTransitioning) {
            
            // Kliknięcie NEXT LEVEL (tylko jeśli jest aktywny, tzn. poziom < 5)
            if (m_currentLevel < 5 && m_nextLevelButton.getGlobalBounds().contains(mousePosF)) {
                m_isTransitioning = true;
                m_goToMenu = false; // Idziemy do następnego poziomu
            }

            // Kliknięcie MENU
            if (m_menuButton.getGlobalBounds().contains(mousePosF)) {
                m_isTransitioning = true;
                m_goToMenu = true;
            }
        }
    }
}

void LevelCompleteState::update(float dt) {
    // Hover effect
    if (!m_isTransitioning) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        if (m_currentLevel < 5) {
            if (m_nextLevelButton.getGlobalBounds().contains(mousePosF)) m_nextLevelButton.setFillColor(sf::Color::White);
            else m_nextLevelButton.setFillColor(sf::Color::Green);
        }

        if (m_menuButton.getGlobalBounds().contains(mousePosF)) m_menuButton.setFillColor(sf::Color::Red);
        else m_menuButton.setFillColor(sf::Color::White);
    }

    // Animacja wyjścia
    if (m_isTransitioning) {
        m_alpha += 500.0f * dt;
        if (m_alpha >= 255.0f) {
            m_alpha = 255.0f;
            
            if (m_goToMenu) {
                // Wracamy do Menu Głównego
                m_data->machine.switchState(std::make_unique<MainMenuState>(m_data));
            } else {
                // Idziemy do następnego poziomu (currentLevel + 1)
                m_data->machine.switchState(std::make_unique<GameState>(m_data, m_currentLevel + 1));
            }
            return;
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
}

void LevelCompleteState::draw(float dt) {
    m_data->window.clear();
    
    m_data->window.draw(m_backgroundSprite);
    m_data->window.draw(m_dimmer);
    
    m_data->window.draw(m_title);
    m_data->window.draw(m_scoreText);
    m_data->window.draw(m_nextLevelButton);
    m_data->window.draw(m_menuButton);
    
    if (m_isTransitioning) {
        m_data->window.draw(m_fadeRect);
    }
    
    m_data->window.display();
}