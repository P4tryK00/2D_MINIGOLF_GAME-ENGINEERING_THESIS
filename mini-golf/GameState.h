//
// Created by patry on 13.01.2026.
//

#ifndef MINI_GOLF_GAMESTATE_H
#define MINI_GOLF_GAMESTATE_H


#include "State.h"
#include "Game.h"
#include "Ball.h"
#include "TileMap.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "PauseMenuState.h"
#include <SFML/Audio.hpp>
#include <numbers>
#include "LevelManager.h"
#include "LevelCompleteState.h"
#include "GameOverState.h"

class GameState : public State {
public:
    GameState(GameDataRef data,int level);

    void init() override;
    void handleInput(sf::Event& event) override;
    void update(float dt) override;
    void draw(float dt) override;

private:
    GameDataRef m_data;
    Ball ball;
    InputManager inputManager;
    TileMap tileMap;
    std::vector<int> levelData;
    sf::Vector2f lastSafePos;
    sf::Vector2f forceVector;
    int m_level;
    sf::CircleShape m_shadow;


    // UI
    sf::Text scoreText;
    sf::Text winText;
    sf::RectangleShape aimLine;

    // Zmienne logiczne
    int strokes;
    bool ballInHole;
    bool wasInWater;
    bool isSinking;
    bool isJumping;
    float jumpTimer;
    float levelFinishTimer;


    // Dźwięki
    sf::Sound m_hitSound;
    sf::Sound m_splashSound;
    sf::Sound m_wallHitSound;
    sf::Sound m_winSound;

    // Metody pomocnicze
    void initUI();
    void initSounds();
    void updateAimLine();
    int getTileAt(sf::Vector2f position);
    bool isWall(int tileId);

    //przejscie
    sf::RectangleShape m_fadeRect;
    float m_alpha;

    int lives;
    std::vector<sf::Sprite> heartSprites;
    void updateHearts();
    void resetLevel();

};



#endif //MINI_GOLF_GAMESTATE_H