#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <iostream>

#include "chip8.hpp"

// ==================================================================================================
// Variables
// ==================================================================================================
// Default font for each character that can be displayed
static uint8_t fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// ==================================================================================================
// Public Functions
// ==================================================================================================
/** 
 * Performs preliminary operations to start the interpreter
 **/
CHIP8Interpreter::CHIP8Interpreter() {
    reset();
}

/**
 * Resets the memory and pointers in the CHIP8
 **/
void CHIP8Interpreter::reset() {
    opcode = 0;
    I = 0;
    timer_delay = 0;
    timer_sound = 0;
    sp = 0;
    pc = 0x200;
    srand(time(NULL));

    // Clear memory
    for(int i=0; i<CHIP8_MEMORY_MAX; i++) {
        memory[i] = 0;
    }

    // Clear registers, stack and keys
    for(int i=0; i<16; i++) {
        V[i] = 0;
        stack[i] = 0;
        key[i] = 0;
    }

    // Clear the display buffer
    clearDisplay();

    // Load font
    for(int i=0; i<80; i++) {
        memory[i] = fontset[i];
    }
}

/**
 * Steps through one CPU cycle for the interpreter
 **/
void CHIP8Interpreter::step() {
    // An opcode is 4 bytes long, therefore need to merge
    opcode = (memory[pc] << 8) | memory[pc+1];
    pc += 2;

    // std::cout << "Opcode: " << std::hex << std::uppercase << opcode << std::nouppercase << std::dec << std::endl;
    // Execute the opcode 
    ((this)->*opcodeTable[(opcode & 0xF000) >> 12])();

    // Temporary: Keeps program from crashing
    if(pc > CHIP8_MEMORY_MAX) {
        pc = 0;
    }

    // Update timers 
    if(timer_flag) {
        if(timer_delay > 0) {
            timer_delay--;
        }
        if(timer_sound > 0) {
            timer_sound--;
        }
    }
}

/**
 * Sets all values in the display buffer to 0
 **/
void CHIP8Interpreter::clearDisplay() {
    // Clear the display buffer
    for(int x=0; x<CHIP8_SCREEN_WIDTH; x++) {
        for(int y=0; y<CHIP8_SCREEN_HEIGHT; y++) {
            display[x][y] = 0;
        }
    }
}

// ==================================================================================================
// Opcodes
// ==================================================================================================
/**
 * Handles all opcodes that start with 0
 **/
void CHIP8Interpreter::opcode0() {
    switch(opcode) {
        case 0x00E0:
            clearDisplay();
            break;
        case 0x00EE:
            // Return from a subroutine
            sp--;
            pc = stack[sp];
            break;
        default:
            break;
    }
}

/**
 * Handles all opcodes that start with 1
 **/
void CHIP8Interpreter::opcode1() {
    // 0x1NNN - Jump to address NNN
    pc = (0x0FFF & opcode);
}

/**
 * Handles all opcodes that start with 2
 **/
void CHIP8Interpreter::opcode2() {
    // 0x2NNN - Execute subroutine starting at address NNN
    stack[sp++] = pc;
    pc = (0x0FFF & opcode);
}

/**
 * Handles all opcodes that start with 3
 **/
void CHIP8Interpreter::opcode3() {
    // 0x3XNN - Skip the following instruction if the value of register VX equals NN
    if(V[(0x0F00 & opcode) >> 8] == (0x00FF & opcode)) {
        pc += 2;
    }
}

/**
 * Handles all opcodes that start with 4
 **/
void CHIP8Interpreter::opcode4() {
    // 0x4XNN - Skip the following instruction if the value of register VX is not equal to NN
    if(V[(0x0F00 & opcode) >> 8] != (0x00FF & opcode)) {
        pc += 2;
    }
}

/**
 * Handles all opcodes that start with 5
 **/
void CHIP8Interpreter::opcode5() {
    // 0x5XY0 - Skip the following instruction if the value of register VX is equal to the value of register VY
    if(V[(0x0F00 & opcode) >> 8] == V[(0x00F0 & opcode) >> 4]) {
        pc += 2;
    }
}

/**
 * Handles all opcodes that start with 6
 **/
void CHIP8Interpreter::opcode6() {
    // 0x6XNN - Store number NN in register VX
    V[(0x0F00 & opcode) >> 8] = (0x00FF & opcode);
}

/**
 * Handles all opcodes that start with 7
 **/
