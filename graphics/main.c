#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL_ttf.h>
#include "SDL.h"
#include "graphics.h"
#include "menu.h"

int main() {

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* screen = NULL;
	SDL_Event event;
	SDL_Texture* texture = NULL, *game_screen = NULL;
	TTF_Font* font = NULL;

	// initializing all subsystems
	if (initialize() == false)
		return 1;

	// loading font
	font = TTF_OpenFont("arial.ttf", 25);
	if (font == NULL)
		return 1;
	
	// creating window and renderer
	window = SDL_CreateWindow("Game of life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_RESIZABLE);
	if (window == NULL)
		return 1;
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
		return 1;

	//setting up main screen
	screen = SDL_GetWindowSurface(window);
	SDL_SetSurfaceColorMod(screen, 0, 0, 0);
	texture = SDL_CreateTextureFromSurface(renderer, screen);
	bool isRunning = true;
	int ind = 0;
	
	// setting game mode to begin with
	GAME_MODE mode = MENI;

	// main loop that manages events
	while (isRunning) {
		while (SDL_PollEvent(&event) != 0){
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
				// handling keyboard events
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_DOWN:
					ind = (ind + 1) % maxChoices;
					break;
				case SDLK_UP:
					ind = (ind == 0 ? 5 : ind - 1);
					break;
				case SDLK_SPACE:
				// add case for enter key later, can't find key code
					switch (ind) {
					case 0:
						mode = NORMAL;
						Uint32 blue = SDL_MapRGB(screen->format, 0, 0, 255);
						SDL_FillRect(screen, &screen, blue);
						SDL_RenderDrawRect(renderer, &screen);
						game_screen = SDL_CreateTextureFromSurface(renderer, game_screen);
						break;
					case 1:
						mode = COEX;
						break;
					case 2:
						mode = PREDATOR;
						break;
					case 3:
						mode = VIRUS;
						break;
					case 4:
						mode = UNKNOWN;
						break;
					case 5:
						mode = ABOUT;
						break;
					default:
						break;
					}
					break;
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				}
			default:
				break;
			}
		}

		// preparing to show events on screen
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		switch (mode)
		{
		case MENI:
			mode = menu(window, renderer, font, mode, ind);
			break;
		case NORMAL:
			break;
		case COEX:
			break;
		case PREDATOR:
			break;
		case VIRUS:
			break;
		case UNKNOWN:
			break;
		case ABOUT:
			break;
		default:
			break;
		}
		// showing events on screen
		SDL_RenderPresent(renderer);
	}

	// cleanup
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	return 0;
}