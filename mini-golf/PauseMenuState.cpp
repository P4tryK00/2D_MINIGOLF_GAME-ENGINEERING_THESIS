#include "PauseMenuState.h"
#include "MainMenuState.h"
#include "ResourceManager.h"
#include "GameState.h"

PauseMenuState::PauseMenuState(GameDataRef data, sf::Sprite bgSprite, std::shared_ptr<sf::Texture> bgTexture)
    : m_data(data), m_backgroundSprite(bgSprite), m_bgTexture(bgTexture)
{
}

void PauseMenuState::init() {
    m_dimmer.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_dimmer.setFillColor(sf::Color(0, 0, 0, 150));

    const sf::Font& font = FontManager::get("font");

    m_pauseTitle.setFont(font);
    m_pauseTitle.setString("PAUSED");
    m_pauseTitle.setCharacterSize(60);
    m_pauseTitle.setFillColor(sf::Color::White);
    sf::FloatRect titleRect = m_pauseTitle.getLocalBounds();
    m_pauseTitle.setOrigin(titleRect.left + titleRect.width/2.0f, titleRect.top + titleRect.height/2.0f);
    m_pauseTitle.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 5.0f);

    m_resumeButton.setFont(font);
    m_resumeButton.setString("RESUME");
    m_resumeButton.setCharacterSize(40);
    m_resumeButton.setFillColor(sf::Color::White);
    sf::FloatRect resRect = m_resumeButton.getLocalBounds();
    m_resumeButton.setOrigin(resRect.left + resRect.width/2.0f, resRect.top + resRect.height/2.0f);
    m_resumeButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 50.f);

    m_resetButton.setFont(font);
    m_resetButton.setString("RESET");
    m_resetButton.setCharacterSize(40);
    m_resetButton.setFillColor(sf::Color::White);
    sf::FloatRect resetRect = m_resetButton.getLocalBounds();
    m_resetButton.setOrigin(resetRect.left + resetRect.width/2.0f, resetRect.top + resetRect.height/2.0f);
    m_resetButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 20.f);

    m_menuButton.setFont(font);
    m_menuButton.setString("BACK TO MENU");
    m_menuButton.setCharacterSize(40);
    m_menuButton.setFillColor(sf::Color::White);
    sf::FloatRect menuRect = m_menuButton.getLocalBounds();
    m_menuButton.setOrigin(menuRect.left + menuRect.width/2.0f, menuRect.top + menuRect.height/2.0f);
    m_menuButton.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 90.f);

    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
    m_alpha = 0.0f;

    m_isGoingToMenu = false;
    m_isReseting = false;
}

void PauseMenuState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            if (!m_isGoingToMenu && !m_isReseting) {
                m_data->machine.removeState();
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        if (!m_isGoingToMenu && !m_isReseting) {

            if (m_resumeButton.getGlobalBounds().contains(mousePosF)) {
                m_data->machine.removeState();
            }

            if (m_resetButton.getGlobalBounds().contains(mousePosF)) {
                m_isReseting = true;
            }

            if (m_menuButton.getGlobalBounds().contains(mousePosF)) {
                m_isGoingToMenu = true;
            }
        }
    }
}

void PauseMenuState::update(float dt) {
    if (!m_isGoingToMenu && !m_isReseting) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_data->window);
        sf::Vector2f mousePosF = m_data->window.mapPixelToCoords(mousePos);

        if (m_resumeButton.getGlobalBounds().contains(mousePosF)) m_resumeButton.setFillColor(sf::Color::Green);
        else m_resumeButton.setFillColor(sf::Color::White);

        if (m_resetButton.getGlobalBounds().contains(mousePosF)) m_resetButton.setFillColor(sf::Color::Yellow);
        else m_resetButton.setFillColor(sf::Color::White);

        if (m_menuButton.getGlobalBounds().contains(mousePosF)) m_menuButton.setFillColor(sf::Color::Red);
        else m_menuButton.setFillColor(sf::Color::White);
    }

    if (m_isGoingToMenu) {
        m_alpha += 500.0f * dt;
        if (m_alpha >= 255.0f) {
            m_alpha = 255.0f;
            m_data->machine.switchState(std::make_unique<MainMenuState>(m_data));
            return;
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
    else if (m_isReseting) {
        m_alpha += 500.0f * dt;
        if (m_alpha >= 255.0f) {
            m_alpha = 255.0f;
            m_data->machine.switchState(std::make_unique<GameState>(m_data));
            return;
        }
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
}

void PauseMenuState::draw(float dt) {
    m_data->window.clear();

    m_data->window.draw(m_backgroundSprite);
    m_data->window.draw(m_dimmer);

    m_data->window.draw(m_pauseTitle);
    m_data->window.draw(m_resumeButton);
    m_data->window.draw(m_resetButton);
    m_data->window.draw(m_menuButton);

    if (m_isGoingToMenu || m_isReseting) {
        m_data->window.draw(m_fadeRect);
    }

    m_data->window.display();
}