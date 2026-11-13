#ifndef INPUT_H
#define INPUT_H

//#include <SDL.h>
#include "game.h"

void inputHandleEvent(Game *game, const SDL_Event *event, int *running);

#endif
