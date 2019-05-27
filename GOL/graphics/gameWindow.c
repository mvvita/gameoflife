#include <stdbool.h>
#include <string.h>
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "graphics.h"
#include "../constants/constants.h"
#include "../game/game.h"
#include <math.h>

SDL_Color white = { 255, 255, 255 };
SDL_Color red = { 255, 0, 0 };
SDL_Color green = { 121, 201, 66 };
SDL_Color blue = { 141, 207, 239 };

void zoom_in(int* cell, coordinates *playerView) {
	if (*cell < 30) {
		(*cell) += (*cell) / 4;
	}
}

void zoom_out(int* cell, coordinates *playerView) {
	if (*cell > 5) {
		(*cell) -= (*cell) / 4;
	}
}

void right(coordinates *playerView) {
	playerView->x += 60;
}

void left(coordinates *playerView) {
	playerView->x -= 60;
}

void up(coordinates *playerView) {
	playerView->y -= 60;
}

void down(coordinates *playerView) {
	playerView->y += 60;
}

void setGameWindow(SDL_Window* window, SDL_Renderer* renderer, int CELL, coordinates playerView, int time, int generation) {
	// setting background/frame
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	SDL_RenderClear(renderer);

	// showing time
	TTF_Font *font = TTF_OpenFont("gamecontinue.ttf", 25);
	SDL_Texture* t;
	SDL_Rect timeRect = { 30,WINDOW_H*0.92 + 5,70,15 };
	SDL_Surface* timeSurface = NULL;
	char tt[30];
	char hp[10];
	strcpy(tt, "time: ");
	_itoa(time, hp, 10);
	strcat(tt, hp);
	timeSurface = TTF_RenderText_Solid(font, tt, white);
	t = SDL_CreateTextureFromSurface(renderer, timeSurface);
	SDL_QueryTexture(t, NULL, NULL, &timeRect.w, &timeRect.h);
	SDL_FreeSurface(timeSurface);
	timeSurface = NULL;
	SDL_RenderCopy(renderer, t, NULL, &timeRect);
	
	// showing generation
	SDL_Texture* gen;
	SDL_Rect genRect = { WINDOW_W*0.95-165,WINDOW_H*0.92 + 5,70,15 };
	SDL_Surface* genSurface = NULL;
	char gg[30];
	char num[10];
	strcpy(gg, "generation: ");
	_itoa(generation, num, 10);
	strcat(gg, num);
	genSurface = TTF_RenderText_Solid(font, gg, white);
	gen = SDL_CreateTextureFromSurface(renderer, genSurface);
	SDL_QueryTexture(gen, NULL, NULL, &genRect.w, &genRect.h);
	SDL_FreeSurface(genSurface);
	genSurface = NULL;
	SDL_RenderCopy(renderer, gen, NULL, &genRect);

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

	SDL_Rect p = { 0, 0, WINDOW_W*0.95, WINDOW_H*0.9 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &p);
	// drawing grid lines
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	for (int x = CELL - (abs(playerView.x) % CELL); x < WINDOW_W*0.95; x += CELL)
		SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_H*0.9);
	for (int y = CELL -( abs(playerView.y) % CELL); y < WINDOW_H*0.9; y += CELL)
		SDL_RenderDrawLine(renderer, 0, y, WINDOW_W*0.95, y);
	
	SDL_DestroyTexture(t);
	SDL_DestroyTexture(upload.texture);
	SDL_DestroyTexture(play.texture);
	SDL_DestroyTexture(download.texture);
	SDL_DestroyTexture(returnbutt.texture);

}

void colorCell(SDL_Renderer* renderer, int x, int y, SDL_Color color, int CELL, coordinates playerView) {
	// initializing rectangle to represent colored cell
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (x == 0) {
		if (playerView.x % CELL != 0)
		{
			if (playerView.x < 0)
				dst.w = CELL - (playerView.x % CELL);
			else
				dst.w = playerView.x % CELL;
		}
		else
			dst.w = CELL;
	} else 
		dst.w = CELL > (WINDOW_W*0.95 - x) ? (WINDOW_W*0.95 - x) : CELL;

	if (y == 0) {
		if (playerView.y % CELL != 0)
		{
			if (playerView.y < 0)
				dst.h = CELL - (playerView.y % CELL);
			else
				dst.h = playerView.y % CELL;
		}
		else
			dst.h = CELL;
	}
	else
		dst.h = CELL > (WINDOW_H*0.9 - y) ? (WINDOW_H*0.9 - y) : CELL;

	// 
	// 

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	
	SDL_RenderFillRect(renderer, &dst);
}

