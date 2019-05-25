#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include "SDL.h"
#include <SDL_ttf.h>
#include <stdbool.h>
// window dimensions
#define WINDOW_W 1300
#define WINDOW_H 700

// cell w/h
#define CELL 15

// number of choices from menu
#define maxChoices 6
// word lenght limit
#define maxStrSize 25

typedef struct Button {
	SDL_Texture* texture;
	SDL_Rect position;
	bool clicked;
} button;

button InitButton(bool click, int x, int y, int width, int height);
void clearButton(button *b);
//initializes all subsystems
bool initialize();

//sets up window, renderer, main screen and font
bool setup(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font, SDL_Surface** screen, SDL_Texture** texture);

// closes all subsystems
void cleanup(SDL_Window** window, TTF_Font** font);

// returns true if mouse is positioned over texture rectangle
bool CheckIfClickedOn(SDL_Rect position, int mouse_x, int mouse_y);

#endif
