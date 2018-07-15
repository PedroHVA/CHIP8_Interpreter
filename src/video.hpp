#ifndef VIDEO_H
#define VIDEO_H

#include <SDL.h>

/**
 * Initialize SDL window and settings
 * 
 * @param	width	Width of the window
 * @param	height	Height of the window
 * @return			True if initialization succesfull
 **/
bool videoInit(int width, int height);

/**
 * Close SDL window and free related memory
 **/
void videoClose();

/**
 * Draws the the CHIP-8 display to the game screen
 * 
 * @param	display		A 64x32 2D array containing information about the CHIP-8 display
 **/
void videoDraw(uint8_t display[64][32]);

#endif // VIDEO_H