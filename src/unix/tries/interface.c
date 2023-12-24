#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int setWindowColor(SDL_Renderer *renderer, SDL_Color color)
{
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if (SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;
}

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;
    SDL_Color orange = {255, 127, 40, 255};

    /* Initialisation, création de la fenêtre et du renderer. */
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              640, 480, SDL_WINDOW_SHOWN);
    if (NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }

    setWindowColor(renderer, orange); /* La réussite ou non de la fonction ne nous intéresse pas */

    SDL_Delay(500);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
    SDL_Rect rect[50];
    SDL_Color black = {0, 0, 0, 255};
    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    size_t i = 0;
    for (i = 0; i < 50; i++)
    {
        rect[i].w = 50;
        rect[i].h = 50;
        rect[i].x = 100 * (i % 5) + 50 * ((i / 5) % 2);
        rect[i].y = 50 * (i / 5);
    }
    SDL_RenderFillRects(renderer, rect, 50);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);

    statut = EXIT_SUCCESS;

Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}