#include "Game.h"


const sf::Time Game::timePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
    : mWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), TITLE)
    , ball()
    , inputManager()
    , forceVector(0.f, 0.f)
    , isReady(false)
{

    if (!backgroundTexture.loadFromFile("Resources/grass.png")) {
        std::cerr << "Error: could not load grass.png\n";
    }

    backgroundTexture.setRepeated(true);

    background.setSize(sf::Vector2f(mWindow.getSize()));
    background.setTexture(&backgroundTexture);

    background.setTextureRect(sf::IntRect(0, 0, mWindow.getSize().x, mWindow.getSize().y));

    sf::Vector2u size = mWindow.getSize();
    ball.setPosition(sf::Vector2f(size.x / 2.f, size.y / 2.f));


}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        // Fixed time step loop
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
        if (event.type == sf::Event::Closed)
        {
            mWindow.close();
        }
        inputManager.handleEvent(event, ball, mWindow);
    }
}

void Game::update(sf::Time dt)
{
    if (inputManager.isReady()) {
        forceVector = inputManager.getForceVector();
        ball.applyImpulse(forceVector);
        isReady = true;
    } else {
        isReady = false;
    }
    ball.update(dt.asSeconds());
}

void Game::render()
{
    mWindow.clear();
    mWindow.draw(background);
    ball.draw(mWindow);
    mWindow.display();
}
