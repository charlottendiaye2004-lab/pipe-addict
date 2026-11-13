/*
 * PIPE-ADDICT - Projet C / SDL2
 * Aide IA : ChatGPT (OpenAI), utilise comme assistance pédagogique.
 * Contribution exacte et pourcentage à estimer honnêtement par l'équipe
 * après vérification, adaptation et compréhension du code.
 * Exemples de prompts utilisés :
 * - "Explique comment organiser un jeu de puzzle en modules C/SDL2."
 * - "Aide-moi à gérer les connexions et le déplacement d'un fluide dans une grille."
 * Les membres doivent vérifier, tester et être capables d'expliquer ce fichier.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "game.h"
#include "grid.h"
#include "fluid.h"

void gameInit(Game *game)
{
    memset(game, 0, sizeof(*game));
    srand((unsigned int)SDL_GetTicks());
    gridGenerate(game);

    memcpy(game->initialBoard, game->board, sizeof(game->board));
    game->initialSwapType = game->swapType;
    game->initialEntryRow = game->entryRow;
    game->initialExitRow = game->exitRow;

    game->status = GAME_RUNNING;
    game->gameStartTick = SDL_GetTicks();
    game->moveDelay = 850;
    snprintf(game->message, sizeof(game->message),
             "Cliquez sur une case pour la reveler. Depart dans 5 secondes.");
}

void gameReset(Game *game)
{
    memcpy(game->board, game->initialBoard, sizeof(game->board));
    game->swapType = game->initialSwapType;
    game->entryRow = game->initialEntryRow;
    game->exitRow = game->initialExitRow;

    game->fluidRow = 0;
    game->fluidCol = 0;
    game->fluidDir = 1;
    game->fluidActive = 0;
    game->bombPaused = 0;
    game->bombJustResumed = 0;
    game->flowStarted = 0;
    game->gameStartTick = SDL_GetTicks();
    game->nextMoveTick = 0;
    game->bombResumeTick = 0;
    game->moveDelay = 850;
    game->status = GAME_RUNNING;
    snprintf(game->message, sizeof(game->message),
             "Plateau restaure. Le fluide part dans 5 secondes.");
}

void gameNew(Game *game)
{
    gridGenerate(game);
    memcpy(game->initialBoard, game->board, sizeof(game->board));
    game->initialSwapType = game->swapType;
    game->initialEntryRow = game->entryRow;
    game->initialExitRow = game->exitRow;

    game->fluidRow = 0;
    game->fluidCol = 0;
    game->fluidDir = 1;
    game->fluidActive = 0;
    game->bombPaused = 0;
    game->bombJustResumed = 0;
    game->flowStarted = 0;
    game->gameStartTick = SDL_GetTicks();
    game->nextMoveTick = 0;
    game->bombResumeTick = 0;
    game->moveDelay = 850;
    game->status = GAME_RUNNING;
    snprintf(game->message, sizeof(game->message),
             "Nouvelle grille generee. Depart dans 5 secondes.");
}

void gameRevealOrSwap(Game *game, int row, int col)
{
    Tile *tile;
    TileType temp;

    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return;
    if (game->status != GAME_RUNNING || game->flowStarted) return;

    tile = &game->board[row][col];

    if (!tile->revealed) {
        tile->revealed = 1;
        return;
    }

    /* Les malus et les cases deja traversees ne sont pas deplacables. */
    if (gridIsMalus(tile->type) || tile->visited) return;

    temp = tile->type;
    tile->type = game->swapType;
    game->swapType = temp;
}

void gameStartFlow(Game *game, Uint32 now)
{
    fluidBegin(game, now);
}

void gameUpdate(Game *game, Uint32 now)
{
    if (game->status != GAME_RUNNING) return;

    if (!game->flowStarted && now - game->gameStartTick >= 5000) {
        gameStartFlow(game, now);
    }
    if (game->flowStarted) fluidUpdate(game, now);
}

const char *gameStatusText(const Game *game)
{
    return game->message;
}
