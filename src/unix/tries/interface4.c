#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h)
{
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    if (0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN, window, renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
}

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer)
{
    SDL_Surface *tmp = NULL;
    SDL_Texture *texture = NULL;
    tmp = SDL_LoadBMP(path);
    if (NULL == tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if (NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

int setWindowColor(SDL_Renderer *renderer, SDL_Color color)
{
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if (SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;
}

Uint32 couleur(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return r << 24 | g << 16 | b << 8 | a;
}

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;

    if (init(&window, &renderer, 640, 480) < 0) {
        goto Quit;
    }
    /* On fait toutes nos initialisations ici */
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 128, 0, 128);
    SDL_RenderFillRect(renderer, NULL);
    SDL_Delay(1000);
    SDL_RenderPresent(renderer);
    loadImage("test.bmp", renderer);
    SDL_Delay(1000);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_Delay(1000);
    statut = EXIT_SUCCESS;

    /* On libère toutes nos ressources ici et on fait notre return*/
Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}
