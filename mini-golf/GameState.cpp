#include "GameState.h"
#include <cmath>
#include <iostream>
#include <algorithm>

// =============================================================
// KONSTRUKTOR I INICJALIZACJA
// =============================================================

GameState::GameState(GameDataRef data, int level) : m_data(data), m_level(level) {
    // Inicjalizacja zmiennych stanu gry
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
    // 1. Ładowanie poziomu z obrazka
    levelData = LevelManager::loadLevelFromImage(m_level);

    // 2. Generowanie mapy kafelkowej
    tileMap.load("tileset", TILE_SIZE, levelData, LEVEL_WIDTH, LEVEL_HEIGHT);

    // 3. Ustawienie widoku kamery
    m_data->window.setView(m_data->window.getDefaultView());

    // 4. Ustawienie piłki
    ball.setPosition((LevelManager::startPosition));
    lastSafePos = ball.getPosition();

    // 5. Inicjalizacja linii celowania
    aimLine.setSize(sf::Vector2f(0.f, 6.f));
    aimLine.setOrigin(0.f, 3.f);
    aimLine.setFillColor(sf::Color::White);

    // 6. Inicjalizacja UI i Audio
    initUI();
    initSounds();

    // 7. Konfiguracja efektu wejścia (Fade In)
    m_fadeRect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_alpha = 255.0f;
    m_fadeRect.setFillColor(sf::Color(0, 0, 0, 255));

    // 8. Inicjalizacja systemu serc
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

    // 9. Cień pod piłką
    m_shadow.setRadius(9.f);
    m_shadow.setFillColor(sf::Color(0, 0, 0, 100));
    m_shadow.setOrigin(9.f, 9.f);

    updateHearts();
}

// =============================================================
// METODY POMOCNICZE
// =============================================================

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
    ball.setPosition(LevelManager::startPosition);
    lastSafePos = LevelManager::startPosition;

    wasInWater = false;
    isSinking = false;
    ball.setScale(1.0f);
    isJumping = false;
}

// =============================================================
// OBSŁUGA WEJŚCIA
// =============================================================

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

// =============================================================
// UPDATE (FIZYKA I LOGIKA)
// =============================================================

