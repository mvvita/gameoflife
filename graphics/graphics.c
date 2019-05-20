#include <stdbool.h>
#include "graphics.h"
#include "SDL.h"
#include <SDL_ttf.h>

//initializes all subsystems
bool initialize() {
	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;
	// initialize TTF
	if (TTF_Init() < 0)
		return false;
	return true;
}