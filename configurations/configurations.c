#include "configurations.h"

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

    if (format == 0 || format == EOF) {
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

// Saves the current configuration in a file
void saveConfiguration(game **g, char *path) {
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
