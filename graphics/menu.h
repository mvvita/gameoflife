#ifndef MENU_DISPLAY
#define MENU_DISPLAY

#include "../constants/constants.h"

#ifdef _WIN32
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#endif

#ifdef linux 
#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#endif

// Draws the menu
void menu(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, GAME_MODE mode, int ind);

#endif