void CHIP8Interpreter::opcode7() {
    // 0x7XNN - Add the value NN to register VX
    V[(0x0F00 & opcode) >> 8] += (0x00FF & opcode);
}

/**
 * Handles all opcodes that start with 8
 **/
void CHIP8Interpreter::opcode8() {
    uint8_t n = 0x000F & opcode;
    uint8_t X = (0x0F00 & opcode) >> 8;
    uint8_t Y = (0x00F0 & opcode) >> 4;
    switch(n) {
        case 0x0000:
            // 0x8XY0 - Store the value of register VY in register VX
            V[X] = V[Y];
            break;
        case 0x0001:
            // 0x8XY1 - Set VX to VX OR VY
            V[X] = V[X] | V[Y];
            break;
        case 0x0002:
            // 0x8XY2 - Set VX to VX AND VY
            V[X] = V[X] & V[Y];
            break;
        case 0x0003:
            // 0x8XY3 - Set VX to VX XOR VY
            V[X] = V[X] ^ V[Y];
            break;
        case 0x0004:
            // 0x8XY4 - Add the value of register VY to register VX
            // Set VF to 01 if a carry occurs
            V[0xF] = (V[X] + V[Y]) > 0xFF;
            V[X] += V[Y];
            break;
        case 0x0005:
            // 0x8XY5 - Subtract the value of register VY from register VX
            // Set VF to 00 if a borrow occurs
             V[0xF] = V[X] >= V[Y];
             V[X] -= V[Y];
             break;
        case 0x0006:
            // 0x8XY6 - Store the value of register VY shifted right one bit in register VX
            // Set register VF to the least significant bit prior to the shift
            V[0xF] = 0x01 & V[Y];
            V[X] = V[Y] >> 1;
            break;
        case 0x0007:
            // 0x8XY7 - Set register VX to the value of VY minus VX
            // Set VF to 00 if a borrow occurs
            V[0xF] = V[Y] >= V[X];
            V[X] = V[Y] - V[X];
            break;
        case 0x000E:
            // 0x8XYE - Store the value of register VY shifted left one bit in register VX
            // Set register VF to the most significant bit prior to the shift
            V[0xF] = V[Y] >> 7;
            V[X] = V[Y] << 1;
            break;
        default:
            break;
    }
}

/**
 * Handles all opcodes that start with 9
 **/
void CHIP8Interpreter::opcode9() {
    // 9XY0 - Skip the following instruction if the value of register VX is not equal to the value of register VY
    if(V[(0x0F00 & opcode) >> 8] != V[(0x00F0 & opcode) >> 4]) {
        pc += 2;
    }
}

/**
 * Handles all opcodes that start with A
 **/
void CHIP8Interpreter::opcodeA() {
    // ANNN - Store memory address NNN in register I
    I = 0x0FFF & opcode;
}

/**
 * Handles all opcodes that start with B
 **/
void CHIP8Interpreter::opcodeB() {
    // BNNN - Jump to address NNN + V0
    pc = V[0] + (0x0FFF & opcode);
}

/**
 * Handles all opcodes that start with C
 **/
void CHIP8Interpreter::opcodeC() {
    // CXNN - Set VX to a random number with a mask of NN
    V[(0x0F00 & opcode) >> 8] = (rand() % 0xFF) & (0x00FF & opcode);
}

/**
 * Handles all opcodes that start with D
 * 
 * Draw a sprite at position VX, VY with N bytes of sprite data starting 
 * at the address stored in I 
 * Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
 **/
void CHIP8Interpreter::opcodeD() {
    V[0xF] = 0;
    uint8_t X = (0x0F00 & opcode) >> 8;
    uint8_t Y = (0x00F0 & opcode) >> 4;
    
    // DXYN - Draw a sprite at position (VX,VY)
    // The corresponding graphic on the screen will be eight pixels wide and N pixels high.
    uint8_t N = 0x000F & opcode;
    for(int sprite_y = 0; sprite_y < N; sprite_y++) {
        // Get the 8-bit pixel data that represents this row in the sprite
        uint8_t pixel = memory[I + sprite_y];
        for(int sprite_x = 0; sprite_x < 8; sprite_x++) {
            // Check if the current x value in the pixel data is a 1 (ie. should be drawn)
            if((pixel & (0x80 >> sprite_x)) != 0) {
                // Test for collision
                if(display[V[X] + sprite_x][V[Y] + sprite_y]) {
                    V[0xF] = 1;
                }
                display[V[X] + sprite_x][V[Y] + sprite_y] ^= 1;
            }
        }
    }

    draw_flag = 1;
}

