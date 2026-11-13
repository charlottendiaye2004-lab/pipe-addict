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

#include "input.h"

static int inside(int x, int y, SDL_Rect r)
{
    return x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h;
}

void inputHandleEvent(Game *game, const SDL_Event *event, int *running)
{
    int x, y, row, col;
    SDL_Rect resetButton = {750, 150, 190, 48};
    SDL_Rect newButton   = {750, 215, 190, 48};
    SDL_Rect quitButton  = {750, 280, 190, 48};

    if (event->type == SDL_QUIT) {
        *running = 0;
        return;
    }

    if (event->type != SDL_MOUSEBUTTONDOWN ||
        event->button.button != SDL_BUTTON_LEFT) return;

    x = event->button.x;
    y = event->button.y;

    if (inside(x, y, resetButton)) {
        gameReset(game);
        return;
    }
    if (inside(x, y, newButton)) {
        gameNew(game);
        return;
    }
    if (inside(x, y, quitButton)) {
        *running = 0;
        return;
    }

    if (x >= GRID_X && x < GRID_X + COLS * CELL_SIZE &&
        y >= GRID_Y && y < GRID_Y + ROWS * CELL_SIZE) {
        col = (x - GRID_X) / CELL_SIZE;
        row = (y - GRID_Y) / CELL_SIZE;
        gameRevealOrSwap(game, row, col);
    }
}
