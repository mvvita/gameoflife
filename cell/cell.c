#include "../constants/constants.h"
#include "../cell/cell.h"
#include "../uthash.h"
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
  free(p);

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

// Gets cell at position (x, y)
CELL_TYPE getCell(int x, int y, cell **table) {
  coordinates location;
  location.x = x;
  location.y = y;

  cell *p;

  HASH_FIND(hh, *table, &location, sizeof(coordinates), p);

  if (p == NULL)
    return CELL_DEAD;

  return p->cellType;
}

// Calculates next generation for normal game mode
CELL_TYPE nextGenerationNormal(int x, int y, cell **table) {
  // If the current cell is alive, it lives if it has 2 or 3 alive neighbours
  // If it is dead, it becomes alive if it has exactly 3 neighbours
  
  int xi, yi, neighbours = 0;
  cell *p, *temp;
  coordinates location;
  
  location.x = x;
  location.y = y;

  // Get the current cell
  HASH_FIND(hh, *table, &location, sizeof(coordinates), p);


  // If a cell is not in hash table, it can't become alive in next iteration
  if (p == NULL)
    return CELL_DEAD;

  // printf("Current cell is (%d %d %d)\n", p->location.x, p->location.y, p->cellType);

  // Iterate through all neighbours and count alive ones
  for (xi = x - 1; xi <= x + 1; xi++) 
    for (yi = y - 1; yi <= y + 1; yi++) {
      if (xi == x && yi == y)
        continue;

      location.x = xi;
      location.y = yi;

      HASH_FIND(hh, *table, &location, sizeof(coordinates), temp);

      if (temp != NULL && temp->cellType == CELL_NORMAL)
        neighbours++;
    }

  if (neighbours == 3 || (p->cellType == CELL_NORMAL && neighbours == 2))
    return CELL_NORMAL;

  return CELL_DEAD;
}

// Calculates next generation for coex game mode
CELL_TYPE nextGenerationCoex(int x, int y, cell **table) {
  // If the current cell is alive, it lives if it has 2 or 3 alive neighbours
  // If it is dead, it becomes alive if it has exactly 3 neighbours
  // Cells of different species are also counted as alive
  // Dead cell becomes the same type as majority of neighbours
  
  int xi, yi, neighboursT1 = 0, neighboursT2 = 0;
  cell *p, *temp;
  coordinates location;
  
  location.x = x;
  location.y = y;

  // Get the current cell
  HASH_FIND(hh, *table, &location, sizeof(coordinates), p);

  // If a cell is not in hash table, it can't become alive in next iteration
  if (p == NULL)
    return CELL_DEAD;

  // Iterate through all neighbours and count alive ones
  for (xi = x - 1; xi <= x + 1; xi++) 
    for (yi = y - 1; yi <= y + 1; yi++) {
      if (xi == x && yi == y)
        continue;

      location.x = xi;
      location.y = yi;

      HASH_FIND(hh, *table, &location, sizeof(coordinates), temp);

      if (temp != NULL && temp->cellType != CELL_DEAD) {
        if (temp->cellType == CELL_COEX_1)
          neighboursT1++;
        else
          neighboursT2++;
      }
    }


  if ((neighboursT1 + neighboursT2) == 3 || (p->cellType != CELL_DEAD && (neighboursT1 + neighboursT2) == 2)) {
    if (p->cellType != CELL_DEAD)
      return p->cellType;

    if (neighboursT2 > neighboursT1)
      return CELL_COEX_2;
    else
      return CELL_COEX_1;
  }

  return CELL_DEAD;
}

// Calculates next generation for predator game mode
CELL_TYPE nextGenerationPredator(int x, int y, cell **table) {
  // Standard rules apply for both species (2 + living or 3 to be alive)
  // But if the cell is of type CELL_NORMAL and has a CELL_PREDATOR neighbour,
  // it dies
  
  int xi, yi, neighboursNormal = 0, neighboursPredator = 0;
  cell *p, *temp;
  coordinates location;
  
  location.x = x;
  location.y = y;

  // Get the current cell
  HASH_FIND(hh, *table, &location, sizeof(coordinates), p);

  // If a cell is not in hash table, it can't become alive in next iteration
  if (p == NULL)
    return CELL_DEAD;

  // Iterate through all neighbours and count alive ones
  for (xi = x - 1; xi <= x + 1; xi++) 
    for (yi = y - 1; yi <= y + 1; yi++) {
      if (xi == x && yi == y)
        continue;

      location.x = xi;
      location.y = yi;

      HASH_FIND(hh, *table, &location, sizeof(coordinates), temp);

      if (temp != NULL && temp->cellType != CELL_DEAD) {
        if (temp->cellType == CELL_NORMAL)
          neighboursNormal++;
        else
          neighboursPredator++;
      }
    }

  // Current cell can either be CELL_NORMAL, CELL_PREDATOR or CELL_DEAD
  if (p->cellType == CELL_NORMAL) {
    if (neighboursPredator > 0)
      return CELL_DEAD;
    else if (neighboursNormal == 2 || neighboursNormal == 3)
      return CELL_NORMAL;

  } else if (p->cellType == CELL_PREDATOR){
    if (neighboursPredator == 2 || neighboursPredator == 3)
      return CELL_PREDATOR;

  } else if (neighboursPredator == 0 && neighboursNormal == 3) {
      return CELL_NORMAL;
  } else if (neighboursPredator == 3)
      return CELL_PREDATOR;

  return CELL_DEAD;
}

