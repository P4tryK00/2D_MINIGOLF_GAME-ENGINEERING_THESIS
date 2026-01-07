#include "InputManager.h"
#include <cmath>

InputManager::InputManager()
    : isGrabbed(false), ready(false), ballStartPos(0.f, 0.f),
      currentMousePos(0.f, 0.f), force(0.f)
{
}

void InputManager::handleEvent(const sf::Event& event, Ball& ball, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        if (ball.contains(worldPos)) {
            isGrabbed = true;
            ready = false;
            ballStartPos = ball.getPosition();
            currentMousePos = worldPos;
        }
    }
    else if (event.type == sf::Event::MouseMoved && isGrabbed) {
        sf::Vector2i pixelPos(event.mouseMove.x, event.mouseMove.y);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        currentMousePos = worldPos;
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && isGrabbed) {
        isGrabbed = false;
        ready = true;
    }
}

bool InputManager::isReady() const
{
    return ready;
}

sf::Vector2f InputManager::getForceVector()
{
    if (!ready) return {0.f, 0.f};

    sf::Vector2f diff = ballStartPos - currentMousePos;
    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (length > 0.f) {
        diff /= length;
        float strength = std::min(length * 5.f, 1000.f);
        diff *= strength;
    }

    ready = false;
    return diff;
}

bool InputManager::isDragging() const
{
    return isGrabbed;
}

sf::Vector2f InputManager::getCurrentForce(const sf::RenderWindow& window) const
{
    if (!isGrabbed) return {0.f, 0.f};

    // Obliczamy wektor tak samo jak przy strzale
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    sf::Vector2f diff = ballStartPos - worldPos;

    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (length > 0.f) {
        diff /= length;
        // Używamy tego samego mnożnika co w getForceVector (* 5.f) i limitu (1000.f)
        float strength = std::min(length * 5.f, 1000.f);
        diff *= strength;
    }

    return diff;
}
