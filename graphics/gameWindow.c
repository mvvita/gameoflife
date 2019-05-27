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
    playerView->x += (WINDOW_W*0.95 / (*cell) / 25) * (*cell);
    playerView->y += (WINDOW_H*0.9 / (*cell) / 25) * (*cell);
		(*cell) += (*cell) / 4;
	}
}

void zoom_out(int* cell, coordinates *playerView) {
	if (*cell > 5) {
		(*cell) -= (*cell) / 4;
    playerView->x -= (WINDOW_W*0.95 / (*cell) / 20) * (*cell);
    playerView->y -= (WINDOW_H*0.9 / (*cell) / 20) * (*cell);
	}
}

void right(coordinates *playerView) {
	playerView->x += 20;
}

void left(coordinates *playerView) {
	playerView->x -= 20;
}

void up(coordinates *playerView) {
	playerView->y -= 20;
}

void down(coordinates *playerView) {
	playerView->y += 20;
}

void setGameWindow(SDL_Window* window, SDL_Renderer* renderer, int CELL, coordinates playerView, int time, int generation) {
	// setting background/frame
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	SDL_RenderClear(renderer);

	// showing time
	TTF_Font *font = TTF_OpenFont("gamecontinue.ttf", 25);
	SDL_Texture* t = NULL;
	SDL_Rect timeRect = { 30,WINDOW_H*0.92 + 5,70,15 };
	SDL_Surface* timeSurface = NULL;
	char tt[30];
	char hp[10];
	strcpy(tt, "time: ");
  sprintf(hp, "%d", time);
	strcat(tt, hp);
	timeSurface = TTF_RenderText_Solid(font, tt, white);
	t = SDL_CreateTextureFromSurface(renderer, timeSurface);
	SDL_QueryTexture(t, NULL, NULL, &timeRect.w, &timeRect.h);
	SDL_FreeSurface(timeSurface);
	timeSurface = NULL;
	SDL_RenderCopy(renderer, t, NULL, &timeRect);
	
	// showing generation
	SDL_Texture* gen = NULL;
	SDL_Rect genRect = { WINDOW_W*0.95-165,WINDOW_H*0.92 + 5,70,15 };
	SDL_Surface* genSurface = NULL;
	char gg[30];
	char num[10];
	strcpy(gg, "generation: ");
  sprintf(num, "%d", generation);
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
  int offset = 0;
  if (playerView.x > 0)
    offset = CELL - (playerView.x % CELL);
  else if (playerView.x < 0)
    offset = abs(playerView.x % CELL);

	for (int x = offset; x < WINDOW_W*0.95; x += CELL)
		SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_H*0.9);

  offset = 0;

  if (playerView.y > 0)
    offset = CELL - (playerView.y % CELL);
  else if (playerView.y < 0)
    offset = abs(playerView.y % CELL);

	for (int y = offset; y < WINDOW_H*0.9; y += CELL)
		SDL_RenderDrawLine(renderer, 0, y, WINDOW_W*0.95, y);
	
	SDL_DestroyTexture(t);
	SDL_DestroyTexture(gen);
	SDL_DestroyTexture(upload.texture);
	SDL_DestroyTexture(play.texture);
	SDL_DestroyTexture(download.texture);
	SDL_DestroyTexture(returnbutt.texture);
  TTF_CloseFont(font);

}

void colorCell(SDL_Renderer* renderer, int x, int y, SDL_Color color, int CELL) {
	SDL_Rect dst;

  if (x < 0) {
    dst.x = 0;
    dst.w = x + CELL;
  } else {
    dst.x = x;
    if (dst.x + CELL > 0.95*WINDOW_W)
      dst.w = 0.95*WINDOW_W - dst.x;
    else
      dst.w = CELL;
  }

  if (y < 0) {
    dst.y = 0;
    dst.h = y + CELL;
  } else {
    dst.y = y;
    if (dst.y + CELL > 0.9*WINDOW_H)
      dst.h = 0.9*WINDOW_H - dst.y;
    else
      dst.h = CELL;
  }

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	
	SDL_RenderFillRect(renderer, &dst);
}

void updateWindow(SDL_Window* window, SDL_Renderer* renderer, cell** table, int CELL, coordinates playerView) {
  int xOffset = 0;
  int yOffset = 0; 

  if (playerView.x > 0)
      xOffset = playerView.x % CELL;
  else if (playerView.x < 0)
      xOffset = CELL - (abs(playerView.x) % CELL);
  
  if (playerView.y > 0)
      yOffset = playerView.y % CELL;
  else if (playerView.y < 0)
      yOffset = CELL - (abs(playerView.y) % CELL);

 	for( int x = -xOffset; x < WINDOW_W*0.95; x += CELL)
 		for ( int y = -yOffset; y < WINDOW_H*0.9; y += CELL) {
 
 			CELL_TYPE type = getCell((x + playerView.x) / CELL, (y + playerView.y) / CELL, table);
 
 			switch (type) {
 			case CELL_DEAD:
 				break;
 			case CELL_NORMAL:
 				colorCell(renderer, x, y, white, CELL);
 				break;
 			case CELL_COEX_1:
 				colorCell(renderer, x, y, blue, CELL);
 				break;
 			case CELL_COEX_2:
 				colorCell(renderer, x, y, green, CELL);
 				break;
 			case CELL_PREDATOR:
 				colorCell(renderer, x, y, red, CELL);
 				break;
 			case CELL_VIRUS:
 				colorCell(renderer, x, y, green, CELL);
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
