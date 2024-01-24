#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int width = 1280, height = 720;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event e;

SDL_Vertex triangleVertex[3] =
    {
        {{400, 400},        /* first point location */
         {255, 255, 255, 0xFF}, /* first color */
         {0.f, 0.f}},
        {{600, 400},        /* second point location */
         {255, 255, 255, 0xFF}, /* second color */
         {0.f, 0.f}},
        {{500, 200},        /* third point location */
         {255, 255, 255, 0xFF}, /* third color */
         {0.f, 0.f}}};

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("SDL Chaste Triangle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (SDL_RenderGeometry(renderer, NULL, triangleVertex, 3, NULL, 0) < 0)
    {
        SDL_Log("%s\n", SDL_GetError());
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect;
    rect.x = 480;
    rect.y = 400;
    rect.w = 40;
    rect.h = 70;
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    while (e.type != SDL_KEYUP && e.type != SDL_QUIT) /*wait until any key is pressed and then released*/
    {
        SDL_PollEvent(&e);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
