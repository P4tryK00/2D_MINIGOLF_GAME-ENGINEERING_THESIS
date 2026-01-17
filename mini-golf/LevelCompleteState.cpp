#include "LevelCompleteState.h"

LevelCompleteState::LevelCompleteState(GameDataRef data, sf::Sprite bgSprite, std::shared_ptr<sf::Texture> bgTexture, int strokes, int currentLevel)
    : m_data(data), m_backgroundSprite(bgSprite), m_bgTexture(bgTexture), m_strokes(strokes), m_currentLevel(currentLevel)
{
}

void LevelCompleteState::init() {
    // Automatyczny zapis wyniku przy wejściu do tego stanu
    SaveManager::submitScore(m_currentLevel, m_strokes);

    m_data->window.setView(m_data->window.getDefaultView());

    // Przyciemnienie tła
    m_dimmer.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_dimmer.setFillColor(sf::Color(0, 0, 0, 150));

    const sf::Font& font = FontManager::get("font");

    // 1. Tytuł LEVEL COMPLETE
    m_title.setFont(font);
    m_title.setString("LEVEL COMPLETE!");
    m_title.setCharacterSize(60);
    m_title.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleRect = m_title.getLocalBounds();
    m_title.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    m_title.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 5.0f);

    float centerX = SCREEN_WIDTH / 2.0f;
    float centerY = SCREEN_HEIGHT / 2.0f;

    // 2. Wyświetlenie wyniku (Strokes)
    m_scoreText.setFont(font);
    m_scoreText.setString("Strokes: " + std::to_string(m_strokes));
    m_scoreText.setCharacterSize(40);
    m_scoreText.setFillColor(sf::Color::White);
    sf::FloatRect scoreRect = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(scoreRect.left + scoreRect.width / 2.0f, scoreRect.top + scoreRect.height / 2.0f);
    m_scoreText.setPosition(centerX, centerY - 120.f);

    // 3. Przycisk NEXT LEVEL (Warunkowo: jeśli to nie ostatni poziom)
    if (m_currentLevel < 5) {
        m_nextLevelButton.setFont(font);
        m_nextLevelButton.setString("NEXT LEVEL");
        m_nextLevelButton.setCharacterSize(50);
        m_nextLevelButton.setFillColor(sf::Color::Green);
        sf::FloatRect nextRect = m_nextLevelButton.getLocalBounds();
        m_nextLevelButton.setOrigin(nextRect.left + nextRect.width / 2.0f, nextRect.top + nextRect.height / 2.0f);
        m_nextLevelButton.setPosition(centerX, centerY - 40.f);
    } else {
        // Jeśli koniec gry
        m_nextLevelButton.setFont(font);
        m_nextLevelButton.setString("ALL LEVELS CLEARED!");
        m_nextLevelButton.setCharacterSize(40);
        m_nextLevelButton.setFillColor(sf::Color::Cyan);
        sf::FloatRect nextRect = m_nextLevelButton.getLocalBounds();
        m_nextLevelButton.setOrigin(nextRect.left + nextRect.width / 2.0f, nextRect.top + nextRect.height / 2.0f);
        m_nextLevelButton.setPosition(centerX, centerY - 40.f);
    }

    // 4. Przycisk RETRY
    m_retryButton.setFont(font);
    m_retryButton.setString("RETRY LEVEL");
    m_retryButton.setCharacterSize(40);
    m_retryButton.setFillColor(sf::Color::White);
    sf::FloatRect retryRect = m_retryButton.getLocalBounds();
    m_retryButton.setOrigin(retryRect.left + retryRect.width / 2.0f, retryRect.top + retryRect.height / 2.0f);
    m_retryButton.setPosition(centerX, centerY + 40.f);

    // 5. Przycisk MENU
    m_menuButton.setFont(font);
    m_menuButton.setString("MAIN MENU");
    m_menuButton.setCharacterSize(40);
    m_menuButton.setFillColor(sf::Color::White);
    sf::FloatRect menuRect = m_menuButton.getLocalBounds();
    m_menuButton.setOrigin(menuRect.left + menuRect.width / 2.0f, menuRect.top + menuRect.height / 2.0f);
    m_menuButton.setPosition(centerX, centerY + 120.f);

    // Fader setup
    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
    m_alpha = 0.0f;
    m_isTransitioning = false;
    m_goToMenu = false;
    m_retryLevel = false;
}

void LevelCompleteState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        if (!m_isTransitioning) {

            // Kliknięcie NEXT LEVEL
            if (m_currentLevel < 5 && m_nextLevelButton.getGlobalBounds().contains(mousePosF)) {
                m_isTransitioning = true;
                m_goToMenu = false; // Idziemy do następnego poziomu
            }

            // Kliknięcie RETRY
            if (m_retryButton.getGlobalBounds().contains(mousePosF)) {
                m_isTransitioning = true;
                m_retryLevel = true; // Ustawiamy flagę restartu
                m_goToMenu = false;
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

        if (m_retryButton.getGlobalBounds().contains(mousePosF)) m_retryButton.setFillColor(sf::Color::Yellow);
        else m_retryButton.setFillColor(sf::Color::White);

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
            } else if (m_retryLevel) {
                // Restart obecnego poziomu
                m_data->machine.switchState(std::make_unique<GameState>(m_data, m_currentLevel));
            } else {
                // Idziemy do następnego poziomu
                m_data->machine.switchState(std::make_unique<GameState>(m_data, m_currentLevel + 1));
            }
            return;
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
}

void LevelCompleteState::draw(float dt) {
    m_data->window.clear();

    // Rysowanie elementów w odpowiedniej kolejności (tło -> przyciemnienie -> UI)
    m_data->window.draw(m_backgroundSprite);
    m_data->window.draw(m_dimmer);
    
    m_data->window.draw(m_title);
    m_data->window.draw(m_scoreText);
    m_data->window.draw(m_nextLevelButton);
    m_data->window.draw(m_menuButton);
    m_data->window.draw(m_retryButton);
    
    if (m_isTransitioning) {
        m_data->window.draw(m_fadeRect);
    }
    
    m_data->window.display();
}