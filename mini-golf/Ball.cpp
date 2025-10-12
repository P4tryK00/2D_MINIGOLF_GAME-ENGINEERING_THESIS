#include "Ball.h"
#include <iostream>

Ball::Ball()
{
    if (!texture.loadFromFile("Resources/ball.png")) {
        std::cerr << "Error: Could not load ball texture\n";
    }
    player.setTexture(texture);
    player.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    velocity = sf::Vector2f(0.f, 0.f);
}

void Ball::update(float dt)
{
    player.move(velocity * dt);
    velocity *= 0.98f;
    if (std::abs(velocity.x) < 0.1f) velocity.x = 0.f;
    if (std::abs(velocity.y) < 0.1f) velocity.y = 0.f;

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
