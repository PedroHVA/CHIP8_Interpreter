#include <iostream>

#include "chip8.hpp"

int main() {
    CHIP8Interpreter chip8;

    chip8.loadRom("TETRIS");
    
    while(1) {
        getchar();
        chip8.step();
    }

    return 0;
}