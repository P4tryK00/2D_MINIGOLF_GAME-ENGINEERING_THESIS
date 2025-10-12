#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SFML/Graphics.hpp>
#include "Ball.h"


class InputManager {
private:
    bool isGrabbed;
    bool ready;
    sf::Vector2f ballStartPos;
    sf::Vector2f currentMousePos;
    float force;

public:
    InputManager();
    void handleEvent(const sf::Event& event, Ball& ball, sf::RenderWindow& window);
    bool isReady() const;
    sf::Vector2f getForceVector();
    bool isDragging() const;
};

#endif // INPUTMANAGER_H
