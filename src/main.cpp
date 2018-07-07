#include <iostream>

#include "chip8.hpp"

int main() {
    CHIP8Interpreter chip8;

    while(1) {
        chip8.step();
    }

    return 0;
}