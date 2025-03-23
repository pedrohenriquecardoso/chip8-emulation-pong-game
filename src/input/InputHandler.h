#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL2/SDL.h>
#include <array> 

class InputHandler {
public:
    InputHandler();
    void handleInput(std::array<uint8_t, 16>& key);

private:
    std::array<SDL_Keycode, 16> keymap;
};

#endif