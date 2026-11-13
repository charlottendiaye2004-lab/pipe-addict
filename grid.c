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
#include "grid.h"

int gridIsPipe(TileType type)
{
    return type >= PIPE_HORIZONTAL && type <= PIPE_CORNER_NW;
}

int gridIsMalus(TileType type)
{
    return type == ELECTRIC || type == BOMB;
}

void gridReveal(Tile *tile)
{
    if (tile != NULL) tile->revealed = 1;
}

void gridHideUnvisited(Game *game)
{
    int r, c;
    for (r = 0; r < ROWS; ++r) {
        for (c = 0; c < COLS; ++c) {
            if (!game->board[r][c].visited)
                game->board[r][c].revealed = 0;
        }
    }
}

void gridGenerate(Game *game)
{
    int r, c, i, j, k;
    TileType bag[ROWS * COLS];
    int index = 0;

    /* Répartition : 4 cases électriques, 3 bombes et 57 tuyaux. */
    for (i = 0; i < 4; ++i) bag[index++] = ELECTRIC;
    for (i = 0; i < 3; ++i) bag[index++] = BOMB;
    for (i = 7; i < ROWS * COLS; ++i)
        bag[index++] = (TileType)(PIPE_HORIZONTAL + rand() % 6);

    /* Mélange de Fisher-Yates. */
    for (i = ROWS * COLS - 1; i > 0; --i) {
        j = rand() % (i + 1);
        {
            TileType tmp = bag[i];
            bag[i] = bag[j];
            bag[j] = tmp;
        }
    }

    k = 0;
    for (r = 0; r < ROWS; ++r) {
        for (c = 0; c < COLS; ++c) {
            game->board[r][c].type = bag[k++];
            game->board[r][c].revealed = 0;
            game->board[r][c].visited = 0;
        }
    }

    game->entryRow = rand() % ROWS;
    game->exitRow = rand() % ROWS;
    game->swapType = (TileType)(PIPE_HORIZONTAL + rand() % 6);
}
