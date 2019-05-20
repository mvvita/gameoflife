#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include "SDL.h"
#include <SDL_ttf.h>

// window dimensions
#define WINDOW_W 1300
#define WINDOW_H 700

// number of choices from menu
#define maxChoices 6
// word lenght limit
#define maxStrSize 25

//initializes all subsystems
bool initialize();

#endif
