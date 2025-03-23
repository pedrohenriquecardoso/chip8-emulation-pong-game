#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

class Renderer {
public:
    Renderer();
    ~Renderer();
    void initialize();
    void render(uint8_t* gfx);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif