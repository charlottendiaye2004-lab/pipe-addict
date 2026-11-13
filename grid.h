#ifndef GRID_H
#define GRID_H

#include "game.h"

void gridGenerate(Game *game);
int gridIsPipe(TileType type);
int gridIsMalus(TileType type);
void gridReveal(Tile *tile);
void gridHideUnvisited(Game *game);

#endif
