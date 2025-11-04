#include "Game.h"


const sf::Time Game::timePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
    : mWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), TITLE)
    , ball()
    , inputManager()
    , forceVector(0.f, 0.f)
{

    background.setTexture(&TextureManager::get("grass"));
    background.setSize(sf::Vector2f(mWindow.getSize()));
    background.setTextureRect(sf::IntRect(0, 0, mWindow.getSize().x, mWindow.getSize().y));

    sf::Vector2u size = mWindow.getSize();
    ball.setPosition(sf::Vector2f(size.x / 2.f, size.y / 2.f));


    hole.setTexture(&TextureManager::get("hole"));
    hole.setRadius(18.f);
    hole.setOrigin(18.f, 18.f);


    hole.setPosition(sf::Vector2f(size.x / 2.f, size.y / 2.f + 150.f));
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
    }
    ball.update(dt.asSeconds());
}

void Game::render()
{
    mWindow.clear();
    mWindow.draw(background);
    mWindow.draw(hole);
    ball.draw(mWindow);
    mWindow.display();
}