/**
 * Handles all opcodes that start with E
 **/
void CHIP8Interpreter::opcodeE() {
    uint8_t X = (0x0F00 & opcode) >> 8;

    // EX9E - Skip the following instruction if the key corresponding to the hex 
    // value currently stored in register VX is pressed
    if((0x00FF & opcode) == 0x009E) {
        if(key[V[X]]) {
            pc += 2;
        }
    }
    
    // EXA1 - Skip the following instruction if the key corresponding to the hex 
    // value currently stored in register VX is not pressed
    if((0x00FF & opcode) == 0x00A1) {
        if(!key[V[X]]) {
            pc += 2;
        }
    }
}

/**
 * Handles all opcodes that start with F
 **/
void CHIP8Interpreter::opcodeF() {
    uint8_t n = 0x00FF & opcode;
    uint8_t X = (0x0F00 & opcode) >> 8;
    switch(n) {
        case 0x0007:
            // FX07 - Sets VX to the value of the delay timer
            V[X] = timer_delay;
            break;
        case 0x000A:
        {
            // FX0A - A key press is awaited, and then stored in VX. 
            // (Blocking Operation. All instruction halted until next key event)
            bool key_pressed = false;
            for(int i=0; i < 16; i++) { 
                if(key[i] != 0) {
                    key_pressed = true;
                    V[X] = i;
                }
            }
            // Repeat this instruction if key wasn't pressed
            if(!key_pressed) {
                pc -= 2;
            }
            break;
        }
        case 0x0015:
            // FX15 - Set the delay timer to the value of register VX
            timer_delay = V[X];
            break;
        case 0x0018:
            // FX18 - Set the sound timer to the value of register VX
            timer_sound = V[X];
            break;
        case 0x001E:
            // FX1E - Add the value stored in register VX to register I
            // Set VF to 1 if overflow
            V[0xF] = (I + V[X] > 0xFFF) ? 1 : 0;
            I += V[X];
        case 0x0029:
            // FX29 - Set I to the memory address of the sprite data corresponding 
            // to the hexadecimal digit stored in register VX
            // Point I to an image of a hex character for the low 4 bits of the value 
            // of register VX. The image is 4 pixels wide and 5 pixels high
            I = V[X] * 5;
            break;  
        case 0x0033:
            // FX33 - Store the binary-coded decimal equivalent of the value stored in 
            // register VX at addresses I, I+1, and I+2
            memory[I] = V[X] / 100;
            memory[I + 1] = (V[X] / 10) % 10;
            memory[I + 2] = (V[X] % 100) % 10;
            break;
        case 0x0055:
            // FX55 - Store the values of registers V0 to VX inclusive in memory starting 
            // at address I. 
            for(int i = 0; i <= X; i++) {
                memory[I + i] = V[i];
            }
            // I is set to I + X + 1 after operation
            I += X + 1;
            break;
        case 0x0065:
            // FX65 - Fill registers V0 to VX inclusive with the values stored in memory starting at address I
            for(int i = 0; i <= X; i++) {
                V[i] = memory[I + i];
            }
            // I is set to I + X + 1 after operation
            I += X + 1;
            break;
        default:
            break;
    }
}

/**
 * Load the contents of a CHIP-8 file into memory.
 * 
 * @param   filename    The name of the file to load
 * @return              true if succesfully loaded the file
 **/
bool CHIP8Interpreter::loadRom(const char *filename) {
    reset();

    // Open the file
    FILE *file = fopen(filename, "rb");
    if(file == NULL) {
        return false;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    unsigned long file_size = ftell(file);
    rewind(file);

    // Create a buffer to hold the file contents
    char *buffer = (char*)malloc(sizeof(char) * file_size);
    if(buffer == NULL) {
        return false;
    }

    // Copy the file into the buffer
    size_t result = fread(buffer, 1, file_size, file);
    if(result != file_size) {
        return false;
    }

    // Copy buffer to the CHIP-8 memory
    if((CHIP8_MEMORY_MAX - 0x200) > file_size) {
        for(unsigned int i = 0; i < file_size; i++) {
            memory[i + 0x200] = buffer[i];
        }
    }

    // Close the file, free buffer
    fclose(file);
    free(buffer);

    return true;
}