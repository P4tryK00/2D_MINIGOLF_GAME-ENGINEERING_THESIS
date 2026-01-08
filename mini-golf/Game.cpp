#include "Game.h"

const sf::Time Game::timePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
    : mWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), TITLE)
    , ball()
    , inputManager()
    , forceVector(0.f, 0.f)
    , strokes(0)
    , ballInHole(false)
{
    // --- 1. DEFINICJA MAPY (ZGODNIE Z NOWĄ LEGENDĄ) ---
    // 0=Trawa, 1=Dołek, 2=Piasek, 11=Woda
    // 3=Lewa, 4=Prawa, 5=Góra, 6=Dół
    // 7=LD, 8=PD, 9=LG, 10=PG (Narożniki)

    levelData = {
        9, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 10, // Góra
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, // Dołek (1)
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 4, // Piasek (2)
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 0, 0, 0, 0, 0, 4, // WODA (11)
        3, 0, 0, 0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 8, // Dół
    };

    tileMap.load("tileset", TILE_SIZE, levelData, LEVEL_WIDTH, LEVEL_HEIGHT);

    // --- 2. START ---
    sf::Vector2u size = mWindow.getSize();
    ball.setPosition(sf::Vector2f(size.x / 2.f, size.y / 2.f + 200.f));

    // Zapisz pozycję początkową jako bezpieczną
    lastSafePos = ball.getPosition();

    initUI();

    aimLine.setSize(sf::Vector2f(0.f, 6.f));
    aimLine.setOrigin(0.f, 3.f);
    aimLine.setFillColor(sf::Color::White);
}

void Game::initUI() {
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

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            processEvents();
            update(timePerFrame);
        }
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
             ballInHole = false;
             strokes = 0;
             scoreText.setString("Strokes: 0");
             ball.stop();
             sf::Vector2u size = mWindow.getSize();
             sf::Vector2f startPos(size.x / 2.f, size.y / 2.f + 200.f);
             ball.setPosition(startPos);
             lastSafePos = startPos; // Reset bezpiecznej pozycji
        }

        inputManager.handleEvent(event, ball, mWindow);
    }
}

// Funkcja pomocnicza: Czy kafelek jest ścianą? (ID 3 do 10)
bool Game::isWall(int tileId) {
    return tileId >= 3 && tileId <= 10;
}

