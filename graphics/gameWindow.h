#ifndef GAMEWINDOW_HEADER
#define GAMEWINDOW_HEADER

#include <stdbool.h>
#include "SDL.h"
#include <SDL_ttf.h>
#include "constants.h"
#include "game.h"
#include "graphics.h"

GAME_MODE normal(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, int ind, game** g);

#endif
