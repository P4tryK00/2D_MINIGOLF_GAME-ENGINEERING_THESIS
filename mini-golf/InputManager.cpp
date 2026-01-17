#include "InputManager.h"
#include <cmath>

InputManager::InputManager()
    : isGrabbed(false), ready(false), ballStartPos(0.f, 0.f),
      currentMousePos(0.f, 0.f), force(0.f)
{
}

void InputManager::handleEvent(const sf::Event& event, Ball& ball, sf::RenderWindow& window)
{
    // Kliknięcie myszką: Sprawdzenie czy gracz kliknął w piłkę
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos); // Konwersja pikseli ekranu na współrzędne świata

        if (ball.contains(worldPos)) {
            isGrabbed = true; // Rozpoczęcie celowania
            ready = false;
            ballStartPos = ball.getPosition();
            currentMousePos = worldPos;
        }
    }
    // Ruch myszką: Aktualizacja pozycji celowania
    else if (event.type == sf::Event::MouseMoved && isGrabbed) {
        sf::Vector2i pixelPos(event.mouseMove.x, event.mouseMove.y);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        currentMousePos = worldPos;
    }
    // Puszczenie myszki: Wykonanie strzału
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && isGrabbed) {
        isGrabbed = false;
        ready = true; // Flaga gotowości do pobrania wektora siły przez GameState
    }
}

bool InputManager::isReady() const
{
    return ready;
}

sf::Vector2f InputManager::getForceVector()
{
    if (!ready) return {0.f, 0.f};

    // Obliczenie wektora różnicy
    sf::Vector2f diff = ballStartPos - currentMousePos;

    // Obliczenie długości wektora
    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (length > 0.f) {
        diff /= length; // Normalizacja wektora (kierunek)

        // Obliczenie siły: Długość * mnożnik, z limitem maksymalnym (1000.f)
        float strength = std::min(length * 5.f, 1000.f);
        diff *= strength;
    }

    ready = false; // Reset po oddaniu strzału
    return diff;
}

bool InputManager::isDragging() const
{
    return isGrabbed;
}

sf::Vector2f InputManager::getCurrentForce(const sf::RenderWindow& window) const
{
    if (!isGrabbed) return {0.f, 0.f};

    // Obliczamy wektor tak samo jak przy strzale, ale bez resetowania stanu
    // Używane do rysowania linii celowania (AimLine)
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    sf::Vector2f diff = ballStartPos - worldPos;

    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (length > 0.f) {
        diff /= length;
        float strength = std::min(length * 5.f, 1000.f);
        diff *= strength;
    }

    return diff;
}