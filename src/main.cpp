#include <iostream>

#include "chip8.hpp"
#include "video.hpp"
#include "input.hpp"

int main(int argc, char *argv[]) {
    bool exit = false;

    // TODO: Move these to a config file instead
    // Rate at which the CHIP-8 cpu updates (in Hz)
    //int chip8_speed = 500;
    // Rate that the screen refreshes
    //int screen_fps = 60;

    // Access CHIP-8 memory and cpu
    CHIP8Interpreter chip8;
    chip8.loadRom("TETRIS");

    // Atempt to create a SDL window
    if(!videoInit(800, 600)) {
        exit = true;
        printf("Video could not be initialized!");
    }
    
    // The main game loop
    while(!exit) {
        int start_time = SDL_GetTicks();

        // Get input from the User. This does not wait for input only reads the event queue
        exit = inputPoll(chip8.key);

        // Process one instruction loaded into the CHIP-8's memory
        chip8.step();

        // Update the screen the CHIP-8 has updated its display
        if(chip8.draw_flag) {
            videoDraw(chip8.display);
            chip8.draw_flag = 0;
        }

        int current_time = SDL_GetTicks() - start_time;
        int sleep_time = (1000 / 60) - current_time;
        if(sleep_time > 0) {
            SDL_Delay(sleep_time);
        }
    }

    // Destroy the SDL window
    videoClose();

    return 0;
}