#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "graphics.h"
#include "../constants/constants.h"
#include "../game/game.h"
#include <math.h>

SDL_Color white = { 204, 204, 204 };
SDL_Color red = { 255, 0, 0 };
SDL_Color green = { 121, 201, 66 };
SDL_Color blue = { 141, 207, 239 };
SDL_Color black = { 0, 0, 0 };
SDL_Color sidebar = { 240, 240, 240 };

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
	TTF_Font *font = TTF_OpenFont("graphics/gamecontinue.ttf", 25);
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
	SDL_Rect genRect = { 250,WINDOW_H*0.92 + 5,70,15 };
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

	// speed
	char sp[10];
	strcpy(sp, "speed");
	SDL_Texture* s = NULL;
	SDL_Rect speedRect = { WINDOW_W - 63, 265, 50, 20 };
	SDL_Surface* speedSurface = NULL;
	speedSurface = TTF_RenderText_Solid(font, sp, white);
	s = SDL_CreateTextureFromSurface(renderer, speedSurface);
	SDL_QueryTexture(s, NULL, NULL, &speedRect.w, &speedRect.h);
	SDL_FreeSurface(speedSurface);
	speedSurface = NULL;
	SDL_RenderCopy(renderer, s, NULL, &speedRect);

	// showing instructions
	SDL_Texture* ins = NULL;
	SDL_Surface* iSurface = NULL;

	char ins1[50];
	strcpy(ins1, "arrows - moving on the grid");
	SDL_Rect ins1Rect = { WINDOW_W*0.45 + 65, WINDOW_H* 0.91, 340, 12 };
	iSurface = TTF_RenderText_Solid(font, ins1, red);
	ins = SDL_CreateTextureFromSurface(renderer, iSurface);
	SDL_QueryTexture(ins, NULL, NULL, &ins1Rect.w, &ins1Rect.h);
	SDL_FreeSurface(iSurface);
	iSurface = NULL;
	SDL_RenderCopy(renderer, ins, NULL, &ins1Rect);

	char ins2[50];
	strcpy(ins2, "mouse wheel - zoom in/out");
	SDL_Rect ins2Rect = { WINDOW_W*0.45 + 65, WINDOW_H* 0.91 + 20, 340, 12 };
	iSurface = TTF_RenderText_Solid(font, ins2, red);
	ins = SDL_CreateTextureFromSurface(renderer, iSurface);
	SDL_QueryTexture(ins, NULL, NULL, &ins2Rect.w, &ins2Rect.h);
	SDL_FreeSurface(iSurface);
	iSurface = NULL;
	SDL_RenderCopy(renderer, ins, NULL, &ins2Rect);

	char ins3[50];
	strcpy(ins3, "R - return to menu");
	SDL_Rect ins3Rect = { WINDOW_W*0.45 + 400, WINDOW_H* 0.91, 340, 12 };
	iSurface = TTF_RenderText_Solid(font, ins3, red);
	ins = SDL_CreateTextureFromSurface(renderer, iSurface);
	SDL_QueryTexture(ins, NULL, NULL, &ins3Rect.w, &ins3Rect.h);
	SDL_FreeSurface(iSurface);
	iSurface = NULL;
	SDL_RenderCopy(renderer, ins, NULL, &ins3Rect);

	char ins4[50];
	strcpy(ins4, "P - pause the simulation");
	SDL_Rect ins4Rect = { WINDOW_W*0.45 + 400, WINDOW_H* 0.91 + 20, 340, 12 };
	iSurface = TTF_RenderText_Solid(font, ins4, red);
	ins = SDL_CreateTextureFromSurface(renderer, iSurface);
	SDL_QueryTexture(ins, NULL, NULL, &ins4Rect.w, &ins4Rect.h);
	SDL_FreeSurface(iSurface);
	iSurface = NULL;
	SDL_RenderCopy(renderer, ins, NULL, &ins4Rect);
	
	// setting up all buttons
	button returnbutt;
	returnbutt = InitButton(WINDOW_W - 50, 10, 30, 30);
	returnbutt.texture = IMG_LoadTexture(renderer, "graphics/return.png");
	SDL_RenderCopy(renderer, returnbutt.texture, NULL, &returnbutt.position);
	button download;
	download = InitButton(WINDOW_W - 50, 50, 30, 30);
	download.texture = IMG_LoadTexture(renderer, "graphics/download.png");
	SDL_RenderCopy(renderer,download.texture, NULL, &download.position);
	button upload;
	upload = InitButton(WINDOW_W - 50, 90, 30, 30);
	upload.texture = IMG_LoadTexture(renderer, "graphics/upload.png");
	SDL_RenderCopy(renderer, upload.texture, NULL, &upload.position);
	button play;
	play = InitButton(WINDOW_W - 50, 200, 30, 30);
	play.texture = IMG_LoadTexture(renderer, "graphics/play.png");
	SDL_RenderCopy(renderer, play.texture, NULL, &play.position);
	button plus;
	plus = InitButton(WINDOW_W - 61, 290, 27, 28);
	plus.texture = IMG_LoadTexture(renderer, "graphics/plus.png");
	SDL_RenderCopy(renderer, plus.texture, NULL, &plus.position);
	button minus;
	minus = InitButton(WINDOW_W - 32, 290, 27, 28);
	minus.texture = IMG_LoadTexture(renderer, "graphics/minus.png");
	SDL_RenderCopy(renderer, minus.texture, NULL, &minus.position);

	// setting up simulation part of the screen
	SDL_Rect p = { 0, 0, WINDOW_W*0.95, WINDOW_H*0.9 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &p);
	// drawing grid lines
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	// vertical
	int offset = 0;
	if (playerView.x > 0)
		offset = CELL - (playerView.x % CELL);
	else if (playerView.x < 0)
		offset = abs(playerView.x % CELL);
	for (int x = offset; x < WINDOW_W*0.95; x += CELL)
		SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_H*0.9);
	// horizontal
	offset = 0;
	if (playerView.y > 0)
		offset = CELL - (playerView.y % CELL);
	else if (playerView.y < 0)
		offset = abs(playerView.y % CELL);
	for (int y = offset; y < WINDOW_H*0.9; y += CELL)
		SDL_RenderDrawLine(renderer, 0, y, WINDOW_W*0.95, y);
	
	// cleanup
	SDL_DestroyTexture(t);
	SDL_DestroyTexture(gen);
	SDL_DestroyTexture(s);
	SDL_DestroyTexture(ins);
	clearButton(&play);
	clearButton(&upload);
	clearButton(&download);
	clearButton(&returnbutt);
	clearButton(&plus);
	clearButton(&minus);
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
	co1 = InitButton(WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	co1.texture = IMG_LoadTexture(renderer, "graphics/blue.png");
	SDL_RenderCopy(renderer, co1.texture, NULL, &co1.position);
	button co2;
	co2 = InitButton(WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	co2.texture = IMG_LoadTexture(renderer, "graphics/green.png");
	SDL_RenderCopy(renderer, co2.texture, NULL, &co2.position);

	SDL_DestroyTexture(co1.texture);
	SDL_DestroyTexture(co2.texture);
}

void predator(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time) {

	setGameWindow(window, renderer, cell, playerView, time, (*g)->generation);
	updateWindow(window, renderer, &((*g)->table), cell, playerView);

	// adding buttons for different cell types
	button predator;
	predator = InitButton(WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	predator.texture = IMG_LoadTexture(renderer, "graphics/red.png");
	SDL_RenderCopy(renderer, predator.texture, NULL, &predator.position);
	button prey;
	prey = InitButton(WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	prey.texture = IMG_LoadTexture(renderer, "graphics/white.png");
	SDL_RenderCopy(renderer, prey.texture, NULL, &prey.position);
	SDL_DestroyTexture(predator.texture);
	SDL_DestroyTexture(prey.texture);
}

void virus(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time) {

	setGameWindow(window, renderer, cell, playerView, time, (*g)->generation);
	updateWindow(window, renderer, &((*g)->table), cell, playerView);
	
	// adding buttons for different cell types
	button virus;
	virus = InitButton(WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	virus.texture = IMG_LoadTexture(renderer, "graphics/green.png");
	SDL_RenderCopy(renderer, virus.texture, NULL, &virus.position);
	button succ;
	succ = InitButton(WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	succ.texture = IMG_LoadTexture(renderer, "graphics/white.png");
	SDL_RenderCopy(renderer, succ.texture, NULL, &succ.position);
	SDL_DestroyTexture(virus.texture);
	SDL_DestroyTexture(succ.texture);
}

void unknown(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, game** g, int cell, coordinates playerView, int time) {

	setGameWindow(window, renderer, cell, playerView, time, (*g)->generation);
	updateWindow(window, renderer, &((*g)->table), cell, playerView);

	// adding buttons for different cell types
	button un1;
	un1 = InitButton(WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30);
	un1.texture = IMG_LoadTexture(renderer, "graphics/blue.png");
	SDL_RenderCopy(renderer, un1.texture, NULL, &un1.position);
	button un2;
	un2 = InitButton(WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30);
	un2.texture = IMG_LoadTexture(renderer, "graphics/green.png");
	SDL_RenderCopy(renderer, un2.texture, NULL, &un2.position);
	SDL_DestroyTexture(un1.texture);
	SDL_DestroyTexture(un2.texture);
}
