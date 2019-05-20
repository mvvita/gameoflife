#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "SDL.h"
#include <SDL_ttf.h>
#include "graphics.h"
#include "menu.h"
#include "constants.h"

GAME_MODE menu(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, GAME_MODE mode, int ind) {

	SDL_Color white = { 255, 255, 255 };

	// preparing text that represents menu options
	SDL_Texture* text[maxChoices];
	SDL_Rect textRect[maxChoices];
	char arr[maxChoices][maxStrSize] =
	{ "Simple game of life",
	  "Coexisting species",
	  "Predator and prey",
	  "Virus spreading",
	  "Unknown to eachother",
	  "History of the game",
	};
	// dimensions of rects that hold menu options
	int wRect = 250;
	int hRect = 50;
	// coordinates of the top rect
	int xStart = WINDOW_W / 2 - wRect / 2;
	int yStart = 50;

	// attaching text to textRects that will be displayed later
	SDL_Surface* textSurface = NULL;
	for (int i = 0; i < maxChoices; i++) {
		textSurface = TTF_RenderText_Solid(font, arr[i], white);
		text[i] = SDL_CreateTextureFromSurface(renderer, textSurface);
		// assigning position and dimensions to textRect
		textRect[i].x = xStart;
		textRect[i].y = yStart;
		textRect[i].w = wRect;
		textRect[i].h = hRect;
		yStart += hRect * 2;
		SDL_QueryTexture(text, NULL, NULL, &textRect[i].w, &textRect[i].h);
		SDL_FreeSurface(textSurface);
		textSurface = NULL;
	}

	// coloring currently available menu option differently
	SDL_SetTextureColorMod(text[ind], 255, 0, 0);

	for (int i = 0; i < maxChoices; i++)
		SDL_RenderCopy(renderer, text[i], NULL, &textRect[i]);
	
	return MENI;
}

