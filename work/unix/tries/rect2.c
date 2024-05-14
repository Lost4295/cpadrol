#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int c = 1;
int *pc = &c;
int SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx)
    {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0)
        {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx)
        {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety))
        {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}
int tableau[6][7];
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
    SDL_SetRenderDrawColor(renderer, 147, 172, 234, 145);
    SDL_RenderFillRect(renderer, NULL);
    SDL_RenderPresent(renderer);
    createTableau(renderer);

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    int num = 0;
    while (!quit)
    {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
            quit = SDL_TRUE;
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_LEFT:
                (num == 0) ? num = 6 : num--;
                printf("scancode left : num = %d\n", num);
                loadTableau(renderer);
                makeChooseCircle(renderer, num);
                break;
            case SDL_SCANCODE_RIGHT:
                (num == 6) ? num = 0 : num++;
                printf("scancode right : num = %d\n",num);
                loadTableau(renderer);
                makeChooseCircle(renderer, num);
                break;
            case SDL_SCANCODE_ESCAPE:
                quit = SDL_TRUE;
                break;
            case SDL_SCANCODE_SPACE:
                SDL_RenderClear(renderer);
                loadTableau(renderer);
                InsertCoin(renderer, num);
                SDL_RenderPresent(renderer);
                break;
            case SDL_SCANCODE_J:
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                break;
            case SDL_SCANCODE_R:
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                break;
            default:
                break;
            }
        }
        SDL_Delay(20);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}

int makeChooseCircle(SDL_Renderer *renderer, int num)
{
    SDL_SetRenderDrawColor(renderer, 83, 28, 125, 14);
    SDL_RenderFillCircle(renderer, num * 50 + 25, 6 * 50 + 25, 5);
    SDL_RenderPresent(renderer);
}
int createTableau(SDL_Renderer *renderer)
{

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            tableau[i][j] = 0;
        }
    }
    SDL_RenderClear(renderer);
    SDL_Rect multi_rect1[7];
    SDL_Rect multi_rect2[7];
    SDL_Rect multi_rect3[7];
    SDL_Rect multi_rect4[7];
    SDL_Rect multi_rect5[7];
    SDL_Rect multi_rect6[7];
    for (int i = 0; i < 7; i++)
    {
        multi_rect1[i].w = 50;
        multi_rect1[i].h = 50;
        multi_rect1[i].x = i * 50;
        multi_rect1[i].y = 0;
    }
    for (int i = 0; i < 7; i++)
    {
        multi_rect2[i].w = 50;
        multi_rect2[i].h = 50;
        multi_rect2[i].x = i * 50;
        multi_rect2[i].y = 50;
    }
    for (int i = 0; i < 7; i++)
    {
        multi_rect3[i].w = 50;
        multi_rect3[i].h = 50;
        multi_rect3[i].x = i * 50;
        multi_rect3[i].y = 100;
    }
    for (int i = 0; i < 7; i++)
    {
        multi_rect4[i].w = 50;
        multi_rect4[i].h = 50;
        multi_rect4[i].x = i * 50;
        multi_rect4[i].y = 150;
    }
    for (int i = 0; i < 7; i++)
    {
        multi_rect5[i].w = 50;
        multi_rect5[i].h = 50;
        multi_rect5[i].x = i * 50;
        multi_rect5[i].y = 200;
    }
    for (int i = 0; i < 7; i++)
    {
        multi_rect6[i].w = 50;
        multi_rect6[i].h = 50;
        multi_rect6[i].x = i * 50;
        multi_rect6[i].y = 250;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 140, 255);
    SDL_RenderFillRects(renderer, multi_rect1, 7);
    SDL_RenderFillRects(renderer, multi_rect2, 7);
    SDL_RenderFillRects(renderer, multi_rect3, 7);
    SDL_RenderFillRects(renderer, multi_rect4, 7);
    SDL_RenderFillRects(renderer, multi_rect5, 7);
    SDL_RenderFillRects(renderer, multi_rect6, 7);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderDrawLine(renderer, 0, 0, 50 * 7, 0);
    SDL_RenderDrawLine(renderer, 0, 50, 50 * 7, 50);
    SDL_RenderDrawLine(renderer, 0, 100, 50 * 7, 100);
    SDL_RenderDrawLine(renderer, 0, 150, 50 * 7, 150);
    SDL_RenderDrawLine(renderer, 0, 200, 50 * 7, 200);
    SDL_RenderDrawLine(renderer, 0, 250, 50 * 7, 250);
    SDL_RenderDrawLine(renderer, 0, 300, 50 * 7, 300);
    SDL_RenderDrawLine(renderer, 0, 0, 0, 50 * 6);
    SDL_RenderDrawLine(renderer, 50, 0, 50, 6 * 50);
    SDL_RenderDrawLine(renderer, 100, 0, 100, 50 * 6);
    SDL_RenderDrawLine(renderer, 150, 0, 150, 50 * 6);
    SDL_RenderDrawLine(renderer, 200, 0, 200, 50 * 6);
    SDL_RenderDrawLine(renderer, 250, 0, 250, 50 * 6);
    SDL_RenderDrawLine(renderer, 300, 0, 300, 50 * 6);
    SDL_RenderDrawLine(renderer, 350, 0, 350, 50 * 6);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            SDL_RenderFillCircle(renderer, i * 50 + 25, j * 50 + 25, 20);
        }
    }
    SDL_RenderPresent(renderer);
}

int InsertCoin(SDL_Renderer *renderer, int num)
{
    int i;
    for (i = 0; i <= 5; i++)
    {
        if (tableau[num][i] == 0)
        {
            tableau[num][i] = *pc;
            break;
        }
    }
    if (*pc == 1)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    else if (*pc == 2)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    }
    SDL_RenderFillCircle(renderer, i * 50 + 25, num * 50 + 25, 20);
    (*pc == 1) ? (*pc = 2) : (*pc = 1);
}

int loadTableau(SDL_Renderer *renderer)
{
    createTableau(renderer);
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (tableau[i][j] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            else if (tableau[i][j] == 2)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            }
            SDL_RenderFillCircle(renderer, i * 50 + 25, j * 50 + 25, 20);
        }
    }
    SDL_RenderPresent(renderer);
}