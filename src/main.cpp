#include <iostream>

#include "chip8.hpp"
#include "video.hpp"
#include "input.hpp"

int main(int argc, char *argv[]) {
    bool exit = false;

    // TODO: Move these to a config file instead
    // Rate that the screen refreshes
    int screen_fps = 60;
    int screen_ticks_per_frame = 1000 / screen_fps;
    // Rate at which the CHIP-8 cpu updates (in Hz) - Has to be larger than screen fps
    int chip8_cpu_freq = 500;
    // How many ticks will the cpu update for every draw call
    int chip8_ticks = chip8_cpu_freq / screen_fps;

    // We'll use this to count the frames per second
    int frames = 0;
    double fps_timer = SDL_GetTicks();

    // Access CHIP-8 memory and cpu
    CHIP8Interpreter chip8;
    chip8.loadRom("TEST");

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
        for(int i = 0; i < chip8_ticks; i++) {
            chip8.step();
        }
        chip8.timerUpdate();

        // Update the screen the CHIP-8 has updated its display
        if(chip8.draw_flag) {
            videoDraw(chip8.display);
            chip8.draw_flag = 0;
        }

        // Calculate FPS
        int fps_ticks = SDL_GetTicks() - fps_timer;
        float fps_avg = frames / (fps_ticks / 1000.f);
        if(fps_avg > 2000000) {
            fps_avg = 0;
        }
        frames++;

        int tick_time = SDL_GetTicks() - start_time;
        // Cap screen refresh time
        if(tick_time < screen_ticks_per_frame) {
            // Wait remaining time 
            SDL_Delay(screen_ticks_per_frame - tick_time);
        }
    }

    // Destroy the SDL window
    videoClose();

    return 0;
}