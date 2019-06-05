#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>
#include "SDL2/SDL.h"
#include "game/game.h"
#include "graphics/menu.h"
#include "graphics/graphics.h"
#include "graphics/gameWindow.h"
#include "graphics/event.h"

int main() {

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* screen = NULL;
	SDL_Event event;
	SDL_Texture* texture = NULL, *game_screen = NULL;
	TTF_Font* font = NULL;
	game* g = NULL;
	Uint32 frameStart = 0;
	int frameTime = 0;

	// initializing all subsystems
	if (initialize() == false)
		return 1;

	//setting up window, renderer, main screen and font
	if (setup(&window, &renderer, &font, &screen, &texture) == false)
		return 1;

	int ind = 0;
	// setting game mode to begin with
	initGame(&g, MENI);

	// main loop that manages events
	handle_events(&window, &renderer, &texture, &font, &event, &ind, &g, frameStart, frameTime);

	// cleanup
	destroyGame(&g);
	cleanup(&window, &font);
	return 0;
}
