#include "Renderer.h"
#include <iostream>

Renderer::Renderer() : window(nullptr), renderer(nullptr) {}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    std::cout << "Renderer initialized successfully." << std::endl;
}

void Renderer::render(uint8_t* gfx) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < 64 * 32; i++) {
        if (gfx[i]) {
            SDL_Rect rect = { (i % 64) * 10, (i / 64) * 10, 10, 10 };
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);
}