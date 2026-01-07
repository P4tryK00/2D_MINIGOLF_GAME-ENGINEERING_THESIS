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
    // --- 1. DEFINICJA MAPY ---
    // Dostosowana do Twojego tileset.png:
    // 0=Trawa, 1=Dołek, 2=Piasek, 3=Ściana Pionowa, 4=Ściana Pozioma, 5=Narożnik L, 6=Narożnik P
    levelData = {
        5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6, // Góra (Poziome ściany + rogi)
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, // Środek (Pionowe ściany po bokach)
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, // Dołek (1)
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3, // Piasek (2)
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        7, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, // Dół
    };

    tileMap.load("tileset", TILE_SIZE, levelData, LEVEL_WIDTH, LEVEL_HEIGHT);

    // --- 2. START ---
    sf::Vector2u size = mWindow.getSize();
    ball.setPosition(sf::Vector2f(size.x / 2.f, size.y / 2.f + 200.f));

    // Inicjalizacja UI
    initUI();

    aimLine.setSize(sf::Vector2f(0.f, 6.f)); // Długość 0, Grubość 6 pikseli
    aimLine.setOrigin(0.f, 3.f); // Ustawiamy punkt obrotu w połowie grubości (żeby linia wychodziła ze środka)
    aimLine.setFillColor(sf::Color::White);
}

void Game::initUI() {
    // Pobieramy czcionkę z Managera
    // Upewnij się, że w ResourceManagerze załadowałeś czcionkę pod kluczem "font"
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

        // Reset gry klawiszem R
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
             ballInHole = false;
             strokes = 0;
             scoreText.setString("Strokes: 0");
             ball.stop();
             sf::Vector2u size = mWindow.getSize();
             ball.setPosition(sf::Vector2f(size.x / 2.f, size.y / 2.f + 200.f));
        }

        inputManager.handleEvent(event, ball, mWindow);
    }
}