// Calculates next generation for virus game mode
CELL_TYPE nextGenerationVirus(int x, int y, cell **table) {
  // Standard rules apply for both species (2 + living or 3 to be alive)
  // But if the cell is of type CELL_NORMAL and has a CELL_VIRUS neighbour,
  // it becomes CELL_VIRUS in next iteration (if it can live) 
  
  int xi, yi, neighboursNormal = 0, neighboursVirus = 0;
  cell *p, *temp;
  coordinates location;
  
  location.x = x;
  location.y = y;

  // Get the current cell
  HASH_FIND(hh, *table, &location, sizeof(coordinates), p);

  // If a cell is not in hash table, it can't become alive in next iteration
  if (p == NULL)
    return CELL_DEAD;

  // Iterate through all neighbours and count alive ones
  for (xi = x - 1; xi <= x + 1; xi++) 
    for (yi = y - 1; yi <= y + 1; yi++) {
      if (xi == x && yi == y)
        continue;

      location.x = xi;
      location.y = yi;

      HASH_FIND(hh, *table, &location, sizeof(coordinates), temp);

      if (temp != NULL && temp->cellType != CELL_DEAD) {
        if (temp->cellType == CELL_NORMAL)
          neighboursNormal++;
        else
          neighboursVirus++;
      }
    }

  // Current cell can either be CELL_NORMAL, CELL_VIRUS or CELL_DEAD
  // Unlike with predator where we didn't count CELL_PREDATOR as living
  // neighbours (they are independent species, aside from one killing the
  // other), here CELL_VIRUS are just infected CELL_NORMAL

  if (p->cellType == CELL_VIRUS) {
    if ((neighboursVirus + neighboursNormal) == 2 || (neighboursVirus + neighboursNormal) == 3) 
      return CELL_VIRUS;

  } else if (p->cellType == CELL_NORMAL) {
    if ((neighboursVirus + neighboursNormal) == 2 || (neighboursVirus + neighboursNormal) == 3) 
      if (neighboursVirus == 0)
        return CELL_NORMAL;
      else
        return CELL_VIRUS;

  } else if ((neighboursVirus + neighboursNormal) == 3) 
      if (neighboursVirus == 0)
        return CELL_NORMAL;
      else
        return CELL_VIRUS;

  return CELL_DEAD;
}

// Calculates next generation for unknown game mode
CELL_TYPE nextGenerationUnknown(int x, int y, cell **table){
  // If the current cell is alive, it lives if it has 2 or 3 alive neighbours
  // If it is dead, it becomes alive if it has exactly 3 neighbours
  // Cells of different species are not counted as alive
  // Dead cell becomes the same type as majority of neighbours
  
  int xi, yi, neighboursT1 = 0, neighboursT2 = 0;
  cell *p, *temp;
  coordinates location;
  
  location.x = x;
  location.y = y;

  // Get the current cell
  HASH_FIND(hh, *table, &location, sizeof(coordinates), p);

  // If a cell is not in hash table, it can't become alive in next iteration
  if (p == NULL)
    return CELL_DEAD;

  // Iterate through all neighbours and count alive ones
  for (xi = x - 1; xi <= x + 1; xi++) 
    for (yi = y - 1; yi <= y + 1; yi++) {
      if (xi == x && yi == y)
        continue;

      location.x = xi;
      location.y = yi;

      HASH_FIND(hh, *table, &location, sizeof(coordinates), temp);

      if (temp != NULL && temp->cellType != CELL_DEAD) {
        if (temp->cellType == CELL_COEX_1)
          neighboursT1++;
        else
          neighboursT2++;
      }
    }

  // Calculate indepentently in case of alive cell
  // In case of a dead cell, see if it has enough neighbours of either species
  if (p->cellType == CELL_COEX_1) {
    if (neighboursT1 == 2 || neighboursT1 == 3)
      return CELL_COEX_1;
  } else if (p->cellType == CELL_COEX_2) {
    if (neighboursT2 == 2 || neighboursT2 == 3)
      return CELL_COEX_2;
  } else {
    if (neighboursT1 == 3)
      return CELL_COEX_1;
    
    if (neighboursT2 == 3)
      return CELL_COEX_2;
  }

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

    default:
      return CELL_DEAD;
      break;
  }
}

// Prints the table for NORMAL mode given the top-left and bottom-right coordinates
void printTable(int x1, int y1, int x2, int y2, cell **table) {
  int x, y;
  CELL_TYPE temp;
  for (x = x1; x <= x2; x++, printf("\n"))
    for (y = y1; y <= y2; y++) {
      temp = getCell(x, y, table); 

      if (temp == CELL_NORMAL)
        printf("O");
      else if (temp == CELL_PREDATOR)
        printf("P");
      else if (temp == CELL_VIRUS)
        printf("V");
      else if (temp == CELL_COEX_1)
        printf("C");
      else if (temp == CELL_COEX_2)
        printf("X");
      else
        printf(" ");
    }
}
