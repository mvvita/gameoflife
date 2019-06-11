#ifndef EVENT_HEADER
#define EVENT_HEADER

#include "../constants/constants.h"
#include "../game/game.h"
#include "graphics.h"
#include <stdbool.h>

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

// tracks keyboard events and game state 
void handle_events(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, TTF_Font** font, SDL_Event* event, int* ind, game** g, Uint32 frameStart, int frameTime);

#endif 
