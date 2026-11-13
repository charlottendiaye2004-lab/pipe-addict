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
#include "render.h"
#include "grid.h"

static void color(SDL_Renderer *r, Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_SetRenderDrawColor(r, red, green, blue, 255);
}

static void drawText(SDL_Renderer *r, int x, int y, const char *text, int scale)
{
    /* Petite police bitmap 5x7 pour les lettres utilisées dans l'interface. */
    static const struct { char ch; unsigned char row[7]; } glyphs[] = {
        {'A',{14,17,17,31,17,17,17}}, {'B',{30,17,17,30,17,17,30}},
        {'C',{14,17,16,16,16,17,14}}, {'D',{30,17,17,17,17,17,30}},
        {'E',{31,16,16,30,16,16,31}}, {'F',{31,16,16,30,16,16,16}},
        {'G',{14,17,16,23,17,17,15}}, {'H',{17,17,17,31,17,17,17}},
        {'I',{31,4,4,4,4,4,31}}, {'J',{7,2,2,2,18,18,12}},
        {'K',{17,18,20,24,20,18,17}}, {'L',{16,16,16,16,16,16,31}},
        {'M',{17,27,21,21,17,17,17}}, {'N',{17,25,21,19,17,17,17}},
        {'O',{14,17,17,17,17,17,14}}, {'P',{30,17,17,30,16,16,16}},
        {'Q',{14,17,17,17,21,18,13}}, {'R',{30,17,17,30,20,18,17}},
        {'S',{15,16,16,14,1,1,30}}, {'T',{31,4,4,4,4,4,4}},
        {'U',{17,17,17,17,17,17,14}}, {'V',{17,17,17,17,17,10,4}},
        {'W',{17,17,17,21,21,21,10}}, {'X',{17,17,10,4,10,17,17}},
        {'Y',{17,17,10,4,4,4,4}}, {'Z',{31,1,2,4,8,16,31}},
        {'0',{14,17,19,21,25,17,14}}, {'1',{4,12,4,4,4,4,14}},
        {'2',{14,17,1,2,4,8,31}}, {'3',{30,1,1,14,1,1,30}},
        {'4',{2,6,10,18,31,2,2}}, {'5',{31,16,16,30,1,1,30}},
        {'6',{14,16,16,30,17,17,14}}, {'7',{31,1,2,4,8,8,8}},
        {'8',{14,17,17,14,17,17,14}}, {'9',{14,17,17,15,1,1,14}},
        {'?',{14,17,1,2,4,0,4}}, {'!',{4,4,4,4,4,0,4}},
        {':',{0,4,0,0,4,0,0}}, {'-',{0,0,0,31,0,0,0}}
    };
    int i, row, col, k;
    for (i = 0; text[i] != '\0'; ++i) {
        char ch = text[i];
        if (ch >= 'a' && ch <= 'z') ch = (char)(ch - 'a' + 'A');
        if (ch == ' ') continue;
        for (k = 0; k < (int)(sizeof(glyphs)/sizeof(glyphs[0])); ++k)
            if (glyphs[k].ch == ch) break;
        if (k == (int)(sizeof(glyphs)/sizeof(glyphs[0]))) continue;
        for (row = 0; row < 7; ++row) {
            for (col = 0; col < 5; ++col) {
                if (glyphs[k].row[row] & (1 << (4-col))) {
                    SDL_Rect p = {x + (i * 6 + col) * scale,
                                  y + row * scale, scale, scale};
                    SDL_RenderFillRect(r, &p);
                }
            }
        }
    }
}

static void drawPipe(SDL_Renderer *r, int x, int y, TileType type)
{
    int cx = x + CELL_SIZE / 2;
    int cy = y + CELL_SIZE / 2;
    color(r, 30, 210, 190);
    switch (type) {
        case PIPE_HORIZONTAL:
            SDL_RenderDrawLine(r, x+5, cy, x+CELL_SIZE-5, cy); break;
        case PIPE_VERTICAL:
            SDL_RenderDrawLine(r, cx, y+5, cx, y+CELL_SIZE-5); break;
        case PIPE_CORNER_NE:
            SDL_RenderDrawLine(r, cx, cy, cx, y+5);
            SDL_RenderDrawLine(r, cx, cy, x+CELL_SIZE-5, cy); break;
        case PIPE_CORNER_SE:
            SDL_RenderDrawLine(r, cx, cy, cx, y+CELL_SIZE-5);
            SDL_RenderDrawLine(r, cx, cy, x+CELL_SIZE-5, cy); break;
        case PIPE_CORNER_SW:
            SDL_RenderDrawLine(r, cx, cy, cx, y+CELL_SIZE-5);
            SDL_RenderDrawLine(r, cx, cy, x+5, cy); break;
        case PIPE_CORNER_NW:
            SDL_RenderDrawLine(r, cx, cy, cx, y+5);
            SDL_RenderDrawLine(r, cx, cy, x+5, cy); break;
        default: break;
    }
}

