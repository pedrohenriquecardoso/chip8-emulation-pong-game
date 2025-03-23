#include "cpu/Chip8.h"
#include "graphics/Renderer.h"
#include "input/InputHandler.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <array> 

int main() {
    Chip8 chip8;
    Renderer renderer;
    InputHandler inputHandler;

    chip8.initialize();
    renderer.initialize();

    chip8.loadGame("/home/pedro-henrique-cardoso/Desktop/teste/assets/Pong (1 player).ch8");

    const int CYCLES_PER_SECOND = 500;
    const int FRAME_DELAY = 1000 / CYCLES_PER_SECOND;

    bool running = true;
    while (running) {
        Uint32 frameStart = SDL_GetTicks();

        inputHandler.handleInput(chip8.key);

        chip8.emulateCycle();

        if (chip8.drawFlag) {
            renderer.render(chip8.gfx);
            chip8.drawFlag = false;
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    return 0;
}