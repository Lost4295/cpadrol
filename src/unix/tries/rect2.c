#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define ROUGE "\033[31;01;51m"
#define JAUNE "\033[33;01m"
#define BLANC "\033[37;07m"
#define NOIR "\033[30;01m"
#define VERT "\033[32;01m"
#define BLEU "\033[34;01m"
#define END "\033[00m"
#define ROUGEW "\033[31;42;01;51m"
#define JAUNEW "\033[33;42;01m"
int tableau[6][7];
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

int makeChooseCircle(SDL_Renderer *renderer, int num)
{
    SDL_SetRenderDrawColor(renderer, 83, 28, 125, 14);
    SDL_RenderFillCircle(renderer, num * 50 + 25, 6 * 50 + 25, 5);
    SDL_Delay(50);
    SDL_RenderPresent(renderer);
}
int createTableau(SDL_Renderer *renderer)
{
    SDL_Rect multi_rect1[6];
    SDL_Rect multi_rect2[6];
    SDL_Rect multi_rect3[6];
    SDL_Rect multi_rect4[6];
    SDL_Rect multi_rect5[6];
    SDL_Rect multi_rect6[6];
    SDL_Rect multi_rect7[6];
    for (int i = 0; i < 6; i++)
    {
        multi_rect1[i].w = 50;
        multi_rect1[i].h = 50;
        multi_rect1[i].x = 0;
        multi_rect1[i].y = i * 50;
    }
    for (int i = 0; i < 6; i++)
    {
        multi_rect2[i].w = 50;
        multi_rect2[i].h = 50;
        multi_rect2[i].x = 50;
        multi_rect2[i].y = i * 50;
    }
    for (int i = 0; i < 6; i++)
    {
        multi_rect3[i].w = 50;
        multi_rect3[i].h = 50;
        multi_rect3[i].x = 100;
        multi_rect3[i].y = i * 50;
    }
    for (int i = 0; i < 6; i++)
    {
        multi_rect4[i].w = 50;
        multi_rect4[i].h = 50;
        multi_rect4[i].x = 150;
        multi_rect4[i].y = i * 50;
    }
    for (int i = 0; i < 6; i++)
    {
        multi_rect5[i].w = 50;
        multi_rect5[i].h = 50;
        multi_rect5[i].x = 200;
        multi_rect5[i].y = i * 50;
    }
    for (int i = 0; i < 6; i++)
    {
        multi_rect6[i].w = 50;
        multi_rect6[i].h = 50;
        multi_rect6[i].x = 250;
        multi_rect6[i].y = i * 50;
    }
    for (int i = 0; i < 6; i++)
    {
        multi_rect7[i].w = 50;
        multi_rect7[i].h = 50;
        multi_rect7[i].x = 300;
        multi_rect7[i].y = i * 50;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 140, 255);
    SDL_RenderFillRects(renderer, multi_rect1, 6);
    SDL_RenderFillRects(renderer, multi_rect2, 6);
    SDL_RenderFillRects(renderer, multi_rect3, 6);
    SDL_RenderFillRects(renderer, multi_rect4, 6);
    SDL_RenderFillRects(renderer, multi_rect5, 6);
    SDL_RenderFillRects(renderer, multi_rect6, 6);
    SDL_RenderFillRects(renderer, multi_rect7, 6);
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
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            SDL_RenderFillCircle(renderer, j * 50 + 25, i * 50 + 25, 20);
        }
    }
    SDL_Delay(50);
    SDL_RenderPresent(renderer);
}

int InsertCoin(SDL_Renderer *renderer, int num)
{
    if (*pc == 1)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    else if (*pc == 2)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    }
    int i;
    for (i = 5; i >= 0; i--)
    {
        if (tableau[i][num] == 0)
        {
            tableau[i][num] = *pc;
            SDL_RenderFillCircle(renderer, num * 50 + 25, i * 50 + 25, 20);
            break;
        }
    }
    (*pc == 1) ? (*pc = 2) : (*pc = 1);
}

void printtab()
{
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("%s ===============================%s\n", BLEU, END);
    printf("%s|   1   2   3   4   5   6   7   |%s\n", BLEU, END);
    for (int i = 0; i < 6; i++)
    {
        printf("%s| %s", BLEU, END);
        for (int j = 0; j < 7; j++)
        {
            if (tableau[i][j] == 1)
            {
                printf("| %s%c%s ", ROUGE, 48, END);
            }
            else if (tableau[i][j] == 2)
            {
                printf("| %s%c%s ", JAUNE, 48, END);
            }
            else if (tableau[i][j] == 3)
            {
                printf("| %s%c%s ", ROUGEW, 48, END);
            }
            else if (tableau[i][j] == 4)
            {
                printf("| %s%c%s ", JAUNEW, 48, END);
            }
            else
            {
                printf("| %s%c%s ", NOIR, 32, END);
            }
        }
        printf("\t\t%s| %s", BLEU, END);
        for (int j = 0; j < 7; j++)
        {
            if (tableau[i][j] == 1)
            {
                printf("| %s[%d][%d]%s ", ROUGE, i, j, END);
            }
            else if (tableau[i][j] == 2)
            {
                printf("| %s[%d][%d]%s ", JAUNE, i, j, END);
            }
            else if (tableau[i][j] == 3)
            {
                printf("| %s[%d][%d]%s ", ROUGEW, i, j, END);
            }
            else if (tableau[i][j] == 4)
            {
                printf("| %s[%d][%d]%s ", JAUNEW, i, j, END);
            }
            else
            {
                printf("| %s[%d][%d]%s ", NOIR, i, j, END);
            }
        }
        printf("|\t%s|%s\n", BLEU, END);
    }
    printf("%s ===============================%s\n", BLEU, END);
}

int loadTableau(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 147, 172, 234, 145);
    SDL_RenderFillRect(renderer, NULL);
    createTableau(renderer);
    SDL_Delay(50);

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
            SDL_RenderFillCircle(renderer, j * 50 + 25, i * 50 + 25, 20);
        }
    }
    SDL_Delay(50);
    SDL_RenderPresent(renderer);
}

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
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            tableau[i][j] = 0;
        }
    }
    SDL_SetRenderDrawColor(renderer, 147, 172, 234, 145);
    SDL_RenderFillRect(renderer, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(500);

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
                SDL_Delay(50);
                makeChooseCircle(renderer, num);
                break;
            case SDL_SCANCODE_RIGHT:
                (num == 6) ? num = 0 : num++;
                printf("scancode right : num = %d\n", num);
                loadTableau(renderer);
                SDL_Delay(50);
                makeChooseCircle(renderer, num);
                break;
            case SDL_SCANCODE_ESCAPE:
                quit = SDL_TRUE;
                break;
            case SDL_SCANCODE_SPACE:
                SDL_RenderClear(renderer);
                loadTableau(renderer);
                SDL_Delay(50);
                InsertCoin(renderer, num);
                SDL_RenderPresent(renderer);
                break;
            case SDL_SCANCODE_J:
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                break;
            case SDL_SCANCODE_R:
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                break;
            case SDL_SCANCODE_P:
                printtab();
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
