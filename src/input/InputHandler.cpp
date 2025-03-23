#include "InputHandler.h"
#include <iostream>

InputHandler::InputHandler() {
    keymap = {
        SDLK_x,     
        SDLK_1,     
        SDLK_2,     
        SDLK_3,    
        SDLK_q,     
        SDLK_w,    
        SDLK_e,     
        SDLK_a,     
        SDLK_s,     
        SDLK_d,     
        SDLK_z,    
        SDLK_c,     
        SDLK_4,     
        SDLK_r,     
        SDLK_f,     
        SDLK_v      
    };
}

void InputHandler::handleInput(std::array<uint8_t, 16>& key) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            for (int i = 0; i < 16; i++) {
                if (event.key.keysym.sym == keymap[i]) {
                    key[i] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                    std::cout << "Tecla " << i << " pressionada: " << key[i] << std::endl;
                }
            }
        }

        if (event.type == SDL_QUIT) {
            exit(0);
        }
    }
}