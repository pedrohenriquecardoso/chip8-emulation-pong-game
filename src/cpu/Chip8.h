#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <string>
#include <array>  

class Chip8 {
public:
    uint8_t gfx[64 * 32];  
    bool drawFlag;
    std::array<uint8_t, 16> key;

    Chip8();
    void initialize();
    void emulateCycle();
    void loadGame(const std::string& filename);

private:
    uint16_t opcode;      
    uint8_t memory[4096];  
    uint8_t V[16];        
    uint16_t I;            
    uint16_t pc;           
    uint8_t delay_timer;   
    uint8_t sound_timer;   
    uint16_t stack[16];    
    uint8_t sp;            
};

#endif