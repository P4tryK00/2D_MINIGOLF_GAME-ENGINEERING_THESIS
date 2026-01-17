#include "Ball.h"

Ball::Ball()
{
    const sf::Texture& ballTexture = TextureManager::get("ball");
    player.setTexture(ballTexture);
    // Ustawiamy punkt centralny (origin) na środku tekstury, aby rotacja i pozycja były liczone od środka
    player.setOrigin(ballTexture.getSize().x / 2.f, ballTexture.getSize().y / 2.f);
    velocity = sf::Vector2f(0.f, 0.f);
}

void Ball::update(float dt, float friction)
{
    // Przesunięcie o wektor prędkości * czas
    player.move(velocity * dt);

    // Aplikacja tarcia (zwalnianie)
    velocity *= friction;

    // Zatrzymanie całkowite, jeśli prędkość jest bardzo mała (zapobiega drganiom)
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

void Ball::stop()
{
    velocity = sf::Vector2f(0.f, 0.f);
}

void Ball::bounceX()
{
    // Odbicie od ściany pionowej:
    // Odwracamy wektor X. Mnożymy przez 0.8f, aby zasymulować utratę energii przy zderzeniu.
    velocity.x = -velocity.x * 0.8f;
}

void Ball::bounceY()
{
    // Odbicie od ściany poziomej:
    // Odwracamy wektor Y, zachowujemy X.
    velocity.y = -velocity.y * 0.8f;
}

void Ball::invertVelocity()
{
    velocity = -velocity;
}

sf::Vector2f Ball::getVelocity() const
{
    return velocity;
}

void Ball::setScale(float factor) {
    player.setScale(factor, factor);
}