void Game::update(sf::Time dt)
{
    bool ballIsStopped = (std::abs(ball.getVelocity().x) < 5.f && std::abs(ball.getVelocity().y) < 5.f);

    // --- LOGIKA STEROWANIA ---
    if (!ballInHole && ballIsStopped) {
        if (inputManager.isReady()) {
            forceVector = inputManager.getForceVector();
            if (std::abs(forceVector.x) > 1.f || std::abs(forceVector.y) > 1.f) {
                // ZAPISUJEMY POZYCJĘ PRZED STRZAŁEM
                lastSafePos = ball.getPosition();

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

    // --- FIZYKA TERENU ---
    int tileUnderBall = getTileAt(ball.getPosition());
    float currentFriction = 0.98f;

    if (tileUnderBall == 2) { // Piasek
        currentFriction = 0.90f;
    }

    // --- WODA (ID 11) - NOWA FIZYKA ---
    if (tileUnderBall == 11) {
        // 1. Obliczamy środek aktualnego kafelka wody
        int gridX = static_cast<int>(ball.getPosition().x) / 32;
        int gridY = static_cast<int>(ball.getPosition().y) / 32;
        sf::Vector2f waterCenter(gridX * 32.f + 16.f, gridY * 32.f + 16.f);

        // 2. Wektor do środka
        sf::Vector2f toCenter = waterCenter - ball.getPosition();
        float distance = std::sqrt(toCenter.x * toCenter.x + toCenter.y * toCenter.y);

        // 3. Wciąganie do środka
        sf::Vector2f pullForce = toCenter / (distance + 0.1f);
        float pullStrength = 3.0f; // Siła nurtu

        // Woda hamuje i ciągnie
        sf::Vector2f newVel = ball.getVelocity() + (pullForce * pullStrength * dt.asSeconds() * 60.f);
        newVel *= 0.90f; // Silne tarcie
        ball.applyImpulse(newVel);

        // 4. Wizualne tonięcie
        // Skalujemy od 1.0 (brzeg) do 0.0 (środek)
        float scale = std::max(0.0f, distance / 16.f);
        ball.setScale(scale);

        // 5. Reset (utonięcie)
        if (distance < 5.0f) {
            ball.stop();
            ball.setPosition(lastSafePos); // WRACAMY NA LĄD
            ball.setScale(1.0f); // Reset rozmiaru
            // strokes++; // Tu możesz odkomentować karę punktową
        }
    }

    ball.update(dt.asSeconds(), currentFriction);

    // --- KOLIZJE ZE ŚCIANAMI ---
    sf::Vector2f ballPos = ball.getPosition();
    float r = 10.f;

    sf::Vector2f pTop    = {ballPos.x, ballPos.y - r};
    sf::Vector2f pBottom = {ballPos.x, ballPos.y + r};
    sf::Vector2f pLeft   = {ballPos.x - r, ballPos.y};
    sf::Vector2f pRight  = {ballPos.x + r, ballPos.y};

    bool hitTop    = isWall(getTileAt(pTop));
    bool hitBottom = isWall(getTileAt(pBottom));
    bool hitLeft   = isWall(getTileAt(pLeft));
    bool hitRight  = isWall(getTileAt(pRight));

    if (hitTop || hitBottom) {
        ball.bounceY();
        if(hitTop) ball.setPosition(sf::Vector2f(ballPos.x, ballPos.y + 1.f));
        if(hitBottom) ball.setPosition(sf::Vector2f(ballPos.x, ballPos.y - 1.f));
    }

    if (hitLeft || hitRight) {
        ball.bounceX();
        if(hitLeft) ball.setPosition(sf::Vector2f(ballPos.x + 1.f, ballPos.y));
        if(hitRight) ball.setPosition(sf::Vector2f(ballPos.x - 1.f, ballPos.y));
    }


    // --- WARUNEK ZWYCIĘSTWA (DOŁEK) ---
    // Przywracamy rozmiar tylko jeśli NIE jesteśmy w wodzie i NIE wygraliśmy
    if (!ballInHole && tileUnderBall != 11) ball.setScale(1.0f);

    if (tileUnderBall == 1) {
        int gridX = static_cast<int>(ball.getPosition().x) / 32;
        int gridY = static_cast<int>(ball.getPosition().y) / 32;
        sf::Vector2f holeCenter(gridX * 32.f + 16.f, gridY * 32.f + 16.f);
        sf::Vector2f toHole = holeCenter - ball.getPosition();
        float distSq = toHole.x * toHole.x + toHole.y * toHole.y;
        float distance = std::sqrt(distSq);

        if (distance < 15.f) {
            float currentSpeed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x +
                                           ball.getVelocity().y * ball.getVelocity().y);

            if (currentSpeed < 150.f) {
                sf::Vector2f pullForce = toHole / distance;
                float pullStrength = 5.0f;
                sf::Vector2f newVel = ball.getVelocity() + (pullForce * pullStrength * dt.asSeconds() * 60.f);
                ball.applyImpulse(newVel);

                float scale = std::max(0.0f, distance / 15.f);
                ball.setScale(scale);

                if (distance < 3.0f && currentSpeed < 20.f) {
                    ball.stop();
                    ballInHole = true;
                    ball.setPosition(holeCenter);
                    ball.setScale(0.f);
                }
            }
        }
    }
    updateAimLine();
}

void Game::render()
{
    mWindow.clear();
    mWindow.draw(tileMap);
    ball.draw(mWindow);
    mWindow.draw(aimLine);
    mWindow.draw(scoreText);
    if (ballInHole) {
        mWindow.draw(winText);
    }
    mWindow.display();
}

int Game::getTileAt(sf::Vector2f position)
{
    if (position.x < 0 || position.y < 0 || position.x >= SCREEN_WIDTH || position.y >= SCREEN_HEIGHT) {
        return 3;
    }

    int gridX = static_cast<int>(position.x) / 32;
    int gridY = static_cast<int>(position.y) / 32;

    if (gridX >= LEVEL_WIDTH || gridY >= LEVEL_HEIGHT) {
        return 3;
    }

    int index = gridX + gridY * LEVEL_WIDTH;
    if (index < 0 || index >= levelData.size()) return 3;

    return levelData[index];
}

void Game::updateAimLine()
{
    bool ballIsStopped = (std::abs(ball.getVelocity().x) < 5.f && std::abs(ball.getVelocity().y) < 5.f);

    if (inputManager.isDragging() && !ballInHole && ballIsStopped) {
        sf::Vector2f ballPos = ball.getPosition();
        sf::Vector2f force = inputManager.getCurrentForce(mWindow);

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