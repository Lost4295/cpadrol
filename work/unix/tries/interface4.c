#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
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


void printText(TTF_Font *font, SDL_Renderer *renderer, SDL_Color color, char * texte)
{
    SDL_Surface *text;

    text = TTF_RenderText_Solid(font, texte, color);
    if (!text)
    {
        printf("Failed to render text: %s\n", TTF_GetError());
    }
    SDL_Texture *text_texture;
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_Rect dest = {0, 0, text->w, text->h};
    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
}
int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Texture *text;
    TTF_Font *font;

    SDL_Rect rect = {100, 100, 100, 100}, dst = {0, 0, 0, 0};
    int statut = EXIT_FAILURE;
    if (init(&window, &renderer, 640, 480) < 0)
    {
        goto Quit;
    }
    font = TTF_OpenFont("Roboto.ttf", 25);
	if ( !font ) {
		printf("Error loading font: %s\n",TTF_GetError());
		return -1;
	}
    /* On fait toutes nos initialisations ici */
    SDL_RenderClear(renderer);
    SDL_Color white_color = {255, 255, 255, 255};
    SDL_Color black_color = {0, 0, 0, 255};
    setWindowColor(renderer, white_color);
    SDL_Delay(1000);
    texture = loadImage("test.bmp", renderer);
    SDL_Delay(1000);
    SDL_SetRenderDrawColor(renderer, 0, 225, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_Delay(1000);

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while (!quit)
    {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
            quit = SDL_TRUE;
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_UP:
                printf("scancode up\n");
                rect.y -= 10;
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, &rect);
                SDL_RenderPresent(renderer);
                break;
            case SDL_SCANCODE_DOWN:
                printf("scancode down\n");
                rect.y += 10;
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, &rect);
                SDL_RenderPresent(renderer);
                break;
            case SDL_SCANCODE_LEFT:
                printf("scancode left\n");
                rect.x -= 10;
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, &rect);
                SDL_RenderPresent(renderer);
                break;
            case SDL_SCANCODE_RIGHT:
                printf("scancode right\n");
                rect.x += 10;
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, &rect);
                SDL_RenderPresent(renderer);
                break;
            case SDL_SCANCODE_ESCAPE:
                quit = SDL_TRUE;
                break;
            case SDL_SCANCODE_SPACE:
                SDL_RenderClear(renderer);
                printText(font, renderer, white_color, "Hello World !");
                SDL_RenderPresent(renderer);
                break;
            default:
                break;
            }
        }
        SDL_Delay(20);
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

