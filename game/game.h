#ifndef GAME_HEADER
#define GAME_HEADER

#include "../cell/cell.h"
#include "../constants/constants.h"

typedef struct g {
  GAME_MODE gameMode;
  cell *table;
  int population;
  int generation;
} game;

// Initialize game with a game mode (must be called!)
void initGame(game **g, GAME_MODE gameMode); 

// Frees the memory after usage (return to main menu)
void destroyGame(game **g);

// Calculates the overall next generation 
void evolve(game *g);

// Frees a hash table
void freeHashTable(cell **table);

// Adds a cell to the game
void addCellToGame(int x, int y, CELL_TYPE type, game *g);

// Removes a cell from the game
void removeCellFromGame(int x, int y, game *g);

#endif
