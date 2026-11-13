#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#define ROWS 8
#define COLS 8
#define CELL_SIZE 58
#define GRID_X 210
#define GRID_Y 90
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

typedef enum {
    PIPE_HORIZONTAL = 1,
    PIPE_VERTICAL,
    PIPE_CORNER_NE,
    PIPE_CORNER_SE,
    PIPE_CORNER_SW,
    PIPE_CORNER_NW,
    ELECTRIC,
    BOMB
} TileType;

typedef struct {
    TileType type;
    int revealed;
    int visited;
} Tile;

typedef enum {
    GAME_READY,
    GAME_RUNNING,
    GAME_WON,
    GAME_LOST,
    GAME_QUIT
} GameStatus;

typedef struct {
    Tile board[ROWS][COLS];
    Tile initialBoard[ROWS][COLS];
    TileType swapType;
    TileType initialSwapType;

    int entryRow;
    int exitRow;
    int initialEntryRow;
    int initialExitRow;

    int fluidRow;
    int fluidCol;
    int fluidDir;       /* 0=N, 1=E, 2=S, 3=W */
    int fluidActive;
    int bombPaused;
    int bombJustResumed;
    int flowStarted;

    Uint32 gameStartTick;
    Uint32 nextMoveTick;
    Uint32 bombResumeTick;
    Uint32 moveDelay;

    GameStatus status;
    char message[128];
} Game;

void gameInit(Game *game);
void gameReset(Game *game);
void gameNew(Game *game);
void gameRevealOrSwap(Game *game, int row, int col);
void gameUpdate(Game *game, Uint32 now);
void gameStartFlow(Game *game, Uint32 now);
const char *gameStatusText(const Game *game);

#endif
