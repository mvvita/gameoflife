#ifndef GAMEWINDOW_HEADER
#define GAMEWINDOW_HEADER

#include <stdbool.h>
#include "SDL.h"
#include <SDL_ttf.h>
#include "../constants/constants.h"
#include "../game/game.h"
#include "graphics.h"

void setGameWindow(SDL_Window* window, SDL_Renderer* renderer);
void colorCell(SDL_Renderer* renderer, int x, int y, SDL_Color color);
void updateWindow(SDL_Window* window, SDL_Renderer* renderer, cell** table);
void normal(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g);
void coex(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g);
void predator(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g);
void virus(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g);
void unknown(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g);

#endif
