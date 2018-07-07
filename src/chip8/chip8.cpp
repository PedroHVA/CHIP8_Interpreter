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

    // Temporary: Keeps program from crashing
    if(pc > CHIP8_MEMORY_MAX) {
        pc = 0;
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
        default:
            break;
    }
}

/**
 * Handles all opcodes that start with 1
 **/
void CHIP8Interpreter::opcode1() {

}

/**
 * Handles all opcodes that start with 2
 **/
void CHIP8Interpreter::opcode2() {
    
}

/**
 * Handles all opcodes that start with 3
 **/
void CHIP8Interpreter::opcode3() {
    
}

/**
 * Handles all opcodes that start with 4
 **/
void CHIP8Interpreter::opcode4() {
    
}

/**
 * Handles all opcodes that start with 5
 **/
void CHIP8Interpreter::opcode5() {
    
}

/**
 * Handles all opcodes that start with 6
 **/
void CHIP8Interpreter::opcode6() {
    
}

/**
 * Handles all opcodes that start with 7
 **/
void CHIP8Interpreter::opcode7() {
    
}

/**
 * Handles all opcodes that start with 8
 **/
void CHIP8Interpreter::opcode8() {
    
}

/**
 * Handles all opcodes that start with 9
 **/
void CHIP8Interpreter::opcode9() {
    
}

/**
 * Handles all opcodes that start with A
 **/
void CHIP8Interpreter::opcodeA() {
    
}

/**
 * Handles all opcodes that start with B
 **/
void CHIP8Interpreter::opcodeB() {
    
}

/**
 * Handles all opcodes that start with C
 **/
void CHIP8Interpreter::opcodeC() {
    
}

/**
 * Handles all opcodes that start with D
 **/
void CHIP8Interpreter::opcodeD() {
    
}

/**
 * Handles all opcodes that start with E
 **/
void CHIP8Interpreter::opcodeE() {
    
}

/**
 * Handles all opcodes that start with F
 **/
void CHIP8Interpreter::opcodeF() {
    
}