#include "../constants/constants.h"
#include "../configurations/configurations.h"
#include "../cell/cell.h"
#include "../game/game.h"
#include "../uthash.h"
#include "event.h"
#include "graphics.h"
#include "gameWindow.h"
#include "menu.h"
#include "SDL.h"
#include <stdbool.h>
#include <SDL_ttf.h>

// tracks keyboard events and game state 
void handle_events(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, TTF_Font** font, SDL_Event* event, int* ind, game** g, Uint32 frameStart, int frameTime) {
	CELL_TYPE ctype = CELL_NORMAL;
	bool isRunning = true;
	bool state = false; // simulation not running at the moment
	int redrawRectangles = 1;
	int changedGameMode = 0;
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	int speed = 3;
	int count = 0;
	SDL_Rect returnpos = { WINDOW_W - 50, 10, 30, 30 };
	SDL_Rect playpos = { WINDOW_W - 50, 200, 30, 30 };
	SDL_Rect ctype1pos = { WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30 };
	SDL_Rect ctype2pos = { WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30 };
	cell *p, *temp;
	while (isRunning) {
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
					*ind = (*ind + 1) % maxChoices;
					break;
				case SDLK_UP:
					*ind = (*ind == 0 ? 5 : *ind - 1);
					break;
				case SDLK_SPACE:
					// add case for enter key later, can't find key code
					changedGameMode = 1;
					switch (*ind) {
					case 0:
						(*g)->gameMode = NORMAL;
						// loadConfiguration(g, "../configurations/glider_gun.txt");
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
						(*g)->gameMode = ABOUT;
						break;
					default:
						break;
					}
					break;
					// key for game quit
				case SDLK_ESCAPE:
					isRunning = false;
					break;
					// key for returning to menu
				case SDLK_r:
					destroyGame(g);
					initGame(g, MENI);
					break;
				case SDLK_p:
					state = !state;
					break;
				}
				// handling mouse events
			case SDL_MOUSEBUTTONDOWN:
				if ((*event).motion.x < WINDOW_W*0.95 && (*event).motion.y < WINDOW_H*0.9) {
					if (getCell((*event).motion.x / CELL, (*event).motion.y / CELL, &(*g)->table) == CELL_DEAD)
						addCellToGame((*event).motion.x / CELL, (*event).motion.y / CELL, ctype, *g);
					else
						removeCellFromGame((*event).motion.x / CELL, (*event).motion.y / CELL, *g);
				}else {
					if (CheckIfClickedOn(returnpos, (*event).motion.x, (*event).motion.y)) {
						state = 0;
						destroyGame(g);
						initGame(g, MENI);
						break;
					}
					if (CheckIfClickedOn(playpos, (*event).motion.x, (*event).motion.y)) {
						state = !state;
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
			default:
				break;
			}	
		}

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}

		count++;
		if (state && count >= speed) {
			evolve(*g);
			count = 0;
		}	

		// preparing to show events on screen
		SDL_RenderClear(*renderer);
		SDL_RenderCopy(*renderer, *texture, NULL, NULL);

		switch ((*g)->gameMode)
		{
		case MENI:
			menu(*window, *renderer, *font, (*g)->gameMode, *ind);
			break;
		case NORMAL:
			normal(*window, *renderer, *font, g);
			break;
		case COEX:
			coex(*window, *renderer, *font, g);
			break;
		case PREDATOR:
			predator(*window, *renderer, *font,  g);
			break;
		case VIRUS:
			virus(*window, *renderer, *font, g);
			break;
		case UNKNOWN:
			unknown(*window, *renderer, *font, g);
			break;
		case ABOUT:
			break;
		default:
			break;
		}
		
		// showing events on screen
		SDL_RenderPresent(*renderer);
	}
}