void Game::update(sf::Time dt)
{
    bool ballIsStopped = (std::abs(ball.getVelocity().x) < 5.f && std::abs(ball.getVelocity().y) < 5.f);

    // --- LOGIKA STEROWANIA ---
    if (!ballInHole && ballIsStopped) {
        // Pozwalamy strzelać TYLKO gdy piłka stoi i nie wygraliśmy
        if (inputManager.isReady()) {
            forceVector = inputManager.getForceVector();

            // Ignorujemy mikro-ruch myszką (przypadkowe kliknięcia)
            if (std::abs(forceVector.x) > 1.f || std::abs(forceVector.y) > 1.f) {
                ball.applyImpulse(forceVector);
                strokes++;
                scoreText.setString("Strokes: " + std::to_string(strokes));
            }
        }
    } else {
        // --- POPRAWKA: Czyszczenie bufora wejścia ---
        // Jeśli piłka się rusza, a gracz próbował strzelić, ignorujemy to.
        // Wywołanie getForceVector() resetuje flagę 'ready' w InputManagerze.
        if (inputManager.isReady()) {
            inputManager.getForceVector();
        }
    }

    // --- FIZYKA TERENU ---
    int tileUnderBall = getTileAt(ball.getPosition());
    float currentFriction = 0.98f; // Trawa

    if (tileUnderBall == 2) { // Piasek (ID 2 w nowym tilesecie)
        currentFriction = 0.90f;
    }

    ball.update(dt.asSeconds(), currentFriction);

    // --- KOLIZJE ZE ŚCIANAMI ---
    sf::Vector2f ballPos = ball.getPosition();
    float r = 10.f;
    sf::Vector2f checkPoints[4] = {
        {ballPos.x, ballPos.y - r},
        {ballPos.x, ballPos.y + r},
        {ballPos.x - r, ballPos.y},
        {ballPos.x + r, ballPos.y}
    };

    bool hitWall = false;
    for (int i = 0; i < 4; i++) {
        // Wszystko co ma ID >= 3 to ściana (pionowa, pozioma, róg)
        if (getTileAt(checkPoints[i]) >= 3) {
            hitWall = true;
            break;
        }
    }

    if (hitWall) {
        ball.invertVelocity();
    }

    // --- ULEPSZONY WARUNEK ZWYCIĘSTWA (GRAWITACJA DOŁKA) ---

    // Resetujemy skalę piłki (na wypadek gdyby wyjechała z dołka)
    if (!ballInHole) ball.setScale(1.0f);

    if (tileUnderBall == 1) { // Jesteśmy na kafelku z dołkiem

        // 1. Obliczamy środek tego konkretnego dołka
        int gridX = static_cast<int>(ball.getPosition().x) / 32;
        int gridY = static_cast<int>(ball.getPosition().y) / 32;
        sf::Vector2f holeCenter(gridX * 32.f + 16.f, gridY * 32.f + 16.f);

        // 2. Wektor od piłki do środka dołka
        sf::Vector2f toHole = holeCenter - ball.getPosition();

        // Odległość do środka (używamy kwadratu odległości dla wydajności, ale tu potrzebujemy też długości)
        float distSq = toHole.x * toHole.x + toHole.y * toHole.y;
        float distance = std::sqrt(distSq);

        // 3. Logika "Wysysania"
        // Działa tylko, jeśli piłka jest w promieniu np. 15 pikseli od środka
        if (distance < 15.f) {

            // Sprawdzamy prędkość. Jeśli piłka pędzi jak szalona (> 150), przeskoczy nad dołkiem (lip out).
            // Jeśli ma rozsądną prędkość (< 150), dołek zaczyna ją wciągać.
            float currentSpeed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x +
                                           ball.getVelocity().y * ball.getVelocity().y);

            if (currentSpeed < 150.f) {
                // A. Ciągniemy piłkę do środka (modyfikacja wektora prędkości)
                // Im bliżej środka, tym mocniej ciągnie, ale zwalniamy ją też (tarcie dołka)
                sf::Vector2f pullForce = toHole / distance; // Znormalizowany wektor kierunku

                // Siła przyciągania (eksperymentuj z tą wartością!)
                float pullStrength = 5.0f;

                // Modyfikujemy prędkość bezpośrednio w Ball (musimy to zrobić "brzydko" przez velocity publiczne
                // lub dodać metodę, ale tutaj zrobimy to przez applyImpulse, które masz)
                // UWAGA: applyImpulse w Twoim kodzie *nadpisuje* velocity.
                // Musimy dodać nową metodę `addForce` albo zmodyfikować `velocity` w update Ball.
                // Użyjmy tricku:
                sf::Vector2f newVel = ball.getVelocity() + (pullForce * pullStrength);
                newVel *= 0.90f; // Mocne tarcie wewnątrz dołka
                ball.applyImpulse(newVel); // Aktualizujemy prędkość

                // B. Zmniejszamy piłkę (wizualny efekt wpadania)
                // Im bliżej środka (distance -> 0), tym mniejsza piłka (scale -> 0)
                float scale = std::max(0.0f, distance / 15.f);
                ball.setScale(scale);

                // C. Czy wpadła? (Jest bardzo blisko i bardzo wolna)
                if (distance < 3.0f && currentSpeed < 20.f) {
                    ball.stop();
                    ballInHole = true;
                    ball.setPosition(holeCenter); // Idealny środek na koniec
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
        return 3; // Poza ekranem traktujemy jak ścianę
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

        // 1. Długość linii (skalujemy ją np. przez 3, żeby nie wychodziła poza ekran)
        float lineLength = std::sqrt(force.x*force.x + force.y*force.y) / 3.0f;
        aimLine.setSize(sf::Vector2f(lineLength, 6.f)); // 6.f to grubość linii

        // 2. Pozycja startowa (środek piłki)
        aimLine.setPosition(ballPos);

        // 3. Obrót (Matematyka: zamiana wektora na kąt w stopniach)
        // atan2 zwraca radiany, mnożymy przez 180/PI żeby mieć stopnie
        float angle = std::atan2(force.y, force.x) * 180.f / 3.14159f;
        aimLine.setRotation(angle);

        // 4. Kolor (Gradient od Białego do Czerwonego)
        // Obliczamy moc (0.0 do 1.0)
        float powerRatio = std::min(lineLength * 3.0f / 1000.f, 1.0f);

        sf::Uint8 r = 255;

        // Zielony i Niebieski maleją wraz ze wzrostem siły (z 255 do 0)
        // Dzięki temu zaczynamy od (255,255,255) -> Biały, a kończymy na (255,0,0) -> Czerwony
        sf::Uint8 gb = static_cast<sf::Uint8>(255 * (1.0f - powerRatio));

        aimLine.setFillColor(sf::Color(r, gb, gb));
    }
    else {
        // Chowamy linię (długość 0)
        aimLine.setSize(sf::Vector2f(0.f, 0.f));
    }
}