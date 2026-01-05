#include "Ball.h"
#include <iostream>

Ball::Ball()
{
    const sf::Texture& ballTexture = TextureManager::get("ball");
    player.setTexture(ballTexture);
    player.setOrigin(ballTexture.getSize().x / 2.f, ballTexture.getSize().y / 2.f);
    velocity = sf::Vector2f(0.f, 0.f);
}



void Ball::update(float dt)
{
    player.move(velocity * dt);
    velocity *= 0.98f;


    if (std::abs(velocity.x) < 0.1f) velocity.x = 0.f;
    if (std::abs(velocity.y) < 0.1f) velocity.y = 0.f;

    float radius = TextureManager::get("ball").getSize().x / 2.f;
    sf::Vector2f pos = player.getPosition();


    if (pos.x - radius < 0.f)
    {
        velocity.x = -velocity.x;
        // Rozwiązanie problemu "przyklejania": ręczne ustawienie pozycji piłki na granicy
        player.setPosition(radius, pos.y);
    }
    // Kolizja z prawą ścianą
    else if (pos.x + radius > SCREEN_WIDTH)
    {
        velocity.x = -velocity.x;
        player.setPosition(SCREEN_WIDTH - radius, pos.y);
    }

    // Kolizja z górną ścianą
    if (pos.y - radius < 0.f)
    {
        velocity.y = -velocity.y;
        player.setPosition(pos.x, radius);
    }
    // Kolizja z dolną ścianą
    else if (pos.y + radius > SCREEN_HEIGHT) // SCREEN_HEIGHT to 800
    {
        velocity.y = -velocity.y;
        player.setPosition(pos.x, SCREEN_HEIGHT - radius);
    }
}

void Ball::draw(sf::RenderWindow& window)
{
    window.draw(player);
}

void Ball::applyImpulse(const sf::Vector2f& impulse)
{
    velocity = impulse;
}

bool Ball::contains(const sf::Vector2f& point) const
{
    return player.getGlobalBounds().contains(point);
}

sf::Vector2f Ball::getPosition() const
{
    return player.getPosition();
}

void Ball::setPosition(const sf::Vector2f& pos)
{
    player.setPosition(pos);
}
