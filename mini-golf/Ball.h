#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "DEFINITIONS.h"


class Ball {
private:
    sf::Sprite player;
    sf::Texture texture;
    sf::Vector2f velocity;

public:
    Ball();
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void applyImpulse(const sf::Vector2f& impulse);
    bool contains(const sf::Vector2f& point) const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& pos);
};

#endif // BALL_H
