cmake_minimum_required(VERSION 3.26.4)
project(mini_golf)

include(FETCHCONTENT)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.0)
FETCHCONTENT_MAKEAVAILABLE(SFML)
set(CMAKE_CXX_STANDARD 20)

set(PROJECT_SOURCES
        main.cpp
        Game.cpp
        Game.h
        DEFINITIONS.h
        Ball.cpp
        Ball.h
        InputManager.cpp
        InputManager.h
        TextureManager.cpp
        TextureManager.h
        FontManager.cpp
        FontManager.h
        SoundManager.cpp
        SoundManager.h
        ResourceManager.cpp
        ResourceManager.h
        TileMap.cpp
        TileMap.h
        State.h
        StateManager.cpp
        StateManager.h
        GameState.cpp
        GameState.h
        MainMenuState.cpp
        MainMenuState.h
        PauseMenuState.cpp
        PauseMenuState.h
        LevelSelectState.cpp
        LevelSelectState.h
        LevelManager.cpp
        LevelManager.h
        LevelCompleteState.cpp
        LevelCompleteState.h


)
add_executable(mini_golf ${PROJECT_SOURCES})


target_link_libraries(mini_golf sfml-graphics)
target_link_libraries(mini_golf sfml-audio)