#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *args[])
{
    SDL_Window *win;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    win = SDL_CreateWindow("Hello World!", 100, 100, 400, 400, SDL_WINDOW_SHOWN);
    if (win == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_RenderClear(renderer);

    SDL_Rect single_rect;
    SDL_Rect multi_rect[5];
    for (int i = 0; i < 5; i++)
    {
        single_rect.w = 50;
        single_rect.h = 50;
        single_rect.x = i * 50;
        single_rect.y = i * 50;
        SDL_SetRenderDrawColor(renderer, 255, (Uint8)(i * 50), 0, 255);
        SDL_RenderFillRect(renderer, &single_rect);

        multi_rect[i].w = 50;
        multi_rect[i].h = 50;
        multi_rect[i].x = i * 50 + 50;
        multi_rect[i].y = i * 50;
    }
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRects(renderer, multi_rect, 5);

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}