#include "configurations.h"
#include "../game/game.h"
#include "../cell/cell.h"
#include <stdlib.h>
#include <stdio.h>

// Loads configuration from a file into the game
void loadConfiguration(game **g, char *path) {
  FILE *f = fopen(path, "r");

  if (f == NULL) {
    printf("Error when opening requested file!\n");
    return;
  }

  game *temp;

  // File contains a header that describes the game mode, and after that
  // all cells with their types 
  int gameMode;
  fscanf(f, "%d", &gameMode);

  initGame(&temp, (GAME_MODE) gameMode);

  // Read the file, and load all cells into the game
  while (f != NULL) {
    int x, y, type, format;
    format = fscanf(f, "%d%d%d", &x, &y, &type);

    if (format == EOF) {
      fclose(f);
      destroyGame(g);
      *g = temp;
      return;
    }

    if (format < 3) {
      printf("Bad format of the game!\n");
      fclose(f);
      return;
    }

    addCellToGame(x, y, (CELL_TYPE) type, temp);
  }

}

// Loads configuration giving the user option to choose current configurations
void loadConfigurationWithList(game **g) {
  printf("1. Glider\n");
  printf("2. Glider gun\n");
  printf("3. Virus configuration\n");
  printf("0. Custom\n");
  int k;
  scanf("%d", &k);
  if (k == 1)
    loadConfiguration(g, "configurations/glider.txt");
  else if (k == 2)
    loadConfiguration(g, "configurations/gg_1003.txt");
  else if (k == 3)
    loadConfiguration(g, "configurations/randomvirus_3k.txt");
  else {
    char path[30];
    printf("Insert path: ");
    scanf("%s", path);
    loadConfiguration(g, path);
  }
}

// Saves the current configuration in a file
void saveConfiguration(game **g) {
  char path[30];
  printf("Insert path to configuration file:\n");
  scanf("%s", path);

  FILE *f = fopen(path, "w");

  if (f == NULL) {
    printf("Cannot write to file %s\n", path);
    return;
  }

  fprintf(f, "%d", (int) (*g)->gameMode);

  cell *p, *tmp;
  HASH_ITER(hh, (*g)->table, p, tmp) {
    if (p->cellType != CELL_DEAD) {
      fprintf(f, " %d %d %d", p->location.x, p->location.y, (int) p->cellType);
    }
  }

  fclose(f);
}