static void drawButton(SDL_Renderer *r, SDL_Rect rect, const char *label)
{
    color(r, 55, 85, 125);
    SDL_RenderFillRect(r, &rect);
    color(r, 220, 230, 240);
    SDL_RenderDrawRect(r, &rect);
    drawText(r, rect.x + 18, rect.y + 17, label, 2);
}

void renderGame(SDL_Renderer *renderer, const Game *game)
{
    int row, col, x, y;
    SDL_Rect cell;
    SDL_Rect swap = {55, 490, 115, 65};
    SDL_Rect resetButton = {750, 150, 190, 48};
    SDL_Rect newButton = {750, 215, 190, 48};
    SDL_Rect quitButton = {750, 280, 190, 48};

    color(renderer, 20, 27, 39);
    SDL_RenderClear(renderer);

    color(renderer, 245, 245, 245);
    drawText(renderer, 300, 25, "PIPE ADDICT", 4);

    for (row = 0; row < ROWS; ++row) {
        for (col = 0; col < COLS; ++col) {
            x = GRID_X + col * CELL_SIZE;
            y = GRID_Y + row * CELL_SIZE;
            cell.x = x; cell.y = y; cell.w = CELL_SIZE; cell.h = CELL_SIZE;

            if (game->board[row][col].visited)
                color(renderer, 45, 85, 95);
            else
                color(renderer, 52, 62, 78);
            SDL_RenderFillRect(renderer, &cell);

            color(renderer, 170, 185, 200);
            SDL_RenderDrawRect(renderer, &cell);

            if (game->board[row][col].revealed) {
                TileType t = game->board[row][col].type;
                if (gridIsPipe(t)) {
                    drawPipe(renderer, x, y, t);
                } else if (t == ELECTRIC) {
                    color(renderer, 255, 220, 30);
                    drawText(renderer, x+17, y+19, "E", 3);
                } else if (t == BOMB) {
                    color(renderer, 255, 90, 65);
                    drawText(renderer, x+17, y+19, "B", 3);
                }
            } else {
                color(renderer, 235, 235, 245);
                drawText(renderer, x+20, y+20, "?", 2);
            }
        }
    }

    /* Entrée verte et sortie rouge. */
    color(renderer, 40, 230, 90);
    {
        SDL_Rect entry = {GRID_X-13, GRID_Y + game->entryRow*CELL_SIZE + 15, 13, 28};
        SDL_RenderFillRect(renderer, &entry);
    }
    color(renderer, 245, 65, 65);
    {
        SDL_Rect exit = {GRID_X+COLS*CELL_SIZE, GRID_Y + game->exitRow*CELL_SIZE + 15, 13, 28};
        SDL_RenderFillRect(renderer, &exit);
    }

    /* Case Swap. */
    color(renderer, 75, 88, 110);
    SDL_RenderFillRect(renderer, &swap);
    color(renderer, 240, 240, 240);
    SDL_RenderDrawRect(renderer, &swap);
    drawText(renderer, 82, 497, "SWAP", 2);
    drawPipe(renderer, 83, 515, game->swapType);

    drawButton(renderer, resetButton, "REFAIRE");
    drawButton(renderer, newButton, "NOUVEAU");
    drawButton(renderer, quitButton, "TERMINER");

    color(renderer, 245, 245, 245);
    drawText(renderer, 735, 370, "ETAT DU JEU", 2);
    drawText(renderer, 735, 395, gameStatusText(game), 1);

    if (game->fluidActive || game->status == GAME_WON || game->status == GAME_LOST) {
        int fx = GRID_X + game->fluidCol*CELL_SIZE + CELL_SIZE/2;
        int fy = GRID_Y + game->fluidRow*CELL_SIZE + CELL_SIZE/2;
        color(renderer, 60, 150, 255);
        {
            int a, b;
            for (a = -6; a <= 6; ++a)
                for (b = -6; b <= 6; ++b)
                    if (a*a + b*b <= 36)
                        SDL_RenderDrawPoint(renderer, fx+a, fy+b);
        }
    }

    SDL_RenderPresent(renderer);
}
