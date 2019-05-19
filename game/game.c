#include "game.h"
#include "../cell/cell.h"
#include "../constants/constants.h"
#include "../uthash.h"
#include <stdlib.h>
#include <stdio.h>

// Initialize game with a game mode (must be called!)
void initGame(game **g, GAME_MODE gameMode) {
  *g = (game *) malloc(sizeof(game));

  (*g)->gameMode = gameMode;

  // Initialize hash table
  (*g)->table = NULL;

  (*g)->population = 0;
  (*g)->generation = 1;
}

// Frees the memory after usage (return to main menu)
void destroyGame(game **g) {
  // Free hash table
  freeHashTable(&((*g)->table));

  free(*g);

  *g = NULL;
}

// Calculates the overall next generation 
void evolve(game *g) {
  cell *newTable = NULL;
  cell *p, *tmp;
  CELL_TYPE next;

  HASH_ITER(hh, g->table, p, tmp) {
    next = nextGeneration(p->location.x, p->location.y, g->gameMode, &g->table);

    if (next != CELL_DEAD)
      addCell(p->location.x, p->location.y, next, &newTable);

    if (p->cellType != CELL_DEAD && next == CELL_DEAD)
      g->population--;

    if (p->cellType == CELL_DEAD && next != CELL_DEAD)
      g->population++;
  }

  freeHashTable(&g->table);

  g->generation++;
  g->table = newTable;
}

// Frees a hash table
void freeHashTable(cell **table) {
  cell *p, *tmp;

  HASH_ITER(hh, *table, p, tmp) {
    HASH_DEL(*table, p);
    free(p);
  }

  free(*table);

  *table = NULL;
}

// Adds a cell to the game
void addCellToGame(int x, int y, CELL_TYPE type, game *g) {
  cell *p;
  coordinates location;
  location.x = x;
  location.y = y;

  // Check if there is already a cell there
  HASH_FIND(hh, g->table, &location, sizeof(coordinates), p);

  if (p == NULL)
    addCell(x, y, type, &g->table);
  else if (p->cellType == CELL_DEAD && type != CELL_DEAD)
    addCell(x, y, type, &g->table);

  // Increment population count
  g->population++;
}

// Removes a cell from the game
void removeCellFromGame(int x, int y, game *g) {
  cell *p;
  coordinates location;
  location.x = x;
  location.y = y;

  CELL_TYPE cType;

  // Check if there is a cell at given coordinates
  HASH_FIND(hh, g->table, &location, sizeof(coordinates), p);

  if (p == NULL)
    return;
  
  cType = p->cellType;
  removeCell(x, y, &g->table);

  if (cType != CELL_DEAD)
    g->population--;
}
