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
    // 1. Ładowanie poziomu z obrazka (LevelManager parsuje piksele)
    levelData = LevelManager::loadLevelFromImage(m_level);

    // 2. Generowanie mapy kafelkowej na podstawie danych
    tileMap.load("tileset", TILE_SIZE, levelData, LEVEL_WIDTH, LEVEL_HEIGHT);

    // 3. Ustawienie widoku kamery
    m_data->window.setView(m_data->window.getDefaultView());

    // 4. Ustawienie piłki na pozycji startowej wykrytej przez LevelManager
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

    // 8. Inicjalizacja systemu serc (życia)
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

    // 9. Cień pod piłką (dla efektu głębi przy skoku)
    m_shadow.setRadius(9.f);
    m_shadow.setFillColor(sf::Color(0, 0, 0, 100)); // Półprzezroczysty czarny
    m_shadow.setOrigin(9.f, 9.f);

    updateHearts();
}

// =============================================================
// METODY POMOCNICZE (LOGIKA STANU)
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
    // Reset zmiennych logicznych
    ballInHole = false;
    strokes = 0;
    lives = 3;
    updateHearts();

    scoreText.setString("Strokes: 0");
    ball.stop();

    // Reset pozycji piłki do startu poziomu
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
    // Restart poziomu pod klawiszem R
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
        resetLevel();
    }

    // Pauza pod klawiszem ESC
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        // Robimy zrzut ekranu do tła menu pauzy
        auto texture = std::make_shared<sf::Texture>();
        texture->create(m_data->window.getSize().x, m_data->window.getSize().y);
        texture->update(m_data->window);

        sf::Sprite sprite;
        sprite.setTexture(*texture);

        // Dodajemy stan pauzy na stos (nie usuwamy gry)
        m_data->machine.addState(std::make_unique<PauseMenuState>(m_data, sprite, texture, m_level), false);
    }

    // Obsługa myszy (Drag & Shoot)
    inputManager.handleEvent(event, ball, m_data->window);
}

// =============================================================
// GŁÓWNA PĘTLA LOGIKI (UPDATE)
// =============================================================

