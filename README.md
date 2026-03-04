# 2D Mini-Golf Engineering Thesis

![C++20](https://img.shields.io/badge/C++-20-blue.svg)
![SFML](https://img.shields.io/badge/SFML-2.6.0-green.svg)
![CMake](https://img.shields.io/badge/CMake-Build-orange.svg)
![License](https://img.shields.io/badge/License-MIT-lightgrey.svg)

> **Note:** This project was developed as my Engineering Thesis in Applied Computer Science at the University of Lodz.

## 📖 About The Project

Building a game in a commercial engine like Unity or Unreal is efficient, but it abstracts away the core mechanics of how real-time applications truly work. This project is a deliberate exercise in low-level software engineering.
I built a complete 2D Mini-Golf game and physics engine completely from scratch using **C++20** and the **SFML** multimedia library. The goal was to design a modular architecture, implement custom physics algorithms, and ensure strict memory safety without relying on external physics frameworks.

## ✨ Key Features & Technical Achievements

**Deterministic Physics Engine:** Implemented a fixed time-step game loop using an accumulator. This ensures that physics calculations and ball trajectories are 100% reproducible regardless of the hardware's frame rate.
**Sub-Tile Collision Detection:** Standard AABB collision was insufficient for the precision required in mini-golf.I developed a custom sub-tile collision algorithm that analyzes the ball's position within a tile's local space, allowing for accurate interactions with irregular shapes and concave corners.
**Procedural Level Generation:** Levels are not hardcoded. The `LevelManager` parses RGB pixel data from PNG images to dynamically construct the game world, mapping specific colors to terrain types (grass, sand, water, walls).
**2.5D Jump Mechanics:** Simulated depth and jumping physics over obstacles without adding a true Z-axis, utilizing dynamic sprite scaling, shadow offset, and collision filtering.
**Advanced Resource Management:** Designed a `ResourceManager` utilizing the RAII idiom and C++ smart pointers (`std::unique_ptr`, `std::shared_ptr`) to prevent memory leaks and ensure assets (textures, sounds) are loaded only once and safely shared.
**State Machine Architecture:** Clean separation of game screens (Menu, Gameplay, Pause) using the State design pattern, allowing for safe context switching and paused background rendering.
**Data Persistence:** Custom file I/O system with data validation to persist high scores and unlock progress, preventing crashes from corrupted save files.

## 🛠️ Built With

* **Language:** C++20
* **Multimedia Library:** SFML 2.6.0
* **Build System:** CMake
* **Development Environment:** CLion (with Clangd static analysis)

## 🚀 Getting Started

The project uses CMake's `FetchContent` module. This means you **do not** need to manually install SFML on your system; CMake will download and configure all dependencies automatically.

### Prerequisites
* A C++20 compatible compiler (GCC, Clang, or MSVC)
* CMake 3.26+

### Building the Project
```bash
# 1. Clone the repository
git clone [https://github.com/P4tryK00/MiniGolf-Engine.git](https://github.com/P4tryK00/MiniGolf-Engine.git)
cd MiniGolf-Engine

# 2. Create a build directory
mkdir build && cd build

# 3. Configure and build
cmake ..
cmake --build . --config Release

# 4. Run the game
./mini_golf
