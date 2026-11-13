/*
 * PIPE-ADDICT - Projet C / SDL2
 *
 * Aide IA : ChatGPT (OpenAI).
 * Utilisation : assistance pédagogique pour la structuration du projet,
 * les explications et des propositions de code à adapter et vérifier.
 * Pourcentage d'utilisation : à estimer par l'équipe selon le code réellement
 * conservé, modifié et compris. Ne pas déclarer un pourcentage fictif.
 *
 * Exemples de prompts :
 * - "Propose une architecture modulaire en C/SDL2 pour un puzzle de tuyaux."
 * - "Explique comment déplacer un fluide dans une grille en vérifiant
 *   les connexions entre les tuyaux."
 *
 * Les membres de l'équipe doivent tester, comprendre et personnaliser
 * l'ensemble du code avant la remise.
 */

#include <stdio.h>
#include <SDL.h>
#include "game.h"
#include "input.h"
#include "render.h"

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    Game game;
    int running = 1;

    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Pipe-Addict",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        printf("Erreur creation fenetre : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        /* Repli utile sur certains ordinateurs/VM. */
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    }

    if (renderer == NULL) {
        printf("Erreur creation renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    gameInit(&game);

    while (running) {
        while (SDL_PollEvent(&event)) {
            inputHandleEvent(&game, &event, &running);
        }

        gameUpdate(&game, SDL_GetTicks());
        renderGame(renderer, &game);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