void GameState::update(float dt) {

    // 1. Fade In
    if (m_alpha > 0.0f) {
        m_alpha -= 300.0f * dt;
        if (m_alpha < 0.0f) m_alpha = 0.0f;
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }

    // 2. Tnięcie
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

    // 3. Logika strzału
    bool ballIsStopped = (std::abs(ball.getVelocity().x) < 5.f && std::abs(ball.getVelocity().y) < 5.f);
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
        if (inputManager.isReady()) inputManager.getForceVector();
    }

    int tileUnderBall = getTileAt(ball.getPosition());

    // 4. Dźwięk wody
    if (tileUnderBall == 11 && !isJumping) {
        if (!wasInWater) {
            m_splashSound.play();
            wasInWater = true;
        }
    } else if (tileUnderBall != 11) {
        wasInWater = false;
    }

    // 5. Tarcie
    float currentFriction = 0.98f;
    if (isJumping) currentFriction = 0.999f;
    else if (tileUnderBall == 2) currentFriction = 0.90f;

    // 6. Fizyka Wody
    if (tileUnderBall == 11 && !isJumping) {
        currentFriction = 0.96f;
        int gridX = static_cast<int>(ball.getPosition().x) / 32;
        int gridY = static_cast<int>(ball.getPosition().y) / 32;
        sf::Vector2f waterCenter(gridX * 32.f + 16.f, gridY * 32.f + 16.f);
        sf::Vector2f toCenter = waterCenter - ball.getPosition();
        float distance = std::sqrt(toCenter.x * toCenter.x + toCenter.y * toCenter.y);

        if (distance > 1.0f) {
            sf::Vector2f direction = toCenter / distance;
            float pullStrength = 10.0f;
            float currentSpeed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);

            if (currentSpeed > 300.f) pullStrength *= 0.1f;
            else if (currentSpeed > 100.f) pullStrength *= 0.5f;

            ball.applyImpulse(ball.getVelocity() + (direction * pullStrength));
        }

        float targetScale = std::min(1.0f, std::max(0.6f, distance / 15.f));
        float currentScale = ball.getScale().x;
        float smoothScale = currentScale + (targetScale - currentScale) * 5.0f * dt;
        ball.setScale(smoothScale);

        float speed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);
        if (distance < 8.0f && speed < 60.f) {
            isSinking = true;
        }
    }

    ball.update(dt, currentFriction);

    // 7. Wypadnięcie poza mapę
    sf::Vector2f pos = ball.getPosition();
    sf::Vector2u windowSize = m_data->window.getSize();
    // Margines 50px pozwala piłce "wylecieć" zamiast się odbić od krawędzi
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
    // === MECHANIKA RAMP I SKOKU ===
    // ==========================================
    float speed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);

    if (!isJumping) {
        bool triggerJump = false;
        float minSpeedForJump = 80.0f;

        if (speed > minSpeedForJump) {
            if (tileUnderBall == 13 && ball.getVelocity().y < 0) triggerJump = true;
            else if (tileUnderBall == 14 && ball.getVelocity().y > 0) triggerJump = true;
            else if (tileUnderBall == 15 && ball.getVelocity().x > 0) triggerJump = true;
            else if (tileUnderBall == 16 && ball.getVelocity().x < 0) triggerJump = true;
        }

        if (triggerJump) {
            isJumping = true;
            float dynamicTime = speed / 300.0f;
            if (dynamicTime < 0.6f) dynamicTime = 0.6f;
            if (dynamicTime > 1.3f) dynamicTime = 1.3f;
            jumpTimer = dynamicTime;
            maxJumpTime = dynamicTime;
        }
    }

    // Animacja lotu
    if (isJumping) {
        jumpTimer -= dt;
        float jumpProgress = 1.0f - (jumpTimer / maxJumpTime);
        float scaleFactor = 1.0f + (0.5f * std::sin(jumpProgress * std::numbers::pi_v<float>));

        float shadowScale = 1.0f - (scaleFactor - 1.0f);
        m_shadow.setScale(shadowScale, shadowScale);

        // Przesunięcie cienia zależne od wysokości skoku
        float shadowOffset = (scaleFactor - 1.0f) * 20.0f;
        m_shadow.setPosition(ball.getPosition().x + 5.0f + shadowOffset, ball.getPosition().y + 5.0f + shadowOffset);

        ball.setScale(scaleFactor);

        if (jumpTimer <= 0.0f) {
            isJumping = false;
            ball.setScale(1.0f);
            sf::Vector2f landPos = ball.getPosition();
            if (landPos.x < 0 || landPos.x > windowSize.x || landPos.y < 0 || landPos.y > windowSize.y) {
                 ball.stop();
                 ball.setPosition(lastSafePos);
            }
        }
    } else {
        m_shadow.setScale(1.0f, 1.0f);
        m_shadow.setPosition(ball.getPosition().x + 2.0f, ball.getPosition().y + 2.0f);
    }

    // ==================================================
    // === DETEKCJA KOLIZJI (System Sub-tile) ===
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

        // --- FUNKCJA OKREŚLAJĄCA TWARDĄ CZĘŚĆ KAFELKA ---
        auto isSolidPart = [&](int tileId, sf::Vector2f point) -> bool {
            int lx = static_cast<int>(point.x) % 32;
            int ly = static_cast<int>(point.y) % 32;
            if (lx < 0) lx += 32;
            if (ly < 0) ly += 32;

            // Granice dla ścian prostych
            int wallThickness = 7;
            int lowBorder = wallThickness;
            int highBorder = 32 - wallThickness;

            // --- ŚCIANY PROSTE ---
            // 4: Ściana LEWA (czyli zajmuje piksele od 0 do lowBorder)
            if (tileId == 4) return lx < lowBorder;

            // 3: Ściana PRAWA (zajmuje piksele od highBorder do 32)
            if (tileId == 3) return lx > highBorder;

            // 6: Ściana GÓRNA (zajmuje piksele od 0 do lowBorder w osi Y)
            if (tileId == 6) return ly < lowBorder;

            // 5: Ściana DOLNA (zajmuje piksele od highBorder do 32 w osi Y)
            if (tileId == 5) return ly > highBorder;


            // 7: Narożnik Lewy-Dół (LD) -> x < lowBorder ORAZ y > highBorder
            if (tileId == 10) return lx < lowBorder && ly > highBorder;

            // 8: Narożnik Prawy-Dół (RD) -> x > highBorder ORAZ y > highBorder
            if (tileId == 9) return lx > highBorder && ly > highBorder;

            // 9: Narożnik Lewy-Góra (LU) -> x < lowBorder ORAZ y < lowBorder
            if (tileId == 8) return lx < lowBorder && ly < lowBorder;

            // 10: Narożnik Prawy-Góra (RU) -> x > highBorder ORAZ y < lowBorder
            if (tileId == 7) return lx > highBorder && ly < lowBorder;

            return true; // Domyślnie pełna ściana
        };

        // --- SPRAWDZANIE KOLIZJI
        bool stdHitTop = isWall(tTop);
        if (stdHitTop && (tTop >= 3 && tTop <= 10)) stdHitTop = isSolidPart(tTop, pTop);

        bool stdHitBottom = isWall(tBottom);
        if (stdHitBottom && (tBottom >= 3 && tBottom <= 10)) stdHitBottom = isSolidPart(tBottom, pBottom);

        bool stdHitLeft = isWall(tLeft);
        if (stdHitLeft && (tLeft >= 3 && tLeft <= 10)) stdHitLeft = isSolidPart(tLeft, pLeft);

        bool stdHitRight = isWall(tRight);
        if (stdHitRight && (tRight >= 3 && tRight <= 10)) stdHitRight = isSolidPart(tRight, pRight);

        int currentTile = getTileAt(ball.getPosition());

        bool rampHitTop = false, rampHitBottom = false, rampHitLeft = false, rampHitRight = false;
        if ((tTop == 14 || tTop == 15 || tTop == 16) && tTop != currentTile) rampHitTop = true;
        if ((tBottom == 13 || tBottom == 15 || tBottom == 16) && tBottom != currentTile) rampHitBottom = true;
        if ((tLeft == 13 || tLeft == 14 || tLeft == 15) && tLeft != currentTile) rampHitLeft = true;
        if ((tRight == 13 || tRight == 14 || tRight == 16) && tRight != currentTile) rampHitRight = true;

        bool hitTop    = stdHitTop || rampHitTop;
        bool hitBottom = stdHitBottom || rampHitBottom;
        bool hitLeft   = stdHitLeft || rampHitLeft;
        bool hitRight  = stdHitRight || rampHitRight;

        // Reakcja
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

    // Reset skali
    if (!ballInHole && !isSinking && !isJumping && tileUnderBall != 11 && tileUnderBall != 1 && tileUnderBall != 12) {
        ball.setScale(1.0f);
    }

    // 8. Dołek
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

    // 9. Level Complete
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

    // 10. Beton
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

