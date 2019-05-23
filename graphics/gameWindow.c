#include <stdbool.h>
#include <string.h>
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "graphics.h"
#include "constants.h"
#include "game.h"

SDL_Color white = { 255, 255, 255 };
SDL_Color red = { 255, 0, 0 };
SDL_Color green = { 0, 255, 0 };
SDL_Color blue = { 0, 0, 255 };

SDL_Rect setGameWindow(SDL_Window* window, SDL_Renderer* renderer) {
	// setting background/frame
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	SDL_RenderClear(renderer);

	// setting up all buttons
	button returnbutt;
	returnbutt = InitButton(false, WINDOW_W - 50, 10, 30, 30);
	returnbutt.texture = IMG_LoadTexture(renderer, "return.png");
	SDL_RenderCopy(renderer, returnbutt.texture, NULL, &returnbutt.position);
	button download;
	download = InitButton(false, WINDOW_W - 50, 50, 30, 30);
	download.texture = IMG_LoadTexture(renderer, "download.png");
	SDL_RenderCopy(renderer,download.texture, NULL, &download.position);
	button upload;
	upload = InitButton(false, WINDOW_W - 50, 90, 30, 30);
	upload.texture = IMG_LoadTexture(renderer, "upload.png");
	SDL_RenderCopy(renderer, upload.texture, NULL, &upload.position);
	button play;
	play = InitButton(false, WINDOW_W - 50, 200, 30, 30);
	play.texture = IMG_LoadTexture(renderer, "play.png");
	SDL_RenderCopy(renderer, play.texture, NULL, &play.position);

	// preparing grid for the game of life
	SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
	SDL_SetSurfaceColorMod(windowSurface, 0, 0, 0);
	SDL_Texture* background = SDL_CreateTextureFromSurface(renderer, windowSurface);
	// setting up viewport for the game
	SDL_Rect gameViewport;
	gameViewport.x = 0;
	gameViewport.y = 0;
	gameViewport.w = WINDOW_W * 0.9;
	gameViewport.h = WINDOW_H * 0.9;

	SDL_RenderSetViewport(renderer, &gameViewport);

	// rendering texture to screen
	SDL_RenderCopy(renderer, background, &gameViewport, NULL);

	// drawing grid lines
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	for (int x = 0; x < WINDOW_W*0.9; x += CELL)
		SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_H*0.9);
	for (int y = 0; y < WINDOW_H*0.9; y += CELL)
		SDL_RenderDrawLine(renderer, 0, y, WINDOW_W*0.9, y);

	return gameViewport;
}

SDL_Rect colorCell(SDL_Renderer* renderer, SDL_Rect* viewPort, int x, int y, SDL_Color color) {
	// initializing rectangle to represent colored cell
	SDL_Rect dst = { x,y,CELL,CELL };

	SDL_Surface *s;
	s = SDL_CreateRGBSurface(0, viewPort->w, viewPort->h, 0, 0, 0, 0, 255);
	SDL_FillRect(s, &dst, SDL_MapRGB(s->format, color.r, color.g, color.b));
	SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
	SDL_FreeSurface(s);
	
	// rendering texture to screen
	SDL_RenderCopy(renderer, t, NULL, &dst);

	return *viewPort;
}

SDL_Rect updateWindow(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect* gameViewport, cell** table) {
	
	for( int i = 0; i < gameViewport->w; i ++)
		for ( int j = 0; j < gameViewport->h; j++) {
			CELL_TYPE type = getCell(i, j, table);
			switch (type) {
			case CELL_DEAD:
				break;
			case CELL_NORMAL:
				*gameViewport = colorCell(renderer, gameViewport, i, j, white);
				break;
			case CELL_COEX_1:
				*gameViewport = colorCell(renderer, gameViewport, i, j, blue);
				break;
			case CELL_COEX_2:
				*gameViewport = colorCell(renderer, gameViewport, i, j, green);
				break;
			case CELL_PREDATOR:
				*gameViewport = colorCell(renderer, gameViewport, i, j, red);
				break;
			case CELL_VIRUS:
				*gameViewport = colorCell(renderer, gameViewport, i, j, green);
				break;
			}
		}

	return *gameViewport;
}

GAME_MODE normal(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, int ind, game** g) {

	SDL_Rect gameViewport = setGameWindow(window, renderer);
	gameViewport = updateWindow(window, renderer, &gameViewport, &((*g)->table));

	// setting full screen as viewport again
	SDL_RenderSetViewport(renderer, NULL);
	return NORMAL;
}

GAME_MODE coex(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, int ind, game** g) {

	SDL_Rect gameViewport = setGameWindow(window, renderer);
	gameViewport = updateWindow(window, renderer, &gameViewport, &((*g)->table));

	// setting full screen as viewport again
	SDL_RenderSetViewport(renderer, NULL);
	// adding buttons for different cell types
	button co1;
	co1 = InitButton(false, WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	co1.texture = IMG_LoadTexture(renderer, "blue.png");
	SDL_RenderCopy(renderer, co1.texture, NULL, &co1.position);
	button co2;
	co2 = InitButton(false, WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	co2.texture = IMG_LoadTexture(renderer, "green.png");
	SDL_RenderCopy(renderer, co2.texture, NULL, &co2.position);

	return COEX;
}

GAME_MODE predator(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, int ind, game** g) {

	SDL_Rect gameViewport = setGameWindow(window, renderer);
	gameViewport = updateWindow(window, renderer, &gameViewport, &((*g)->table));

	// setting full screen as viewport again
	SDL_RenderSetViewport(renderer, NULL);
	// adding buttons for different cell types
	button predator;
	predator = InitButton(false, WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	predator.texture = IMG_LoadTexture(renderer, "red.png");
	SDL_RenderCopy(renderer, predator.texture, NULL, &predator.position);
	button prey;
	prey = InitButton(false, WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	prey.texture = IMG_LoadTexture(renderer, "white.png");
	SDL_RenderCopy(renderer, prey.texture, NULL, &prey.position);
	
	return PREDATOR;
}

GAME_MODE virus(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, int ind, game** g) {

	SDL_Rect gameViewport = setGameWindow(window, renderer);
	gameViewport = updateWindow(window, renderer, &gameViewport, &((*g)->table));

	// setting full screen as viewport again
	SDL_RenderSetViewport(renderer, NULL);
	// adding buttons for different cell types
	button virus;
	virus = InitButton(false, WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	virus.texture = IMG_LoadTexture(renderer, "green.png");
	SDL_RenderCopy(renderer, virus.texture, NULL, &virus.position);
	button succ;
	succ = InitButton(false, WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	succ.texture = IMG_LoadTexture(renderer, "white.png");
	SDL_RenderCopy(renderer, succ.texture, NULL, &succ.position);
	
	return VIRUS;
}

GAME_MODE unknown(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, int ind, game** g) {

	SDL_Rect gameViewport = setGameWindow(window, renderer);
	gameViewport = updateWindow(window, renderer, &gameViewport, &((*g)->table));

	// setting full screen as viewport again
	SDL_RenderSetViewport(renderer, NULL);
	// adding buttons for different cell types
	button un1;
	un1 = InitButton(false, WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	un1.texture = IMG_LoadTexture(renderer, "blue.png");
	SDL_RenderCopy(renderer, un1.texture, NULL, &un1.position);
	button un2;
	un2 = InitButton(false, WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	un2.texture = IMG_LoadTexture(renderer, "green.png");
	SDL_RenderCopy(renderer, un2.texture, NULL, &un2.position);
	
	return UNKNOWN;
}