void GameState::update(float dt) {

    // 1. Obsługa efektu Fade In (rozjaśnianie ekranu)
    if (m_alpha > 0.0f) {
        m_alpha -= 300.0f * dt;
        if (m_alpha < 0.0f) m_alpha = 0.0f;
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
    }

    // 2. Obsługa tonięcia (animacja zmniejszania piłki)
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
                // GAME OVER - Przejście do stanu przegranej
                auto texture = std::make_shared<sf::Texture>();
                texture->create(m_data->window.getSize().x, m_data->window.getSize().y);
                texture->update(m_data->window);
                sf::Sprite bgSprite(*texture);
                m_data->machine.switchState(std::make_unique<GameOverState>(m_data, bgSprite, texture, m_level));
                return;
            } else {
                // Reset do ostatniej bezpiecznej pozycji
                ball.stop();
                ball.setPosition(lastSafePos);
                ball.setScale(1.0f);
                wasInWater = false;
            }
        }
        return; // Przerywamy update fizyki podczas tonięcia
    }

    // Sprawdzenie czy piłka stoi w miejscu
    bool ballIsStopped = (std::abs(ball.getVelocity().x) < 5.f && std::abs(ball.getVelocity().y) < 5.f);

    // 3. Logika strzału (InputManager)
    if (!ballInHole && ballIsStopped && !isJumping) {
        if (inputManager.isReady()) {
            forceVector = inputManager.getForceVector();
            // Ignorujemy bardzo słabe pociągnięcia
            if (std::abs(forceVector.x) > 1.f || std::abs(forceVector.y) > 1.f) {
                lastSafePos = ball.getPosition(); // Zapisujemy pozycję przed strzałem
                m_hitSound.play();
                ball.applyImpulse(forceVector);
                strokes++;
                scoreText.setString("Strokes: " + std::to_string(strokes));
            }
        }
    } else {
        // Resetujemy input jeśli piłka się rusza
        if (inputManager.isReady()) {
            inputManager.getForceVector();
        }
    }

    int tileUnderBall = getTileAt(ball.getPosition());

    // 4. Obsługa dźwięku wody (tylko raz przy wejściu)
    if (tileUnderBall == 11 && !isJumping) {
        if (!wasInWater) {
            m_splashSound.play();
            wasInWater = true;
        }
    } else if (tileUnderBall != 11) {
        wasInWater = false;
    }

    // 5. Fizyka Tarcia (zależna od podłoża)
    float currentFriction = 0.98f; // Domyślna trawa

    if (isJumping) currentFriction = 0.999f; // W powietrzu opór jest minimalny
    else if (tileUnderBall == 2) currentFriction = 0.90f; // Piasek spowalnia

    // 6. Fizyka Wody (Wektorowe pole sił)
    if (tileUnderBall == 11 && !isJumping) {
        currentFriction = 0.96f;

        // Obliczamy środek kafelka wody
        int gridX = static_cast<int>(ball.getPosition().x) / 32;
        int gridY = static_cast<int>(ball.getPosition().y) / 32;
        sf::Vector2f waterCenter(gridX * 32.f + 16.f, gridY * 32.f + 16.f);

        sf::Vector2f toCenter = waterCenter - ball.getPosition();
        float distance = std::sqrt(toCenter.x * toCenter.x + toCenter.y * toCenter.y);

        // Przyciąganie do środka
        if (distance > 1.0f) {
            sf::Vector2f direction = toCenter / distance;
            float pullStrength = 10.0f;

            // Siła rośnie im bliżej środka, ale słabnie przy dużej prędkości (żeby dało się przeskoczyć)
            float currentSpeed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x +
                                           ball.getVelocity().y * ball.getVelocity().y);

            if (currentSpeed > 300.f) pullStrength *= 0.1f;
            else if (currentSpeed > 100.f) pullStrength *= 0.5f;

            ball.applyImpulse(ball.getVelocity() + (direction * pullStrength));
        }

        // Efekt wizualny zanurzania
        float targetScale = std::min(1.0f, std::max(0.6f, distance / 15.f));
        float currentScale = ball.getScale().x;
        float smoothScale = currentScale + (targetScale - currentScale) * 5.0f * dt;
        ball.setScale(smoothScale);

        // Warunek zatonięcia
        float speed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);
        if (distance < 8.0f && speed < 60.f) {
            isSinking = true;
        }
    }

    // Aktualizacja pozycji piłki
    ball.update(dt, currentFriction);

    // 7. Obsługa wypadnięcia poza mapę
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
    // === MECHANIKA RAMP I SKOKU ===
    // ==========================================
    float speed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);

    if (!isJumping) {
        bool triggerJump = false;
        float minSpeedForJump = 80.0f; // Próg prędkości wymagany do skoku

        if (speed > minSpeedForJump) {
            // Sprawdzamy kierunek najazdu na rampę
            if (tileUnderBall == 13 && ball.getVelocity().y < 0) triggerJump = true; // Rampa UP
            else if (tileUnderBall == 14 && ball.getVelocity().y > 0) triggerJump = true; // Rampa DOWN
            else if (tileUnderBall == 15 && ball.getVelocity().x > 0) triggerJump = true; // Rampa RIGHT
            else if (tileUnderBall == 16 && ball.getVelocity().x < 0) triggerJump = true; // Rampa LEFT
        }

        if (triggerJump) {
            isJumping = true;

            // Obliczamy dynamiczny czas skoku na podstawie prędkości
            float dynamicTime = speed / 300.0f;

            // Ograniczamy czas (min 0.6s, max 1.3s)
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
        // Funkcja sinus do symulacji paraboli lotu (zmiana skali)
        float scaleFactor = 1.0f + (0.5f * std::sin(jumpProgress * std::numbers::pi_v<float>));

        // Cień maleje gdy piłka jest wysoko
        float shadowScale = 1.0f - (scaleFactor - 1.0f);
        m_shadow.setScale(shadowScale, shadowScale);
        m_shadow.setPosition(ball.getPosition());

        ball.setScale(scaleFactor);

        if (jumpTimer <= 0.0f) {
            isJumping = false;
            ball.setScale(1.0f);

            // Sprawdzenie lądowania poza mapą
            sf::Vector2f landPos = ball.getPosition();
            if (landPos.x < 0 || landPos.x > windowSize.x || landPos.y < 0 || landPos.y > windowSize.y) {
                 ball.stop();
                 ball.setPosition(lastSafePos);
            }
        }
    } else {
        // Cień w normalnej pozycji
        m_shadow.setScale(1.0f, 1.0f);
        m_shadow.setPosition(ball.getPosition().x + 2.0f, ball.getPosition().y + 2.0f);
    }

    // ==================================================
    // === DETEKCJA KOLIZJI (System Sub-tile) ===
    // ==================================================
    if (!isJumping) {
        sf::Vector2f ballPos = ball.getPosition();
        float r = 10.f; // Promień kolizji

        // Punkty sprawdzania kolizji (Góra, Dół, Lewo, Prawo)
        sf::Vector2f pTop    = {ballPos.x, ballPos.y - r};
        sf::Vector2f pBottom = {ballPos.x, ballPos.y + r};
        sf::Vector2f pLeft   = {ballPos.x - r, ballPos.y};
        sf::Vector2f pRight  = {ballPos.x + r, ballPos.y};

        int tTop    = getTileAt(pTop);
        int tBottom = getTileAt(pBottom);
        int tLeft   = getTileAt(pLeft);
        int tRight  = getTileAt(pRight);

        // Funkcja lambda: Sprawdza czy uderzyliśmy w TWARDĄ część kafelka
        auto isSolidPart = [&](int tileId, sf::Vector2f point) -> bool {
            int lx = static_cast<int>(point.x) % 32;
            int ly = static_cast<int>(point.y) % 32;
            if (lx < 0) lx += 32;
            if (ly < 0) ly += 32;

            int half = 16;

            // Logika dla pół-ścian (naprawia odbijanie się od "powietrza")
            if (tileId == 3) return lx > half; // Ściana Prawa
            if (tileId == 4) return lx < half; // Ściana Lewa
            if (tileId == 5) return ly > half; // Ściana Dół
            if (tileId == 6) return ly < half; // Ściana Góra

            return true; // Pełna ściana
        };

        // Weryfikacja kolizji
        bool stdHitTop = isWall(tTop);
        if (stdHitTop && (tTop >= 3 && tTop <= 6)) stdHitTop = isSolidPart(tTop, pTop);

        bool stdHitBottom = isWall(tBottom);
        if (stdHitBottom && (tBottom >= 3 && tBottom <= 6)) stdHitBottom = isSolidPart(tBottom, pBottom);

        bool stdHitLeft = isWall(tLeft);
        if (stdHitLeft && (tLeft >= 3 && tLeft <= 6)) stdHitLeft = isSolidPart(tLeft, pLeft);

        bool stdHitRight = isWall(tRight);
        if (stdHitRight && (tRight >= 3 && tRight <= 6)) stdHitRight = isSolidPart(tRight, pRight);

        // Blokowanie ramp od złej strony (ściany jednokierunkowe)
        bool rampHitTop = false, rampHitBottom = false, rampHitLeft = false, rampHitRight = false;
        if (tTop == 14 || tTop == 15 || tTop == 16) rampHitTop = true;
        if (tBottom == 13 || tBottom == 15 || tBottom == 16) rampHitBottom = true;
        if (tLeft == 13 || tLeft == 14 || tLeft == 15) rampHitLeft = true;
        if (tRight == 13 || tRight == 14 || tRight == 16) rampHitRight = true;

        bool hitTop    = stdHitTop || rampHitTop;
        bool hitBottom = stdHitBottom || rampHitBottom;
        bool hitLeft   = stdHitLeft || rampHitLeft;
        bool hitRight  = stdHitRight || rampHitRight;

        // Reakcja na kolizję (Odbicie + Korekta pozycji + Dźwięk)
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

    // Reset skali jeśli jesteśmy na zwykłym terenie
    if (!ballInHole && !isSinking && !isJumping && tileUnderBall != 11 && tileUnderBall != 1 && tileUnderBall != 12) {
        ball.setScale(1.0f);
    }

    // 8. Logika Dołka (Hole)
    if (tileUnderBall == 1 && !isJumping) {
        int gridX = static_cast<int>(ball.getPosition().x) / 32;
        int gridY = static_cast<int>(ball.getPosition().y) / 32;
        sf::Vector2f holeCenter(gridX * 32.f + 16.f, gridY * 32.f + 16.f);
        sf::Vector2f toHole = holeCenter - ball.getPosition();
        float distance = std::sqrt(toHole.x * toHole.x + toHole.y * toHole.y);

        // Przyciąganie (magnes) gdy blisko
        if (distance < 20.f) {
            float currentSpeed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);
            if (currentSpeed < 250.f) {
                 currentFriction = 0.94f;
                 sf::Vector2f direction = toHole / distance;
                 float pullStrength = 1.0f + (20.0f - distance) * 0.15f;
                 ball.applyImpulse(ball.getVelocity() + (direction * pullStrength));

                 // Zmniejszanie piłki
                 float scale = std::max(0.0f, distance / 20.f);
                 ball.setScale(scale);

                // Wpadnięcie
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

    // 9. Przejście do ekranu wygranej (Level Complete)
    if (ballInHole) {
        levelFinishTimer += dt;
        if (levelFinishTimer > 1.0f) { // Czekamy 1s po wpadnięciu
            auto texture = std::make_shared<sf::Texture>();
            texture->create(m_data->window.getSize().x, m_data->window.getSize().y);
            texture->update(m_data->window);
            sf::Sprite bgSprite(*texture);
            m_data->machine.switchState(std::make_unique<LevelCompleteState>(m_data, bgSprite, texture, strokes, m_level));
            return;
        }
    }

    // 10. Logika Betonu (Losowe odbicia / nierówności)
    if (tileUnderBall == 12 && !isJumping) {
         currentFriction = 0.99f;
         float speedBeton = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);
         if (speedBeton > 40.f) {
             float rX = ((std::rand() % 200) - 100) / 100.0f;
             float rY = ((std::rand() % 200) - 100) / 100.0f;
             float roughness = 8.0f;
             sf::Vector2f jitter(rX * roughness, rY * roughness);
             ball.applyImpulse(ball.getVelocity() + jitter);

             // Efekt wizualny podskakiwania
             float bounceIntensity = (std::abs(rX) + std::abs(rY)) / 2.0f;
             float bounceScale = 1.0f + (bounceIntensity * 0.06f);
             ball.setScale(bounceScale);
         } else {
             ball.setScale(1.0f);
         }
    }

    // Aktualizacja linii celowania
    updateAimLine();
}

// =============================================================
// METODY RYSOWANIA I INITIALIZACJI POMOCNICZEJ
// =============================================================

void GameState::draw(float dt) {
    m_data->window.clear();

    // Warstwy rysowania
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

    // Rysowanie efektu przejścia na wierzchu
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

        float angle = std::atan2(force.y, force.x) * 180.f / std::numbers::pi_v<float>;
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