// =============================================================
// DRAW & UTILS
// =============================================================

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

    if (ballInHole) m_data->window.draw(winText);
    if (m_alpha > 0.0f) m_data->window.draw(m_fadeRect);
    m_data->window.display();
}

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
    winText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
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

        float angle = std::atan2(force.y, force.x) * 180.f / std::numbers::pi_v<float>;
        aimLine.setRotation(angle);

        float powerRatio = std::min(lineLength * 3.0f / 550.f, 1.0f);
        sf::Uint8 r = 255;
        sf::Uint8 gb = static_cast<sf::Uint8>(255 * (1.0f - powerRatio));
        aimLine.setFillColor(sf::Color(r, gb, gb));
    }
    else {
        aimLine.setSize(sf::Vector2f(0.f, 0.f));
    }
}

int GameState::getTileAt(sf::Vector2f position) {
    if (position.x < 0 || position.y < 0 || position.x >= SCREEN_WIDTH || position.y >= SCREEN_HEIGHT) return 0;

    int gridX = static_cast<int>(position.x) / 32;
    int gridY = static_cast<int>(position.y) / 32;
    if (gridX >= LEVEL_WIDTH || gridY >= LEVEL_HEIGHT) return 0;
    int index = gridX + gridY * LEVEL_WIDTH;
    if (index < 0 || index >= levelData.size()) return 0;
    return levelData[index];
}

bool GameState::isWall(int tileId) {
    return tileId >= 3 && tileId <= 10;
}