#ifndef CONSTANTS_HEADER
#define CONSTANTS_HEADER
/* Game Modes 
		NORMAL - Classic game of life simulation
		COEX - Coexistance, two species coexisting with each other
		PREDATOR - Two species, one is predator (one kills the other)
		VIRUS - Two species, one infects the other 
		UNKNOWN - Two species, both evolve treating the other as dead cells
 */

typedef enum { NORMAL, COEX, PREDATOR, VIRUS, UNKNOWN, MENI, ABOUT } GAME_MODE;

/* Cell Types 
		Matching the above game modes. CELL_COEX_1 & CELL_COEX_2 used in game modes COEX & UNKNOWN. 
 */

typedef enum {CELL_DEAD, CELL_NORMAL, CELL_COEX_1, CELL_COEX_2, CELL_PREDATOR, CELL_VIRUS} CELL_TYPE;

typedef struct coor {
  int x, y;
} coordinates;

#endif
