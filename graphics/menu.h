#ifndef MENU_DISPLAY
#define MENU_DISPLAY

#include "SDL.h"
#include "SDL_ttf.h"
#include "constants.h"

GAME_MODE menu(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, GAME_MODE mode, int ind);

#endif