void updateWindow(SDL_Window* window, SDL_Renderer* renderer, cell** table, int CELL, coordinates playerView) {
	for( int i = 0; i*CELL < WINDOW_W*0.95; i ++)
		for ( int j = 0; j*CELL < WINDOW_H*0.9; j++) {
			int minusX = (i*CELL - playerView.x) < 0 ? 1 : 0;
			int minusY = (j*CELL - playerView.y) < 0 ? 1 : 0;
			CELL_TYPE type = getCell((i*CELL + playerView.x) / CELL, (j*CELL + playerView.y) / CELL, table);
			switch (type) {
			case CELL_DEAD:
				break;
			case CELL_NORMAL:
				colorCell(renderer, i*CELL, j*CELL, white, CELL, playerView);
				break;
			case CELL_COEX_1:
				colorCell(renderer, i*CELL, j*CELL, blue, CELL, playerView);
				break;
			case CELL_COEX_2:
				colorCell(renderer, i*CELL, j*CELL, green, CELL, playerView);
				break;
			case CELL_PREDATOR:
				colorCell(renderer, i*CELL, j*CELL, red, CELL, playerView);
				break;
			case CELL_VIRUS:
				colorCell(renderer, i*CELL, j*CELL, green, CELL, playerView);
				break;
			}
		}
}

void normal(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time) {
	setGameWindow(window, renderer, cell, playerView, time, (*g)->generation);
	updateWindow(window, renderer, &((*g)->table), cell, playerView);
	// setting full screen as viewport again
}

void coex(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time) {

	setGameWindow(window, renderer, cell, playerView, time, (*g)->generation);
	updateWindow(window, renderer, &((*g)->table), cell, playerView);

	// adding buttons for different cell types
	button co1;
	co1 = InitButton(false, WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	co1.texture = IMG_LoadTexture(renderer, "blue.png");
	SDL_RenderCopy(renderer, co1.texture, NULL, &co1.position);
	button co2;
	co2 = InitButton(false, WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	co2.texture = IMG_LoadTexture(renderer, "green.png");
	SDL_RenderCopy(renderer, co2.texture, NULL, &co2.position);

	SDL_DestroyTexture(co1.texture);
	SDL_DestroyTexture(co2.texture);
}

void predator(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time) {

	setGameWindow(window, renderer, cell, playerView, time, (*g)->generation);
	updateWindow(window, renderer, &((*g)->table), cell, playerView);

	// adding buttons for different cell types
	button predator;
	predator = InitButton(false, WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	predator.texture = IMG_LoadTexture(renderer, "red.png");
	SDL_RenderCopy(renderer, predator.texture, NULL, &predator.position);
	button prey;
	prey = InitButton(false, WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	prey.texture = IMG_LoadTexture(renderer, "white.png");
	SDL_RenderCopy(renderer, prey.texture, NULL, &prey.position);
	SDL_DestroyTexture(predator.texture);
	SDL_DestroyTexture(prey.texture);
}

void virus(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time) {

	setGameWindow(window, renderer, cell, playerView, time, (*g)->generation);
	updateWindow(window, renderer, &((*g)->table), cell, playerView);
	
	// adding buttons for different cell types
	button virus;
	virus = InitButton(false, WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	virus.texture = IMG_LoadTexture(renderer, "green.png");
	SDL_RenderCopy(renderer, virus.texture, NULL, &virus.position);
	button succ;
	succ = InitButton(false, WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	succ.texture = IMG_LoadTexture(renderer, "white.png");
	SDL_RenderCopy(renderer, succ.texture, NULL, &succ.position);
	SDL_DestroyTexture(virus.texture);
	SDL_DestroyTexture(succ.texture);
}

void unknown(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time) {

	setGameWindow(window, renderer, cell, playerView, time, (*g)->generation);
	updateWindow(window, renderer, &((*g)->table), cell, playerView);

	// adding buttons for different cell types
	button un1;
	un1 = InitButton(false, WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	un1.texture = IMG_LoadTexture(renderer, "blue.png");
	SDL_RenderCopy(renderer, un1.texture, NULL, &un1.position);
	button un2;
	un2 = InitButton(false, WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	un2.texture = IMG_LoadTexture(renderer, "green.png");
	SDL_RenderCopy(renderer, un2.texture, NULL, &un2.position);
	SDL_DestroyTexture(un1.texture);
	SDL_DestroyTexture(un2.texture);
}