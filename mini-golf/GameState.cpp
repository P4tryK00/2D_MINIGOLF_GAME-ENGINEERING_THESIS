#include "GameState.h"
#include <cmath>
#include <iostream>

GameState::GameState(GameDataRef data) : m_data(data) {
    // Inicjalizacja zmiennych w liście inicjalizacyjnej
    strokes = 0;
    ballInHole = false;
    wasInWater = false;
    isSinking = false;
    forceVector = {0.f, 0.f};
}

void GameState::init() {
    // 1. Definicja Mapy - PRZENIESIONE Z GAME.CPP
    levelData = {
        9, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 10,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 4, 
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 0, 0, 0, 0, 0, 4, 
        3, 0, 0, 0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 8,
    };

    tileMap.load("tileset", TILE_SIZE, levelData, LEVEL_WIDTH, LEVEL_HEIGHT);

    sf::Vector2u size = m_data->window.getSize();
    ball.setPosition(sf::Vector2f(size.x / 2.f, size.y / 2.f + 200.f));
    lastSafePos = ball.getPosition();

    aimLine.setSize(sf::Vector2f(0.f, 6.f));
    aimLine.setOrigin(0.f, 3.f);
    aimLine.setFillColor(sf::Color::White);

    initUI();
    initSounds();
    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_alpha = 255.0f;
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 255));
}

void GameState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
        ballInHole = false;
        strokes = 0;
        scoreText.setString("Strokes: 0");
        ball.stop();
        sf::Vector2u size = m_data->window.getSize();
        sf::Vector2f startPos(size.x / 2.f, size.y / 2.f + 200.f);
        ball.setPosition(startPos);
        lastSafePos = startPos;
        wasInWater = false;
        isSinking = false;
        ball.setScale(1.0f);
    }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            auto texture = std::make_shared<sf::Texture>();
            texture->create(m_data->window.getSize().x, m_data->window.getSize().y);
            texture->update(m_data->window);

            sf::Sprite sprite;
            sprite.setTexture(*texture);

            m_data->machine.addState(std::make_unique<PauseMenuState>(m_data, sprite, texture), false);
        }



    inputManager.handleEvent(event, ball, m_data->window);
}

