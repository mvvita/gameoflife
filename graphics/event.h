#ifndef EVENT_HEADER
#define EVENT_HEADER

#include "SDL.h"
#include "constants.h"
#include <stdbool.h>
#include "graphics.h"
#include <SDL_ttf.h>
#include "game.h"

// tracks keyboard events and game state 
void handle_events(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, TTF_Font** font, bool* isRunning, SDL_Event* event, int* ind, game** g, Uint32 frameStart, int frameTime);

#endif 