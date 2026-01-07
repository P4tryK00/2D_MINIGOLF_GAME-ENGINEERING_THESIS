#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "DEFINITIONS.h"
#include "ResourceManager.h"

class Ball {
private:
    sf::Sprite player;
    sf::Vector2f velocity;

public:
    Ball();
    void update(float dt, float friction);
    void draw(sf::RenderWindow& window);
    void applyImpulse(const sf::Vector2f& impulse);
    bool contains(const sf::Vector2f& point) const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& pos);
    void stop();
    void invertVelocity();
    sf::Vector2f getVelocity() const;
    void setScale(float factor);

};

#endif // BALL_H