void GameState::update(float dt) {

    if (m_alpha > 0.0f) {
        m_alpha -= 300.0f * dt; // Szybkość rozjaśniania
        if (m_alpha < 0.0f) m_alpha = 0.0f;
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }
    if (isSinking) {
        float currentScale = ball.getScale().x;
        // Zmniejszamy skalę (szybkość tonięcia)
        currentScale -= dt * 2.5f;

        if (currentScale < 0.f) currentScale = 0.f;
        ball.setScale(currentScale);

        // Opcjonalnie: Centrujemy piłkę powoli
        // int gridX = static_cast<int>(ball.getPosition().x) / 32; ...
        // Ale to może być zbyteczne, sama skala wystarczy.

        // KONIEC TONIĘCIA -> RESET
        if (currentScale <= 0.f) {
            isSinking = false;
            ball.stop();
            ball.setPosition(lastSafePos);
            ball.setScale(1.0f);
            wasInWater = false; // Resetujemy pamięć wody po respawnie
        }
        return; // PRZERYWAMY update(), nie obliczamy kolizji ani ruchu
    }


    bool ballIsStopped = (std::abs(ball.getVelocity().x) < 5.f && std::abs(ball.getVelocity().y) < 5.f);

    // --- 1. LOGIKA STEROWANIA ---
    if (!ballInHole && ballIsStopped) {
        if (inputManager.isReady()) {
            forceVector = inputManager.getForceVector();
            if (std::abs(forceVector.x) > 1.f || std::abs(forceVector.y) > 1.f) {
                lastSafePos = ball.getPosition();
                m_hitSound.play();
                ball.applyImpulse(forceVector);
                strokes++;
                scoreText.setString("Strokes: " + std::to_string(strokes));
            }
        }
    } else {
        if (inputManager.isReady()) {
            inputManager.getForceVector();
        }
    }

    // --- 2. IDENTYFIKACJA TERENU I DŹWIĘK ---
    int tileUnderBall = getTileAt(ball.getPosition());

    if (tileUnderBall == 11) {
        if (!wasInWater) {
            m_splashSound.play();
            wasInWater = true;
        }
    } else if (tileUnderBall != 11) {
        wasInWater = false;
    }

    // --- 3. FIZYKA TERENU ---
    float currentFriction = 0.98f;
    if (tileUnderBall == 2) currentFriction = 0.90f;

    // --- WODA (ID 11) - FIX FIZYKI ---
    // --- WODA (ID 11) - FIX BLOKOWANIA ---
    if (tileUnderBall == 11) {
        currentFriction = 0.96f;

        int gridX = static_cast<int>(ball.getPosition().x) / 32;
        int gridY = static_cast<int>(ball.getPosition().y) / 32;
        sf::Vector2f waterCenter(gridX * 32.f + 16.f, gridY * 32.f + 16.f);
        sf::Vector2f toCenter = waterCenter - ball.getPosition();
        float distance = std::sqrt(toCenter.x * toCenter.x + toCenter.y * toCenter.y);

        float currentSpeed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x +
                                       ball.getVelocity().y * ball.getVelocity().y);

        if (distance > 1.0f) {
            sf::Vector2f direction = toCenter / distance;

            float pullStrength = 10.0f;
            // Skalowanie siły w zależności od odległości od centrum wody
            float edgeFactor = 1.0f - (distance / 24.0f);
            if (edgeFactor < 0.0f) edgeFactor = 0.0f;


            pullStrength *= (edgeFactor * edgeFactor);

            // Skalowanie dla prędkości
            if (currentSpeed > 300.f) pullStrength *= 0.1f;
            else if (currentSpeed > 100.f) pullStrength *= 0.5f;

            // --- Aplikacja siły ---
            sf::Vector2f currentVel = ball.getVelocity();
            sf::Vector2f impulse = direction * pullStrength;
            sf::Vector2f newVel = currentVel + impulse;

            // Zabezpieczenie przed przyspieszaniem (woda tylko zwalnia/zmienia kierunek)
            float oldSpeed = std::sqrt(currentVel.x*currentVel.x + currentVel.y*currentVel.y);
            float newSpeed = std::sqrt(newVel.x*newVel.x + newVel.y*newVel.y);

            if (newSpeed > oldSpeed && oldSpeed > 0.1f) {
                newVel = (newVel / newSpeed) * oldSpeed;
            }

            ball.applyImpulse(newVel);
        }

        // Reszta logiki (skalowanie wizualne i tonięcie) bez zmian...
        float targetScale = std::min(1.0f, std::max(0.6f, distance / 15.f));
        float currentScale = ball.getScale().x;
        float smoothScale = currentScale + (targetScale - currentScale) * 5.0f * dt;
        ball.setScale(smoothScale);

        if (distance < 8.0f && currentSpeed < 60.f) {
            isSinking = true;
        }

    }

    ball.update(dt, currentFriction);

    // --- 4. KOLIZJE ZE ŚCIANAMI ---
    sf::Vector2f ballPos = ball.getPosition();
    float r = 10.f;
    sf::Vector2f pTop = {ballPos.x, ballPos.y - r};
    sf::Vector2f pBottom = {ballPos.x, ballPos.y + r};
    sf::Vector2f pLeft = {ballPos.x - r, ballPos.y};
    sf::Vector2f pRight = {ballPos.x + r, ballPos.y};

    bool hitTop = isWall(getTileAt(pTop));
    bool hitBottom = isWall(getTileAt(pBottom));
    bool hitLeft = isWall(getTileAt(pLeft));
    bool hitRight = isWall(getTileAt(pRight));

    float speed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);

    if (hitTop || hitBottom) {
        if (speed > 50.f) m_wallHitSound.play();
        ball.bounceY();
        if(hitTop) ball.setPosition(sf::Vector2f(ballPos.x, ballPos.y + 1.f));
        if(hitBottom) ball.setPosition(sf::Vector2f(ballPos.x, ballPos.y - 1.f));
    }

    if (hitLeft || hitRight) {
        if (speed > 50.f) m_wallHitSound.play();
        ball.bounceX();
        if(hitLeft) ball.setPosition(sf::Vector2f(ballPos.x + 1.f, ballPos.y));
        if(hitRight) ball.setPosition(sf::Vector2f(ballPos.x - 1.f, ballPos.y));
    }

    // --- 5. DOŁEK I BETON ---
    if (!ballInHole && !isSinking && tileUnderBall != 11 && tileUnderBall != 1 && tileUnderBall != 12) {
        ball.setScale(1.0f);
    }

    // Dołek
    if (tileUnderBall == 1) {
        int gridX = static_cast<int>(ball.getPosition().x) / 32;
        int gridY = static_cast<int>(ball.getPosition().y) / 32;
        sf::Vector2f holeCenter(gridX * 32.f + 16.f, gridY * 32.f + 16.f);
        sf::Vector2f toHole = holeCenter - ball.getPosition();
        float distance = std::sqrt(toHole.x * toHole.x + toHole.y * toHole.y);

        if (distance < 20.f) {
            float currentSpeed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);
            if (currentSpeed < 250.f) {
                currentFriction = 0.94f;
                sf::Vector2f direction = toHole / distance;
                float pullStrength = 1.0f + (20.0f - distance) * 0.15f;

                ball.applyImpulse(ball.getVelocity() + (direction * pullStrength));
                float scale = std::max(0.0f, distance / 20.f);
                ball.setScale(scale);

                if (distance < 5.0f) {
                    if (!ballInHole) {
                        m_winSound.play();
                        ballInHole = true;
                    }
                    ball.stop();
                    ball.setPosition(holeCenter);
                    ball.setScale(0.f);
                }
            }
        }
    }

    // Beton
    if (tileUnderBall == 12) {
        currentFriction = 0.99f;
        float speed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);
        if (speed > 40.f) {
            float rX = ((std::rand() % 200) - 100) / 100.0f;
            float rY = ((std::rand() % 200) - 100) / 100.0f;
            float roughness = 8.0f;
            sf::Vector2f jitter(rX * roughness, rY * roughness);
            ball.applyImpulse(ball.getVelocity() + jitter);

            float bounceIntensity = (std::abs(rX) + std::abs(rY)) / 2.0f;
            float bounceScale = 1.0f + (bounceIntensity * 0.06f);
            ball.setScale(bounceScale);
        } else {
            ball.setScale(1.0f);
        }
    }

    updateAimLine();

}

