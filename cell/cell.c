#include "../constants/constants.h"
#include "../uthash.h"
#include "cell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Adds a cell at (x, y) coordinates of specific type (dead or alive)
void addCell(int x, int y, CELL_TYPE type, cell **table) {
  cell *p = NULL;

  coordinates location;
  location.x = x;
  location.y = y;

  // Check if there is already a cell at the given position
  HASH_FIND(hh, *table, &location, sizeof(coordinates), p);

  if (p != NULL) {
    // If user tries to add a dead cell
    if (type == CELL_DEAD)
      return;

    // If user tries to add a living cell in place of a living cell
    if (type != CELL_DEAD && p->cellType != CELL_DEAD)
      return;

    // If user places a living cell on a dead cell, remove the dead cell
    HASH_DEL(*table, p);
  }

  p = (cell *) malloc(sizeof(cell));

  p->location.x = x;
  p->location.y = y;
  p->cellType = type;

  // Adding to the hash table
  HASH_ADD(hh, *table, location, sizeof(coordinates), p);

  // If cell is alive, we have to add dead neighbours around it
  if (type != CELL_DEAD) {
    int xi, yi;
    for (xi = x - 1; xi <= x + 1; xi++) {
      for (yi = y - 1; yi <= y + 1; yi ++) {

        // Skip current cell
        if (xi == x && yi == y) 
          continue;

        // printf("Adding type : %d after (%d, %d, %d)\n",CELL_DEAD, xi, yi, type);

        // Add a dead neighbour (if an alive one doesn't exist)
        addCell(xi, yi, CELL_DEAD, table);

      }
    }
  }
}


// Removes a cell from (x, y)
void removeCell(int x, int y, cell **table) {
  // This function is called when mouse is clicked on a populated spot
  cell *p = NULL;

  coordinates location;
  location.x = x;
  location.y = y;

  // Check if there is a cell on the spot
  HASH_FIND(hh, *table, &location, sizeof(coordinates), p);

  if (p == NULL)
    return;

  // Removing the cell (save CELL_TYPE in case it is alive)
  CELL_TYPE cType = p->cellType;

  HASH_DEL(*table, p);

  // If the cell was alive, remove all dead neighbours that
  // have no living neighbour

  if (cType == CELL_DEAD)
    return;

  int xi, yi;

  
  for (xi = x - 1; xi <= x + 1; xi++) {
    for (yi = y - 1; yi <= y + 1; yi ++) {
   
      // Skip current cell
      if (xi == x && yi == y)
        continue;
      
      location.x = xi;
      location.y = yi;

      HASH_FIND(hh, *table, &location, sizeof(coordinates), p);

      // If there is a cell on the spot and it is dead
      if (p != NULL && p->cellType == CELL_DEAD) {

        // Check if all neighbours are now dead
        if (deadNeighbours(xi, yi, table)) 
          HASH_DEL(*table, p);
      }
    }
  }

}

// Returns 1 if a cell doesn't have alive neighbours
int deadNeighbours(int x, int y, cell **table) {
  int xi, yi;
  cell *p;
  coordinates location;

  for (xi = x - 1; xi <= x + 1; xi++) {
    for (yi = y - 1; yi <= y + 1; yi ++) {
   
      // Skip current cell
      if (xi == x && yi == y)
        continue;

      location.x = xi;
      location.y = yi;

      HASH_FIND(hh, *table, &location, sizeof(coordinates), p);

      if (p != NULL && p->cellType != CELL_DEAD)
        return 0;
    }
  }

  return 1;
}

// Calculates next generation for normal game mode
CELL_TYPE nextGenerationNormal(int x, int y, cell **table) {
  // To be implemented
  return CELL_DEAD;
}

// Calculates next generation for coex game mode
CELL_TYPE nextGenerationCoex(int x, int y, cell **table) {
  // To be implemented
  return CELL_DEAD;
}

// Calculates next generation for predator game mode
CELL_TYPE nextGenerationPredator(int x, int y, cell **table) {
  // To be implemented
  return CELL_DEAD;
}

// Calculates next generation for virus game mode
CELL_TYPE nextGenerationVirus(int x, int y, cell **table) {
  // To be implemented
  return CELL_DEAD;
}

// Calculates next generation for unknown game mode
CELL_TYPE nextGenerationUnknown(int x, int y, cell **table){
  // To be implemented
  return CELL_DEAD;
}

// Calculates the next generation value for a cell
CELL_TYPE nextGeneration(int x, int y, GAME_MODE gameMode, cell **table) {
  switch(gameMode) {
    case NORMAL:
      return nextGenerationNormal(x, y, table);
      break;

    case COEX:
      return nextGenerationCoex(x, y, table);
      break;

    case VIRUS:
      return nextGenerationVirus(x, y, table);
      break;

    case PREDATOR:
      return nextGenerationPredator(x, y, table);
      break;

    case UNKNOWN:
      return nextGenerationUnknown(x, y, table);
      break;
  }
}

//  int main () {
//    cell *table = NULL, *tmp, *p;
//  
//    addCell(1, 1, CELL_NORMAL, &table);
//    addCell(1, 2, CELL_NORMAL, &table);
//    addCell(3, 4, CELL_NORMAL, &table);
//  
//    printf("Before removing: \n");
//    HASH_ITER(hh, table, p, tmp) {
//      char name[20];
//      if (p->cellType == CELL_DEAD)
//        strcpy(name, "CELL_DEAD");
//      else
//        strcpy(name, "CELL_NORMAL");
//  
//      printf("%d %d %s\n", p->location.x, p->location.y, name);
//    }
//  
//    removeCell(1, 2, &table);
//    removeCell(1, 1, &table);
//    removeCell(4, 3, &table);
//  
//    printf("\n\nAfter removing:\n");
//    HASH_ITER(hh, table, p, tmp) {
//      char name[20];
//      if (p->cellType == CELL_DEAD)
//        strcpy(name, "CELL_DEAD");
//      else
//        strcpy(name, "CELL_NORMAL");
//  
//      printf("%d %d %s\n", p->location.x, p->location.y, name);
//    }
//  
//    return 0;
//  }
