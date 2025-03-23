# Chip8 Emulator with Pong Game

This is a **Chip8 emulator** project that emulates the Chip8 CPU. It includes a working **Pong** game (written in Chip8), along with basic input handling, graphical rendering using **SDL2**.

## Features

- **Chip8 Emulation**: Emulates the Chip8 microprocessor, allowing you to run Chip8 games.
- **2D Graphics Rendering**: Uses **SDL2** to render the game’s display in a window.
- **Pong**: A simple **Pong** game written in Chip8, ready to play.

## Prerequisites

Before running the project, make sure you have the following installed:

- **C++ Compiler** (like `g++` or `clang++`).
- **SDL2** (Simple DirectMedia Layer 2) library for graphical rendering.

### Installing SDL2

#### On Linux (Debian-based):

```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-mixer-dev
sudo apt install cmake -y
```

#### in the file main.cpp change line 16 to your path to the assets folder

### Run

```bash
mkdir build
cd build
cmake ..
make
./Chip8Emulator
```

```bash
/Chip8Emulator
  ├── /src
  │   ├── /cpu
  │   ├── /graphics
  │   ├── /input
  │   └── main.cpp
  ├── /assets
  │   ├── Pong (1 player).ch8
  ├── CMakeLists.txt
  └── README.md
```