void GameState::draw(float dt) {
    m_data->window.clear();
    m_data->window.draw(tileMap);
    ball.draw(m_data->window);
    m_data->window.draw(aimLine);
    m_data->window.draw(scoreText);
    if (ballInHole) {
        m_data->window.draw(winText);
    }
    if (m_alpha > 0.0f) {
        m_data->window.draw(m_fadeRect);
    }
    m_data->window.display();
}

void GameState::initUI() {
    const sf::Font& font = FontManager::get("font");
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);
    scoreText.setString("HIT COUNT: 0");

    winText.setFont(font);
    winText.setCharacterSize(50);
    winText.setFillColor(sf::Color::Red);
    winText.setString("WIN!");

    sf::FloatRect textRect = winText.getLocalBounds();
    winText.setOrigin(textRect.left + textRect.width/2.0f,
                      textRect.top  + textRect.height/2.0f);
    winText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
}

void GameState::initSounds() {
    m_hitSound.setBuffer(SoundManager::get("hit"));
    m_splashSound.setBuffer(SoundManager::get("splash"));
    m_wallHitSound.setBuffer(SoundManager::get("wall_hit"));
    m_winSound.setBuffer(SoundManager::get("win"));
    m_wallHitSound.setVolume(50.f);
}

void GameState::updateAimLine() {
    bool ballIsStopped = (std::abs(ball.getVelocity().x) < 5.f && std::abs(ball.getVelocity().y) < 5.f);

    if (inputManager.isDragging() && !ballInHole && !isSinking && ballIsStopped) {
        sf::Vector2f ballPos = ball.getPosition();
        sf::Vector2f force = inputManager.getCurrentForce(m_data->window);

        float lineLength = std::sqrt(force.x*force.x + force.y*force.y) / 3.0f;
        aimLine.setSize(sf::Vector2f(lineLength, 6.f));
        aimLine.setPosition(ballPos);

        float angle = std::atan2(force.y, force.x) * 180.f / 3.14159f;
        aimLine.setRotation(angle);

        float powerRatio = std::min(lineLength * 3.0f / 1000.f, 1.0f);
        sf::Uint8 r = 255;
        sf::Uint8 gb = static_cast<sf::Uint8>(255 * (1.0f - powerRatio));
        aimLine.setFillColor(sf::Color(r, gb, gb));
    }
    else {
        aimLine.setSize(sf::Vector2f(0.f, 0.f));
    }
}

// Metody pomocnicze (getTileAt, isWall) kopiujesz bez zmian
int GameState::getTileAt(sf::Vector2f position) {
    if (position.x < 0 || position.y < 0 || position.x >= SCREEN_WIDTH || position.y >= SCREEN_HEIGHT) return 3;
    int gridX = static_cast<int>(position.x) / 32;
    int gridY = static_cast<int>(position.y) / 32;
    if (gridX >= LEVEL_WIDTH || gridY >= LEVEL_HEIGHT) return 3;
    int index = gridX + gridY * LEVEL_WIDTH;
    if (index < 0 || index >= levelData.size()) return 3;
    return levelData[index];
}

bool GameState::isWall(int tileId) {
    return tileId >= 3 && tileId <= 10;
}