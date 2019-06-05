#ifndef MENU_DISPLAY
#define MENU_DISPLAY

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "../constants/constants.h"

// Draws the menu
GAME_MODE menu(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, GAME_MODE mode, int ind);

#endif
