#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define CHIP8_MEMORY_MAX    4096
#define CHIP8_SCREEN_WIDTH  64
#define CHIP8_SCREEN_HEIGHT 32

/**
 * Emulates a CHIP-8 interpreter by providing functions to execute a 
 * loaded program in the rom
 **/
class CHIP8Interpreter {
    uint16_t opcode;    // Current loaded opcode
    uint16_t pc;        // Program counter
    uint16_t I;         // Index register

    uint8_t memory[CHIP8_MEMORY_MAX];   // CPU memory (Program rom and work ram: 0x200-0xFFF)
    uint8_t V[16];                      // Registers                                              
    uint8_t display[CHIP8_SCREEN_WIDTH][CHIP8_SCREEN_HEIGHT];   // Screen buffer

    uint16_t stack[16]; // Stack where the program counter is stored - has 16 levels
    uint16_t sp;        // Stack pointer - the current level in the stack we're at

    uint8_t key[16];    // The state of each key for the Chip-8 keypad

    // Timers that count down - To emulate accurately, they should count at 60 Hz
    uint8_t timer_delay;
    uint8_t timer_sound;

    public:
        CHIP8Interpreter();
        void reset();
        void step();
        void clearDisplay();

    private:
        // ======================================== Opcode Functions ========================================  
        void opcode0();
        void opcode1();
        void opcode2();
        void opcode3();
        void opcode4();
        void opcode5();
        void opcode6();
        void opcode7();
        void opcode8();
        void opcode9();
        void opcodeA();
        void opcodeB();
        void opcodeC();
        void opcodeD();
        void opcodeE();
        void opcodeF();

        typedef void (CHIP8Interpreter::*OpcodeFunc)();
        /**
         * A table of function pointers. Every CHIP-8 opcode falls in the range 0x0nnn to 0xFnnn therefore
         * this table holds the functions that handle opcodes that start with 0x0 until 0xF.
         **/
        OpcodeFunc opcodeTable[16] = {
            opcode0, opcode1, opcode2, opcode3, opcode4, opcode5, opcode6, opcode7, 
            opcode8, opcode9, opcodeA, opcodeB, opcodeC, opcodeD, opcodeE, opcodeF
        };
};

#endif // CHIP8_H