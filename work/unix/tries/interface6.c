#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    if (TTF_Init() < 0)
    {
        fprintf(stderr, "Erreur TTF_Init : %s", TTF_GetError());
        return -1;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_PNG | IMG_INIT_WEBP) < 0)
    {
        fprintf(stderr, "Erreur IMG_Init : %s", IMG_GetError());
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
    SDL_RenderFillRect(renderer, NULL);
    SDL_RenderPresent(renderer);
    return 0;
}
void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);
    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);
    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);
        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }
        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

int SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx)
    {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

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

void printText(TTF_Font *font, SDL_Renderer *renderer, SDL_Color color, char *texte, SDL_Rect *dest)
{
    SDL_Surface *text;

    text = TTF_RenderText_Solid(font, texte, color);
    if (!text)
    {
        printf("Failed to render text: %s\n", TTF_GetError());
    }
    SDL_Texture *text_texture;
    SDL_Rect destn = {0, 0, text->w, text->h};
    destn.x = dest->x;
    destn.y = dest->y;
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_RenderCopy(renderer, text_texture, NULL, &destn);
}
int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Texture *text;
    SDL_Surface *recte;
    TTF_Font *font, *tfont;

    SDL_Rect rect = {100, 100, 100, 100}, dst = {0, 0, 0, 0};
    int statut = EXIT_FAILURE;
    if (init(&window, &renderer, 640, 480) < 0)
    {
        goto Quit;
    }
    font = TTF_OpenFont("Roboto.ttf", 25);
    tfont = TTF_OpenFont("Roboto.ttf", 32);
    if (!font)
    {
        printf("Error loading font: %s\n", TTF_GetError());
        return -1;
    }
    /* On fait toutes nos initialisations ici */
    SDL_RenderClear(renderer);
    SDL_Color white_color = {255, 255, 255, 255};
    SDL_Color black_color = {0, 0, 0, 255};
    SDL_Color yellow_color = {255, 255, 0, 255};
    SDL_Color blue_color = {0, 0, 150, 255};
    SDL_Color red_color = {255, 0, 0, 255};
    setWindowColor(renderer, blue_color);

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while (!quit)
    {
        SDL_bool renderText = SDL_FALSE;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.key.keysym.scancode != SDL_GetScancodeFromKey(event.key.keysym.sym))
            {
                SDL_Log("Physical %s key acting as %s key",
                        SDL_GetScancodeName(event.key.keysym.scancode),
                        SDL_GetKeyName(event.key.keysym.sym));
            }
        }
    }
    statut = EXIT_SUCCESS;

    /* On libère toutes nos ressources ici et on fait notre return*/
Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != texture)
        SDL_DestroyTexture(texture);
    if (NULL != window)
        SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return statut;
}
