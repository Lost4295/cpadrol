#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    int statut = EXIT_FAILURE;
    SDL_Rect rect = {100, 100, 100, 100}, dst = {0, 0, 0, 0};
    SDL_Color rouge = {255, 0, 0, 255}, bleu = {0, 0, 255, 255};

    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    if (0 != SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_SHOWN, &window, &renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        goto Quit;
    }
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, 200, 200);
    if (NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTexture : %s", SDL_GetError());
        goto Quit;
    }

    SDL_SetRenderTarget(renderer, texture);
    /* La texture est la cible de rendu, maintenant, on dessine sur la texture. */
    SDL_SetRenderDrawColor(renderer, bleu.r, bleu.g, bleu.b, bleu.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, rouge.r, rouge.g, rouge.b, rouge.a);
    SDL_RenderFillRect(renderer, &rect); /* On dessine un rectangle rouge sur la texture. */

    SDL_SetRenderTarget(renderer, NULL); /* Le renderer est la cible de rendu. */

    /* On récupère les dimensions de la texture, on la copie sur le renderer
       et on met à jour l’écran. */
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_RenderPresent(renderer);
    statut = EXIT_SUCCESS;
    SDL_Delay(3000);
    SDL_Surface *tmp = NULL;
    SDL_Texture *texture2 = NULL;
    tmp = SDL_LoadBMP("test.bmp");
    if (NULL == tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        goto Quit;
    }

    texture2 = SDL_CreateTextureFromSurface(renderer, tmp);
    Image(renderer, tmp, dst);
    SDL_FreeSurface(tmp); /* On libère la surface, on n’en a plus besoin */
    if (NULL == texture2)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        goto Quit;
    }

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while(!quit)
    {
    SDL_WaitEvent(&event);
    if(event.type == SDL_QUIT)
        quit = SDL_TRUE;
    else if(event.type == SDL_KEYDOWN)
    {
        if(event.key.keysym.scancode == SDL_SCANCODE_UP)
            printf("scancode up\n");
        if(event.key.keysym.sym == SDLK_UP)
            printf("keysym up\n");
        if(event.key.keysym.scancode == SDL_SCANCODE_DOWN)
            printf("scancode down\n");
        if(event.key.keysym.sym == SDLK_DOWN)
            printf("keysym down\n");
                if(event.key.keysym.scancode == SDL_SCANCODE_LEFT)
            printf("scancode left\n");
        if(event.key.keysym.sym == SDLK_LEFT)
            printf("keysym left\n");
        if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
            printf("scancode RIGHT\n");
        if(event.key.keysym.sym == SDLK_RIGHT)
            printf("keysym RIGHT\n");
        if (event.key.keysym.sym == SDLK_ESCAPE)
            quit = SDL_TRUE;
    }
    SDL_Delay(20);
}

Quit:
    if (NULL != texture)
        SDL_DestroyTexture(texture);
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}

int Image(SDL_Renderer *renderer, SDL_Surface *image, SDL_Rect img_dst)
{
    if (!image)
    {
        fprintf(stderr, "Erreur Image : %s\n", SDL_GetError());
        return 0;
    }
 
    SDL_Texture *image_texture = NULL;
 
    image_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
 
    SDL_RenderCopy(renderer, image_texture , NULL, &img_dst);
 
    SDL_RenderPresent(renderer);

    return 0;
}