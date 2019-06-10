#ifndef GAMEWINDOW_HEADER
#define GAMEWINDOW_HEADER

#include <stdbool.h>
#include "SDL.h"
#include <SDL_ttf.h>
#include "../constants/constants.h"
#include "../game/game.h"
#include "graphics.h"

void zoom_in(int* cell, coordinates *playerView);
void zoom_out(int* cell, coordinates *playerView);

void up(coordinates *playerView);
void down(coordinates *playerView);
void left(coordinates *playerView);
void right(coordinates *playerView);

void setGameWindow(SDL_Window* window, SDL_Renderer* renderer, int cell, coordinates playerView, int time, int generation);
void colorCell(SDL_Renderer* renderer, int x, int y, SDL_Color color, int cell);
void updateWindow(SDL_Window* window, SDL_Renderer* renderer, cell** table, int cell, coordinates playerView);
void normal(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time);
void coex(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time);
void predator(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time);
void virus(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time);
void unknown(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time);

#endif
