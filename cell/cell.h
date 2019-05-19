/* Header file for cells */
#ifndef CELL_HEADER
#define CELL_HEADER

#include "../constants/constants.h"
#include "../uthash.h"

/*
  Cells that will be hashed are the ones that are alive,
  or have an alive neighbour. Only those cells can change their 
  state in the next generation. 
*/

typedef struct cel {
  coordinates location;
  CELL_TYPE cellType;
  UT_hash_handle hh;
} cell;

// Adds a cell at (x, y) coordinates of specific type (dead or alive)
void addCell(int x, int y, CELL_TYPE type, cell **table);

// Removes a cell from (x, y)
void removeCell(int x, int y, cell **table);

// Gets cell at position (x, y)
CELL_TYPE getCell(int x, int y, cell **table);

// Returns 1 if a cell doesn't have alive neighbours
int deadNeighbours(int x, int y, cell **table);

// Calculates next generation for normal game mode
CELL_TYPE nextGenerationNormal(int x, int y, cell **table);

// Calculates next generation for coex game mode
CELL_TYPE nextGenerationCoex(int x, int y, cell **table);

// Calculates next generation for predator game mode
CELL_TYPE nextGenerationPredator(int x, int y, cell **table);

// Calculates next generation for virus game mode
CELL_TYPE nextGenerationVirus(int x, int y, cell **table);

// Calculates next generation for unknown game mode
CELL_TYPE nextGenerationUnknown(int x, int y, cell **table);

// Combines the above 5 functions into one 
CELL_TYPE nextGeneration(int x, int y, GAME_MODE gameMode, cell **table);

// Prints the table for NORMAL mode given the top-left and bottom-right coordinates
// Used in development
void printTable(int x1, int y1, int x2, int y2, cell **table); 

#endif
