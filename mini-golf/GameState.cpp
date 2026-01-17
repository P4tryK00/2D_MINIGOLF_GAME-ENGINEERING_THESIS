#include "GameState.h"
#include <cmath>
#include <iostream>
#include <algorithm>


GameState::GameState(GameDataRef data, int level) : m_data(data), m_level(level) {
    strokes = 0;
    ballInHole = false;
    wasInWater = false;
    isSinking = false;

    isJumping = false;
    jumpTimer = 0.0f;
    maxJumpTime = 0.0f;

    lives = 3;

    forceVector = {0.f, 0.f};
    levelFinishTimer = 0.0f;
}

void GameState::init() {

    levelData = LevelManager::loadLevelFromImage(m_level);
    tileMap.load("tileset", TILE_SIZE, levelData, LEVEL_WIDTH, LEVEL_HEIGHT);
    m_data->window.setView(m_data->window.getDefaultView());

    ball.setPosition((LevelManager::startPosition));
    lastSafePos = ball.getPosition();

    aimLine.setSize(sf::Vector2f(0.f, 6.f));
    aimLine.setOrigin(0.f, 3.f);
    aimLine.setFillColor(sf::Color::White);

    initUI();
    initSounds();

    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_alpha = 255.0f;
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 255));

    lives = 3;
    heartSprites.clear();
    const sf::Texture& heartTex = TextureManager::get("heart");

    for (int i = 0; i < 3; i++) {
        float scale = 1.5f;
        sf::Sprite heart;
        heart.setTexture(heartTex);
        heart.setScale(scale, scale);
        float spacing = 60.0f;
        float margin = 70.0f;
        float xPos = SCREEN_WIDTH - margin - (i * spacing);
        float yPos = 10.f;
        heart.setPosition(xPos, yPos);
        heartSprites.push_back(heart);
    }

    m_shadow.setRadius(9.f);
    m_shadow.setFillColor(sf::Color(0, 0, 0, 100));
    m_shadow.setOrigin(9.f, 9.f);

    updateHearts();
}

void GameState::updateHearts() {
    const sf::Texture& redHeart = TextureManager::get("heart");
    const sf::Texture& whiteHeart = TextureManager::get("heart_white");

    for (int i = 0; i < heartSprites.size(); i++) {
        if (i < lives) {
            heartSprites[i].setTexture(redHeart);
        } else {
            heartSprites[i].setTexture(whiteHeart);
        }
    }
}

void GameState::resetLevel() {
    ballInHole = false;
    strokes = 0;
    lives = 3;
    updateHearts();

    scoreText.setString("Strokes: 0");
    ball.stop();

    sf::Vector2u size = m_data->window.getSize();
    // Resetujemy do pozycji startowej z LevelManager (lub lastSafePos jeśli wolisz ostatnie bezpieczne miejsce)
    ball.setPosition(LevelManager::startPosition);
    lastSafePos = LevelManager::startPosition;

    wasInWater = false;
    isSinking = false;
    ball.setScale(1.0f);
    isJumping = false;
}

void GameState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
        resetLevel();
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        auto texture = std::make_shared<sf::Texture>();
        texture->create(m_data->window.getSize().x, m_data->window.getSize().y);
        texture->update(m_data->window);

        sf::Sprite sprite;
        sprite.setTexture(*texture);

        m_data->machine.addState(std::make_unique<PauseMenuState>(m_data, sprite, texture, m_level), false);
    }

    inputManager.handleEvent(event, ball, m_data->window);
}

