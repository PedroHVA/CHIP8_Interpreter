#include <stdlib.h>
#include <stdint.h>
#include <iostream>

#include "video.hpp"

//The window we'll be rendering to
SDL_Window* window = NULL;
//The window renderer
SDL_Renderer* renderer = NULL;

/**
 * Initialize SDL window and settings
 * 
 * @param	width	Width of the window
 * @param	height	Height of the window
 * @return			True if initialization succesfull
 **/
bool videoInit(int width, int height) {
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    } else {
        //Create window
		window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if(window == NULL) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		} else {
			//Create renderer for window - Vsync disabled
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if(renderer == NULL) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				return false;
			} else {
				// Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				// Clear screen
				SDL_RenderClear(renderer);
			}
		}
    }

    return true;
}

/**
 * Close SDL window and free related memory
 **/
void videoClose() {
    //Destroy window
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	renderer = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}

/**
 * Draws the the CHIP-8 display to the game screen
 * 
 * @param	display		A 64x32 2D array containing information about the CHIP-8 display
 **/
void videoDraw(uint8_t display[64][32]) {
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);

	// Window dimensions
	int window_width = 0;
	int window_height = 0;
	SDL_GetWindowSize(window, &window_width, &window_height);

	// Draw the game screen	
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	int size = (int)SDL_floor(window_height / 32);
	for(int y=0; y<32; y++) {
		for(int x=0; x<64; x++) { 
			if(display[x][y]) {
				SDL_Rect sprite = {x*size, y*size, size, size};
				SDL_RenderFillRect(renderer, &sprite);
			}
		}
	}

	//Update screen
	SDL_RenderPresent(renderer);
}