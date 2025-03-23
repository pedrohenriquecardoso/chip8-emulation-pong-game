#include "Chip8.h"
#include <iostream>
#include <fstream>
#include <cstdint>

Chip8::Chip8() {
    initialize();
}

void Chip8::initialize() {
    pc = 0x200;  
    opcode = 0;
    I = 0;
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;
    drawFlag = false;

    for (int i = 0; i < 4096; i++) memory[i] = 0;
    for (int i = 0; i < 16; i++) V[i] = 0;
    key.fill(0); 
    for (int i = 0; i < 64 * 32; i++) gfx[i] = 0;
}

void Chip8::loadGame(const std::string& filename) {
    std::ifstream gameFile(filename, std::ios::binary);
    if (gameFile) {
        gameFile.read(reinterpret_cast<char*>(memory + 0x200), 3584); 
        gameFile.close();
        std::cout << "ROM loaded successfully: " << filename << std::endl;
    } else {
        std::cerr << "Error loading ROM: " << filename << std::endl;
    }
}

void Chip8::emulateCycle() {
    opcode = (memory[pc] << 8) | memory[pc + 1];

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F) {
                case 0x0000:  
                    for (int i = 0; i < 64 * 32; i++) gfx[i] = 0;
                    drawFlag = true;
                    pc += 2;
                    break;

                case 0x000E:  
                    if (sp == 0) {
                        std::cerr << "Stack underflow!" << std::endl;
                        return;
                    }
                    sp--;
                    pc = stack[sp];
                    pc += 2;
                    break;

                default:
                    std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
                    pc += 2;
                    break;
            }
            break;

        case 0x1000:  
            pc = opcode & 0x0FFF;
            break;

        case 0x2000:  
            stack[sp] = pc;
            sp++;
            pc = opcode & 0x0FFF;
            break;

        case 0x3000:  
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t nn = opcode & 0x00FF;
            if (V[x] == nn) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }

        case 0x4000:  
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t nn = opcode & 0x00FF;
            if (V[x] != nn) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }

        case 0x5000:  
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            if (V[x] == V[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }

        case 0x6000:  
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t nn = opcode & 0x00FF;
            V[x] = nn;
            pc += 2;
            break;
        }

        case 0x7000:  
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t nn = opcode & 0x00FF;
            V[x] += nn;
            pc += 2;
            break;
        }

        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    V[x] = V[y];
                    pc += 2;
                    break;
                }

                case 0x0001:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    V[x] |= V[y];
                    pc += 2;
                    break;
                }

                case 0x0002:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    V[x] &= V[y];
                    pc += 2;
                    break;
                }

                case 0x0003:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    V[x] ^= V[y];
                    pc += 2;
                    break;
                }

                case 0x0004:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    uint16_t sum = V[x] + V[y];
                    V[0xF] = (sum > 255) ? 1 : 0;
                    V[x] = sum & 0xFF;
                    pc += 2;
                    break;
                }

                case 0x0005:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    V[0xF] = (V[x] >= V[y]) ? 1 : 0;
                    V[x] -= V[y];
                    pc += 2;
                    break;
                }

                case 0x0006:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    V[0xF] = V[x] & 0x1;
                    V[x] >>= 1;
                    pc += 2;
                    break;
                }

                case 0x0007:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    V[0xF] = (V[y] >= V[x]) ? 1 : 0;
                    V[x] = V[y] - V[x];
                    pc += 2;
                    break;
                }

                case 0x000E:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    V[0xF] = (V[x] >> 7) & 0x1;
                    V[x] <<= 1;
                    pc += 2;
                    break;
                }

                default:
                    std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
                    pc += 2;
                    break;
            }
            break;

        case 0x9000:  
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            if (V[x] != V[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }

        case 0xA000:  
        {
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        }

        case 0xB000:  
        {
            pc = (opcode & 0x0FFF) + V[0];
            break;
        }

        case 0xC000:  
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t nn = opcode & 0x00FF;
            V[x] = (rand() % 256) & nn;
            pc += 2;
            break;
        }

        case 0xD000:  
        {
            uint8_t x = V[(opcode & 0x0F00) >> 8];
            uint8_t y = V[(opcode & 0x00F0) >> 4];
            uint8_t height = opcode & 0x000F;
            uint8_t pixel;
        
            V[0xF] = 0;
        
            for (int yline = 0; yline < height; yline++) {
                pixel = memory[I + yline];
                for (int xline = 0; xline < 8; xline++) {
                    if ((pixel & (0x80 >> xline)) != 0) {
                        int gfx_index = (x + xline + ((y + yline) * 64)) % (64 * 32);  
                        if (gfx[gfx_index] == 1) {
                            V[0xF] = 1;
                        }
                        gfx[gfx_index] ^= 1;
                    }
                }
            }
        
            drawFlag = true;
            pc += 2;
            break;
        }

        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x009E:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    if (key[V[x]]) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    break;
                }

                case 0x00A1:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    if (!key[V[x]]) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    break;
                }

                default:
                    std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
                    pc += 2;
                    break;
            }
            break;

        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x0007:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    V[x] = delay_timer;
                    pc += 2;
                    break;
                }

                case 0x000A:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    bool keyPressed = false;

                    for (int i = 0; i < 16; i++) {
                        if (key[i]) {
                            V[x] = i;
                            keyPressed = true;
                            break;
                        }
                    }

                    if (!keyPressed) {
                        return;  
                    }

                    pc += 2;
                    break;
                }

                case 0x0015:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    delay_timer = V[x];
                    pc += 2;
                    break;
                }

                case 0x0018:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    sound_timer = V[x];
                    pc += 2;
                    break;
                }

                case 0x001E:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    I += V[x];
                    pc += 2;
                    break;
                }

                case 0x0029:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    I = V[x] * 5; 
                    pc += 2;
                    break;
                }

                case 0x0033:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    memory[I] = V[x] / 100;
                    memory[I + 1] = (V[x] / 10) % 10;
                    memory[I + 2] = V[x] % 10;
                    pc += 2;
                    break;
                }

                case 0x0055:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i <= x; i++) {
                        memory[I + i] = V[i];
                    }
                    pc += 2;
                    break;
                }

                case 0x0065:  
                {
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i <= x; i++) {
                        V[i] = memory[I + i];
                    }
                    pc += 2;
                    break;
                }

                default:
                    std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
                    pc += 2;
                    break;
            }
            break;

        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
            pc += 2;
            break;
    }

    if (delay_timer > 0) delay_timer--;
    if (sound_timer > 0) sound_timer--;
}