void GameState::update(float dt) {

    if (m_alpha > 0.0f) {
        m_alpha -= 300.0f * dt;
        if (m_alpha < 0.0f) m_alpha = 0.0f;
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }

    // --- TONIĘCIE ---
    if (isSinking) {
        float currentScale = ball.getScale().x;
        currentScale -= dt * 2.5f;
        if (currentScale < 0.f) currentScale = 0.f;
        ball.setScale(currentScale);

        if (currentScale <= 0.f) {
            isSinking = false;
            lives--;
            updateHearts();

            if (lives <= 0) {
                auto texture = std::make_shared<sf::Texture>();
                texture->create(m_data->window.getSize().x, m_data->window.getSize().y);
                texture->update(m_data->window);
                sf::Sprite bgSprite(*texture);
                m_data->machine.switchState(std::make_unique<GameOverState>(m_data, bgSprite, texture, m_level));
                return;
            } else {
                ball.stop();
                ball.setPosition(lastSafePos);
                ball.setScale(1.0f);
                wasInWater = false;
            }
        }
        return;
    }

    bool ballIsStopped = (std::abs(ball.getVelocity().x) < 5.f && std::abs(ball.getVelocity().y) < 5.f);

    // --- STRZAŁ ---
    if (!ballInHole && ballIsStopped && !isJumping) {
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

    int tileUnderBall = getTileAt(ball.getPosition());

    // --- DŹWIĘK WODY ---
    if (tileUnderBall == 11 && !isJumping) {
        if (!wasInWater) {
            m_splashSound.play();
            wasInWater = true;
        }
    } else if (tileUnderBall != 11) {
        wasInWater = false;
    }

    // --- FIZYKA TARCIA ---
    float currentFriction = 0.98f;
    if (isJumping) currentFriction = 0.999f;
    else if (tileUnderBall == 2) currentFriction = 0.90f;

    // --- FIZYKA WODY (PRZYCIĄGANIE) ---
    if (tileUnderBall == 11 && !isJumping) {
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
            float edgeFactor = 1.0f - (distance / 24.0f);
            if (edgeFactor < 0.0f) edgeFactor = 0.0f;
            pullStrength *= (edgeFactor * edgeFactor);

            if (currentSpeed > 300.f) pullStrength *= 0.1f;
            else if (currentSpeed > 100.f) pullStrength *= 0.5f;

            sf::Vector2f currentVel = ball.getVelocity();
            sf::Vector2f impulse = direction * pullStrength;
            ball.applyImpulse(currentVel + impulse);
        }

        // Efekt zmniejszania w wodzie
        float targetScale = std::min(1.0f, std::max(0.6f, distance / 15.f));
        float currentScale = ball.getScale().x;
        float smoothScale = currentScale + (targetScale - currentScale) * 5.0f * dt;
        ball.setScale(smoothScale);

        if (distance < 8.0f && currentSpeed < 60.f) {
            isSinking = true;
        }
    }

    ball.update(dt, currentFriction);

    // --- OBSŁUGA POZYCJI POZA EKRANEM ---
    sf::Vector2f pos = ball.getPosition();
    sf::Vector2u windowSize = m_data->window.getSize();
    if (pos.x < -50 || pos.x > windowSize.x + 50 || pos.y < -50 || pos.y > windowSize.y + 50) {
        lives--;
        updateHearts();
        if (lives <= 0) {
            auto texture = std::make_shared<sf::Texture>();
            texture->create(m_data->window.getSize().x, m_data->window.getSize().y);
            texture->update(m_data->window);
            sf::Sprite bgSprite(*texture);
            m_data->machine.switchState(std::make_unique<GameOverState>(m_data, bgSprite, texture, m_level));
            return;
        } else {
            ball.stop();
            ball.setPosition(lastSafePos);
            ball.setScale(1.0f);
            isJumping = false;
            jumpTimer = 0.0f;
        }
    }

    // ==========================================
    // === MECHANIKA RAMP - POPRAWIONA ===
    // ==========================================
    float speed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);

    if (!isJumping) {
        bool triggerJump = false;
        float minSpeedForJump = 120.0f; // Zmniejszyłem lekko próg

        if (speed > minSpeedForJump) {
            if (tileUnderBall == 13 && ball.getVelocity().y < 0) triggerJump = true;
            else if (tileUnderBall == 14 && ball.getVelocity().y > 0) triggerJump = true;
            else if (tileUnderBall == 15 && ball.getVelocity().x > 0) triggerJump = true;
            else if (tileUnderBall == 16 && ball.getVelocity().x < 0) triggerJump = true;
        }

        if (triggerJump) {
            isJumping = true;

            // --- DYNAMICZNY CZAS SKOKU ---
            // Im szybciej jedziesz, tym dłużej lecisz.
            // speed ~150 -> 0.6s
            // speed ~400 -> 1.2s
            float dynamicTime = speed / 350.0f;

            // Clamp (ograniczenie) czasu skoku (min 0.6s, max 1.3s)
            if (dynamicTime < 0.6f) dynamicTime = 0.6f;
            if (dynamicTime > 1.3f) dynamicTime = 1.3f;

            jumpTimer = dynamicTime;
            maxJumpTime = dynamicTime; // Zapamiętujemy do obliczania skali
        }
    }

    // --- OBSŁUGA LOTU ---
    if (isJumping) {
        jumpTimer -= dt;

        // Progress od 0.0 do 1.0
        float jumpProgress = 1.0f - (jumpTimer / maxJumpTime);

        // Sinusoida dla wysokości (skali)
        float scaleFactor = 1.0f + (0.5f * std::sin(jumpProgress * std::numbers::pi_v<float>));

        // Cień maleje, gdy piłka jest wysoko
        float shadowScale = 1.0f - (scaleFactor - 1.0f);
        m_shadow.setScale(shadowScale, shadowScale);
        m_shadow.setPosition(ball.getPosition());

        ball.setScale(scaleFactor);

        if (jumpTimer <= 0.0f) {
            isJumping = false;
            ball.setScale(1.0f);

            // Lądowanie poza mapą
            sf::Vector2f landPos = ball.getPosition();
            if (landPos.x < 0 || landPos.x > windowSize.x || landPos.y < 0 || landPos.y > windowSize.y) {
                 ball.stop();
                 ball.setPosition(lastSafePos);
            }
        }
    } else {
        // Cień normalnie
        m_shadow.setScale(1.0f, 1.0f);
        m_shadow.setPosition(ball.getPosition().x + 2.0f, ball.getPosition().y + 2.0f);
    }

    // ==================================================
    // === POPRAWIONE KOLIZJE ZE ŚCIANAMI (Sub-tile) ===
    // ==================================================
    if (!isJumping) {
        sf::Vector2f ballPos = ball.getPosition();
        float r = 10.f;

        sf::Vector2f pTop    = {ballPos.x, ballPos.y - r};
        sf::Vector2f pBottom = {ballPos.x, ballPos.y + r};
        sf::Vector2f pLeft   = {ballPos.x - r, ballPos.y};
        sf::Vector2f pRight  = {ballPos.x + r, ballPos.y};

        int tTop    = getTileAt(pTop);
        int tBottom = getTileAt(pBottom);
        int tLeft   = getTileAt(pLeft);
        int tRight  = getTileAt(pRight);


        auto isSolidPart = [&](int tileId, sf::Vector2f point) -> bool {
            int lx = static_cast<int>(point.x) % 32;
            int ly = static_cast<int>(point.y) % 32;
            if (lx < 0) lx += 32;
            if (ly < 0) ly += 32;

            int half = 24; // Połowa kafelka

            // 3: Ściana PRAWA (odbija od prawej, lewa to trawa) -> Hitbox: lx > 16
            if (tileId == 3) return lx > half;

            // 4: Ściana LEWA (odbija od lewej, prawa to trawa) -> Hitbox: lx < 16
            if (tileId == 4) return lx < half;

            // 5: Ściana DÓŁ (odbija od dołu, góra to trawa) -> Hitbox: ly > 16
            if (tileId == 5) return ly > half;

            // 6: Ściana GÓRA (odbija od góry, dół to trawa) -> Hitbox: ly < 16
            if (tileId == 6) return ly < half;

            return true; // Inne ściany (7,8,9,10) są pełne
        };

        // --- Sprawdzanie kolizji ze standardowymi ścianami (3-10) ---
        bool stdHitTop = isWall(tTop);
        if (stdHitTop && (tTop >= 3 && tTop <= 6)) stdHitTop = isSolidPart(tTop, pTop);

        bool stdHitBottom = isWall(tBottom);
        if (stdHitBottom && (tBottom >= 3 && tBottom <= 6)) stdHitBottom = isSolidPart(tBottom, pBottom);

        bool stdHitLeft = isWall(tLeft);
        if (stdHitLeft && (tLeft >= 3 && tLeft <= 6)) stdHitLeft = isSolidPart(tLeft, pLeft);

        bool stdHitRight = isWall(tRight);
        if (stdHitRight && (tRight >= 3 && tRight <= 6)) stdHitRight = isSolidPart(tRight, pRight);


        // --- Blokowanie ramp od złej strony (bez zmian) ---
        bool rampHitTop = false, rampHitBottom = false, rampHitLeft = false, rampHitRight = false;
        if (tTop == 14 || tTop == 15 || tTop == 16) rampHitTop = true;
        if (tBottom == 13 || tBottom == 15 || tBottom == 16) rampHitBottom = true;
        if (tLeft == 13 || tLeft == 14 || tLeft == 15) rampHitLeft = true;
        if (tRight == 13 || tRight == 14 || tRight == 16) rampHitRight = true;

        bool hitTop    = stdHitTop || rampHitTop;
        bool hitBottom = stdHitBottom || rampHitBottom;
        bool hitLeft   = stdHitLeft || rampHitLeft;
        bool hitRight  = stdHitRight || rampHitRight;

        if (hitTop || hitBottom) {
            if (speed > 50.f && (stdHitTop || stdHitBottom)) {
                m_wallHitSound.play();
            }
            ball.bounceY();
            if(hitTop) ball.setPosition(sf::Vector2f(ballPos.x, ballPos.y + 1.f));
            if(hitBottom) ball.setPosition(sf::Vector2f(ballPos.x, ballPos.y - 1.f));
        }

        if (hitLeft || hitRight) {
            if (speed > 50.f && (stdHitLeft || stdHitRight)) {
                m_wallHitSound.play();
            }
            ball.bounceX();
            if(hitLeft) ball.setPosition(sf::Vector2f(ballPos.x + 1.f, ballPos.y));
            if(hitRight) ball.setPosition(sf::Vector2f(ballPos.x - 1.f, ballPos.y));
        }
    }

    if (!ballInHole && !isSinking && !isJumping && tileUnderBall != 11 && tileUnderBall != 1 && tileUnderBall != 12) {
        ball.setScale(1.0f);
    }

    // Dołek
    if (tileUnderBall == 1 && !isJumping) {
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

    // Przejście do wygranej
    if (ballInHole) {
        levelFinishTimer += dt;
        if (levelFinishTimer > 1.0f) {
            auto texture = std::make_shared<sf::Texture>();
            texture->create(m_data->window.getSize().x, m_data->window.getSize().y);
            texture->update(m_data->window);
            sf::Sprite bgSprite(*texture);
            m_data->machine.switchState(std::make_unique<LevelCompleteState>(m_data, bgSprite, texture, strokes, m_level));
            return;
        }
    }

    // Beton
    if (tileUnderBall == 12 && !isJumping) {
         currentFriction = 0.99f;
         float speedBeton = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);
         if (speedBeton > 40.f) {
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
    m_data->window.draw(m_shadow);
    ball.draw(m_data->window);
    m_data->window.draw(aimLine);
    m_data->window.draw(scoreText);

    for (const auto& heart : heartSprites) {
        m_data->window.draw(heart);
    }

    if (ballInHole) {
        m_data->window.draw(winText);
    }
    if (m_alpha > 0.0f) {
        m_data->window.draw(m_fadeRect);
    }
    m_data->window.display();
}

// ... initUI, initSounds, updateAimLine, getTileAt, isWall bez zmian ...
void GameState::initUI() {
    const sf::Font& font = FontManager::get("font");
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);
    scoreText.setString("Strokes: 0");

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

    if (inputManager.isDragging() && !ballInHole && !isSinking && ballIsStopped && !isJumping) {
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