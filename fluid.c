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

#include <stdio.h>
#include <string.h>
#include "fluid.h"
#include "grid.h"

/* Directions : nord=0, est=1, sud=2, ouest=3. */
static const int dr[4] = {-1, 0, 1, 0};
static const int dc[4] = {0, 1, 0, -1};

static int hasConnection(TileType type, int direction)
{
    switch (type) {
        case PIPE_HORIZONTAL: return direction == 1 || direction == 3;
        case PIPE_VERTICAL:   return direction == 0 || direction == 2;
        case PIPE_CORNER_NE:  return direction == 0 || direction == 1;
        case PIPE_CORNER_SE:  return direction == 1 || direction == 2;
        case PIPE_CORNER_SW:  return direction == 2 || direction == 3;
        case PIPE_CORNER_NW:  return direction == 3 || direction == 0;
        default: return 0;
    }
}

/* Renvoie l'autre connexion du tuyau. -1 signifie mauvaise connexion. */
static int pipeExitDirection(TileType type, int travelDirection)
{
    int entrySide = (travelDirection + 2) % 4;
    int d;
    if (!hasConnection(type, entrySide)) return -1;
    for (d = 0; d < 4; ++d) {
        if (d != entrySide && hasConnection(type, d)) return d;
    }
    return -1;
}

static void lose(Game *game, const char *reason)
{
    game->status = GAME_LOST;
    game->fluidActive = 0;
    snprintf(game->message, sizeof(game->message), "DEFAITE : %s", reason);
}

static void win(Game *game)
{
    game->status = GAME_WON;
    game->fluidActive = 0;
    snprintf(game->message, sizeof(game->message), "VICTOIRE ! Le fluide a atteint la sortie.");
}

void fluidBegin(Game *game, Uint32 now)
{
    game->flowStarted = 1;
    game->fluidActive = 1;
    game->bombPaused = 0;
    game->fluidRow = game->entryRow;
    game->fluidCol = 0;
    game->fluidDir = 1; /* Le fluide arrive de la gauche et va vers l'est. */
    game->moveDelay = 850;
    game->nextMoveTick = now;
    game->board[game->fluidRow][game->fluidCol].visited = 1;
    game->board[game->fluidRow][game->fluidCol].revealed = 1;
    snprintf(game->message, sizeof(game->message), "Le fluide commence a circuler !");
}

void fluidUpdate(Game *game, Uint32 now)
{
    Tile *current;
    int nextDir, nr, nc;

    if (!game->fluidActive || game->status != GAME_RUNNING) return;

    if (game->bombPaused) {
        if (now < game->bombResumeTick) return;
        game->bombPaused = 0;
        game->bombJustResumed = 1;
        game->nextMoveTick = now;
        snprintf(game->message, sizeof(game->message), "La bombe est passee, le fluide reprend !");
    }

    if (now < game->nextMoveTick) return;

    current = &game->board[game->fluidRow][game->fluidCol];
    current->visited = 1;
    current->revealed = 1;

    if (current->type == ELECTRIC) {
        /* Le malus électrique laisse passer le fluide en ligne droite et accélère. */
        nextDir = game->fluidDir;
        game->moveDelay = 350;
    } else if (current->type == BOMB && !game->bombJustResumed) {
        /* La bombe bloque 5 secondes puis le fluide ressort dans la même direction. */
        game->bombPaused = 1;
        game->bombResumeTick = now + 5000;
        game->moveDelay = 850;
        gridHideUnvisited(game);
        snprintf(game->message, sizeof(game->message), "BOMBE : pause de 5 secondes !");
        return;
    } else if (current->type == BOMB && game->bombJustResumed) {
        /* Ne pas redéclencher la même bombe après la pause. */
        game->bombJustResumed = 0;
        nextDir = game->fluidDir;
        game->moveDelay = 850;
    } else {
        nextDir = pipeExitDirection(current->type, game->fluidDir);
        game->moveDelay = 850;
        if (nextDir < 0) {
            lose(game, "tuyau mal oriente ou impasse");
            return;
        }
    }

    nr = game->fluidRow + dr[nextDir];
    nc = game->fluidCol + dc[nextDir];

    /* Une sortie n'est valide que par le bord droit, sur la ligne prévue. */
    if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS) {
        if (nc == COLS && nr == game->exitRow && nextDir == 1) {
            win(game);
        } else {
            lose(game, "le fluide sort par un mauvais bord");
        }
        return;
    }

    game->fluidRow = nr;
    game->fluidCol = nc;
    game->fluidDir = nextDir;
    game->board[nr][nc].visited = 1;
    game->board[nr][nc].revealed = 1;
    game->nextMoveTick = now + game->moveDelay;
}
