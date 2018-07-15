#include "input.hpp"

SDL_Event event;

bool inputPoll(int (&key)[16]) {
    bool quit = false;

    while(SDL_PollEvent(&event)) {
        // A key was pressed (true) or released (false)
        bool key_press = (event.type == SDL_KEYDOWN) || (event.type == SDL_KEYUP);

        // If a quit event was pressed (like pressing the 'x' on a window)
        if(event.type == SDL_QUIT) {
            quit = true;
        }else if(key_press) {
            // Sets the value to 1 if a key was pressed, 0 otherwise
            int key_down = (event.type == SDL_KEYDOWN) ? 1 : 0;
            // Check whick key the event came from 
            switch(event.key.keysym.sym) {
                case SDLK_1:
                    key[0x1] = key_down;
                    break;
                case SDLK_2:
                    key[0x2] = key_down;;
                    break;
                case SDLK_3:
                    key[0x3] = key_down;
                    break;
                case SDLK_4:
                    key[0xC] = key_down;
                    break;
                case SDLK_q:
                    key[0x4] = key_down;
                    break;
                case SDLK_w:
                    key[0x5] = key_down;
                    break;
                case SDLK_e:
                    key[0x6] = key_down;
                    break;
                case SDLK_r:
                    key[0xD] = key_down;
                    break;
                case SDLK_a:
                    key[0x7] = key_down;
                    break;
                case SDLK_s:
                    key[0x8] = key_down;
                    break;
                case SDLK_d:
                    key[0x9] = key_down;
                    break;
                case SDLK_f:
                    key[0xE] = key_down;
                    break;
                case SDLK_z:
                    key[0xA] = key_down;
                    break;
                case SDLK_x:
                    key[0x0] = key_down;
                    break;
                case SDLK_c:
                    key[0xB] = key_down;
                    break;
                case SDLK_v:
                    key[0xF] = key_down;
                    break;
                default:
                    break;
            }
        }
    }

    return quit;
}