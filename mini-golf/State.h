//
// Created by patry on 13.01.2026.
//

#ifndef MINI_GOLF_STATE_H
#define MINI_GOLF_STATE_H

#include <SFML/Graphics.hpp>

class State {
public:
    virtual ~State() {}

    virtual void init() = 0;
    virtual void handleInput(sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(float dt) = 0;

    virtual void pause() {}
    virtual void resume() {}
};

#endif //MINI_GOLF_STATE_H