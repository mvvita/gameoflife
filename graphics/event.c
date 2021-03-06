#include "../constants/constants.h"
#include "../configurations/configurations.h"
#include "../cell/cell.h"
#include "../game/game.h"
#include "../uthash.h"
#include "event.h"
#include "graphics.h"
#include "gameWindow.h"
#include "../configurations/configurations.h"
#include "menu.h"
#include <stdbool.h>

#ifdef _WIN32
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#endif

#ifdef linux 
#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#endif

// tracks keyboard events and game state 
void handle_events(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, TTF_Font** font, SDL_Event* event, int* ind, game** g, Uint32 frameStart, int frameTime) {
	CELL_TYPE ctype = CELL_NORMAL;
	int CELL = 15;
	int time = 0;
	Uint32 timeCounter = -1;
	coordinates playerView = { 0,0 };
	bool isRunning = true;
	bool state = false; // simulation not running at the moment
	int redrawRectangles = 1;
	int changedGameMode = 0;
	const int FPS = 30;
	const int frameDelay = 1000 / FPS;
	int speed = 2;
	int count = 0;
	int gen = 1;
	SDL_Rect returnpos = { WINDOW_W - 50, 10, 30, 30 };
	SDL_Rect playpos = { WINDOW_W - 50, 200, 30, 30 };
	SDL_Rect ctype1pos = { WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30 };
	SDL_Rect ctype2pos = { WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30 };
	SDL_Rect downloadpos = { WINDOW_W - 50, 50, 30, 30 };
	SDL_Rect uploadpos = { WINDOW_W - 50, 90, 30, 30 };
	SDL_Rect pluspos = { WINDOW_W - 61, 290, 27, 28 };
	SDL_Rect minuspos = { WINDOW_W - 32, 290, 27, 28 };
	cell *p, *temp;
	while (isRunning) {
		if (timeCounter == -1)
			timeCounter = SDL_GetTicks();
		frameStart = SDL_GetTicks();
		while (SDL_PollEvent(event) != 0) {
			switch ((*event).type) {
			case SDL_QUIT:
				isRunning = false;
				break;
				// handling keyboard events
			case SDL_KEYDOWN:
				switch ((*event).key.keysym.sym) {
					// keys that control menu options
				case SDLK_DOWN:
					if ((*g)->gameMode == MENI)
						*ind = (*ind + 1) % maxChoices;
					else
						down(&playerView);
					break;
				case SDLK_UP:
					if ((*g)->gameMode == MENI)
						*ind = (*ind == 0 ? 5 : *ind - 1);
					else
						up(&playerView);
					break;
				case SDLK_LEFT:
					left(&playerView);
					break;
				case SDLK_RIGHT:
					right(&playerView);
					break;
				case SDLK_SPACE:
				case SDLK_RETURN:
					if ((*g)->gameMode == MENI) {
						gen = 1;
						changedGameMode = 1;
						switch (*ind) {
						case 0:
							(*g)->gameMode = NORMAL;
							count = 1;
							break;
						case 1:
							(*g)->gameMode = COEX;
							ctype = CELL_COEX_1;
							break;
						case 2:
							(*g)->gameMode = PREDATOR;
							break;
						case 3:
							(*g)->gameMode = VIRUS;
							break;
						case 4:
							(*g)->gameMode = UNKNOWN;
							ctype = CELL_COEX_1;
							break;
						case 5:
							isRunning = false;
							break;
						default:
							break;
						}
					}
					break;
				// key for returning to menu
				case SDLK_ESCAPE:
					destroyGame(g);
					time = 0;
					initGame(g, MENI);
					state = 0;
					gen = 1;
					break;
				case SDLK_p:
					state = !state;
					timeCounter = -1;
					break;
				}
			// handling mouse events
			case SDL_MOUSEBUTTONDOWN:
				if ((*event).motion.x < WINDOW_W*0.95 && (*event).motion.y < WINDOW_H*0.9) {
					int dx = playerView.x;
					int dy = playerView.y;
					int minusX = (dx + (*event).motion.x) < 0 ? 1 : 0;
					int minusY = (dy + (*event).motion.y) < 0 ? 1 : 0;
					if (getCell(((*event).motion.x + dx)/ CELL - minusX, ((*event).motion.y + dy ) / CELL - minusY, &(*g)->table) == CELL_DEAD)
						addCellToGame(((*event).motion.x + dx)/ CELL - minusX, ((*event).motion.y + dy)/ CELL - minusY, ctype, *g);
					else
						removeCellFromGame(((*event).motion.x + dx) / CELL - minusX, ((*event).motion.y + dy) / CELL - minusY, *g);
				}else {
					if (CheckIfClickedOn(returnpos, (*event).motion.x, (*event).motion.y)) {
						state = 0;
						time = 0;
						ctype = CELL_NORMAL;
						destroyGame(g);
						initGame(g, MENI);
						gen = 1;
						break;
					}
					if (CheckIfClickedOn(playpos, (*event).motion.x, (*event).motion.y)) {
						state = !state;
						timeCounter = -1;
					}
					if (CheckIfClickedOn(pluspos, (*event).motion.x, (*event).motion.y)) {
						if (gen < 10)
							gen++;
					}
					if (CheckIfClickedOn(minuspos, (*event).motion.x, (*event).motion.y)) {
						if (gen > 1)
							gen--;
					}
					if (CheckIfClickedOn(downloadpos, (*event).motion.x, (*event).motion.y)) {
						loadConfigurationWithList(g);
						playerView.x = playerView.y = 0;
						CELL = 15;
						time = 0;
						timeCounter = -1;
						gen = 1;
                        state = 0;
                    }
					if (CheckIfClickedOn(uploadpos, (*event).motion.x, (*event).motion.y)) {
						saveConfiguration(g);
                        state = 0;
						gen = 1;
                    }
					if (CheckIfClickedOn(ctype1pos, (*event).motion.x, (*event).motion.y))
						switch ((*g)->gameMode)
						{
						case COEX:
						case UNKNOWN:
							ctype = CELL_COEX_1;
							break;
						case PREDATOR:
							ctype = CELL_PREDATOR;
							break;
						case VIRUS:
							ctype = CELL_VIRUS;
							break;
						default:
							break;
						}
					if (CheckIfClickedOn(ctype2pos, (*event).motion.x, (*event).motion.y))
						switch ((*g)->gameMode)
						{
						case COEX:
						case UNKNOWN:
							ctype = CELL_COEX_2;
							break;
						case PREDATOR:
							ctype = CELL_NORMAL;
							break;
						case VIRUS:
							ctype = CELL_NORMAL;
							break;
						default:
							break;
						}
				}
				break;
			// handling mouse wheel events
			case SDL_MOUSEWHEEL:
				// mouse wheel scroll up
				if (event->wheel.y > 0)
					zoom_in(&CELL, &playerView);
				// mouse wheel scroll down
				if (event->wheel.y < 0)
					zoom_out(&CELL, &playerView);
				break;
			default:
				break;
			}	
		}

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
		if (SDL_GetTicks() - timeCounter >= 1000 && state) {
			timeCounter = -1;
			time++;
		}

		count++;
		if (state && count >= speed) {
			evolveByN(*g, gen);
			count = 0;
		}	

		// preparing to show events on screen
		SDL_RenderClear(*renderer);
		SDL_RenderCopy(*renderer, *texture, NULL, NULL);

		switch ((*g)->gameMode)
		{
		case MENI:
			playerView.x = 0; playerView.y = 0;
			CELL = 15;
			menu(*window, *renderer, *font, (*g)->gameMode, *ind);
			break;
		case NORMAL:
			normal(*window, *renderer, *font, g, CELL, playerView, time);
			break;
		case COEX:
			coex(*window, *renderer, *font, g, CELL, playerView, time);
			break;
		case PREDATOR:
			predator(*window, *renderer, *font,  g, CELL, playerView, time);
			break;
		case VIRUS:
			virus(*window, *renderer, *font, g, CELL, playerView, time);
			break;
		case UNKNOWN:
			unknown(*window, *renderer, *font, g, CELL, playerView, time);
			break;
		default:
			break;
		}
		
		// showing events on screen
		SDL_RenderPresent(*renderer);
	}
}
