#include "Game.h"


const sf::Time Game::timePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
    : mWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), TITLE)
    , ball()
    , inputManager()
    , forceVector(0.f, 0.f)
{
    tileMap.load("tileset", TILE_SIZE, level, LEVEL_WIDTH, LEVEL_HEIGHT);

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
    mWindow.draw(tileMap);
    ball.draw(mWindow);
    mWindow.display();
}
