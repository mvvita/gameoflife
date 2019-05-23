#include "event.h"
#include "SDL.h"
#include "constants.h"
#include <stdbool.h>
#include "graphics.h"
#include <SDL_ttf.h>
#include "gameWindow.h"
#include "cell.h"
#include "game.h"

// tracks keyboard events and game state 
void handle_events(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, TTF_Font** font, bool* isRunning, SDL_Event* event, int* ind, game** g, Uint32 frameStart, int frameTime) {

	CELL_TYPE ctype = NORMAL;
	bool state = false; // simulation not running at the moment
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	SDL_Rect returnpos = { WINDOW_W - 50, 10, 30, 30 };
	SDL_Rect playpos = { WINDOW_W - 50, 200, 30, 30 };
	SDL_Rect ctype1pos = { WINDOW_W*0.45 - 35, WINDOW_H* 0.92, 30, 30 };
	SDL_Rect ctype2pos = { WINDOW_W*0.45 + 5, WINDOW_H* 0.92, 30, 30 };

	frameStart = SDL_GetTicks();
	while (SDL_PollEvent(event) != 0) {
		switch ((*event).type) {
		case SDL_QUIT:
			*isRunning = false;
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
				switch (*ind) {
				case 0:
					(*g)->gameMode = NORMAL;
					ctype = NORMAL;
					break;
				case 1:
					(*g)->gameMode = COEX;
					break;
				case 2:
					(*g)->gameMode = PREDATOR;
					break;
				case 3:
					(*g)->gameMode = VIRUS;
					break;
				case 4:
					(*g)->gameMode = UNKNOWN;
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
				*isRunning = false;
				break;
			// key for returning to menu
			case SDLK_r:
				(*g)->gameMode = MENI;
				break;
			}
		// handling mouse events
		case SDL_MOUSEBUTTONDOWN :
			if((*event).motion.x < WINDOW_W*0.9 && (*event).motion.y < WINDOW_H)
				addCellToGame((*event).motion.x, (*event).motion.y, ctype, *g);
			else {
				if (CheckIfClickedOn(returnpos, (*event).motion.x, (*event).motion.y))
					(*g)->gameMode = MENI;
				if (CheckIfClickedOn(playpos, (*event).motion.x, (*event).motion.y)) {
					if (state) state = false;
					else state = true;
				}
				if (CheckIfClickedOn(ctype1pos, (*event).motion.x, (*event).motion.y))
					switch ((*g)->gameMode)
					{
					case COEX :
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
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	if((*g)->table != NULL && state)
		evolve(*g);

	// preparing to show events on screen
	SDL_RenderClear(*renderer);
	SDL_RenderCopy(*renderer, *texture, NULL, NULL);

	switch ((*g)->gameMode)
	{
	case MENI:
		(*g)->gameMode = menu(*window, *renderer, *font, (*g)->gameMode, *ind);
		break;
	case NORMAL:
		(*g)->gameMode = normal(*window, *renderer, *font, *ind, g);
		break;
	case COEX:
		(*g)->gameMode = coex(*window, *renderer, *font, *ind, g);
		break;
	case PREDATOR:
		(*g)->gameMode = predator(*window, *renderer, *font, *ind, g);
		break;
	case VIRUS:
		(*g)->gameMode = virus(*window, *renderer, *font, *ind, g);
		break;
	case UNKNOWN:
		(*g)->gameMode = unknown(*window, *renderer, *font, *ind, g);
		break;
	case ABOUT:
		break;
	default:
		break;
	}
	// showing events on screen
	SDL_RenderPresent(*renderer);

}