#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Texture *text;
TTF_Font *font, *tfont, *lfont;
SDL_Rect rect = {100, 100, 100, 100}, dst = {0, 0, 0, 0};
SDL_Rect title = {350, 15, 100, 100};
SDL_Rect authors = {0, 400, 100, 100};
SDL_Color black_color = {0, 0, 0, 255};
SDL_Color yellow_color = {255, 255, 0, 255};
SDL_Color white_color = {255, 255, 255, 255};
int num = 0, nus = 0;
int fplay = 0, freplay = 0, fsettings = 0, fmenu = 1;
char **menu[4];
char **settingsmenu[3];
char *play = "Play";
char *multiplayer = "Watch a replay";
char *settings = "Settings";
char *quit = "Quit";
char *rtm = "Return to Menu";
char *mscolor = "Modify Screen Color";
char *mpseudo = "Modify Pseudo";

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

void print_menu_opts(TTF_Font *font, SDL_Renderer *renderer, int num)
{
    SDL_RenderClear(renderer);
    SDL_Surface *image = IMG_Load("image.jpeg");
    SDL_Texture *img_texture = NULL;
    if (!image)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
    }
    img_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, img_texture, NULL, NULL);
    SDL_Rect rects[4];
    rects[0].x = 50;
    rects[1].x = 50;
    rects[2].x = 50;
    rects[3].x = 50;
    rects[0].y = 140;
    rects[1].y = 220;
    rects[2].y = 300;
    rects[3].y = 380;
    rects[0].w = 100;
    rects[1].w = 100;
    rects[2].w = 100;
    rects[3].w = 100;
    rects[0].h = 100;
    rects[1].h = 100;
    rects[2].h = 100;
    rects[3].h = 100;

    for (int i = 0; i <= 3; i++)
    {
        if (num == i)
        {
            printText(font, renderer, yellow_color, *menu[i], &rects[i]);
        }
        else
        {
            printText(font, renderer, white_color, *menu[i], &rects[i]);
        }
    }
    SDL_RenderPresent(renderer);
}

void print_settings_opts(TTF_Font *font, SDL_Renderer *renderer, int num)
{
    SDL_RenderClear(renderer);
    SDL_Surface *image = IMG_Load("p.jpg");
    SDL_Texture *img_texture = NULL;
    if (!image)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
    }
    img_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, img_texture, NULL, NULL);
    SDL_Rect rects[3];
    rects[0].x = 50;
    rects[1].x = 50;
    rects[2].x = 50;
    rects[0].y = 140;
    rects[1].y = 220;
    rects[2].y = 300;
    rects[0].w = 100;
    rects[1].w = 100;
    rects[2].w = 100;
    rects[0].h = 100;
    rects[1].h = 100;
    rects[2].h = 100;

    for (int i = 0; i <= 2; i++)
    {
        if (num == i)
        {
            printText(font, renderer, yellow_color, *settingsmenu[i], &rects[i]);
        }
        else
        {
            printText(font, renderer, black_color, *settingsmenu[i], &rects[i]);
        }
    }
    SDL_RenderPresent(renderer);
}

int main()
{
    menu[0] = &play;
    menu[1] = &multiplayer;
    menu[2] = &settings;
    menu[3] = &quit;

    settingsmenu[0] = &mscolor;
    settingsmenu[1] = &mpseudo;
    settingsmenu[2] = &rtm;

    /* On fait toutes nos initialisations ici */
    int statut = EXIT_FAILURE;
    if (init(&window, &renderer, 640, 480) < 0)
    {
        goto Quit;
    }
    font = TTF_OpenFont("Roboto.ttf", 32);
    lfont = TTF_OpenFont("Roboto.ttf", 18);
    tfont = TTF_OpenFont("Roboto.ttf", 50);
    if (!font || !lfont || !tfont)
    {
        printf("Error loading font: %s\n", TTF_GetError());
        return -1;
    }
    setWindowColor(renderer, black_color);
    SDL_Surface *image = IMG_Load("p.jpg");
    SDL_Texture *img_texture = NULL;
    if (!image)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
        return -1;
    }
    img_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, img_texture, NULL, NULL);
    printText(tfont, renderer, black_color, "Puissance 4", &title);
    printText(lfont, renderer, black_color, "Realise par Mathis Vareilles, Ylan Turin--Kondi et Zacharie Roger", &authors);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
    print_menu_opts(font, renderer,num);
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
                if (fmenu)
                {
                    (num == 0) ? num = 3 : num--;
                    print_menu_opts(font, renderer, num);
                }
                if (fsettings)
                {
                    (nus == 0) ? nus = 2 : nus--;
                    print_settings_opts(font, renderer, nus);
                }
                break;
            case SDL_SCANCODE_DOWN:
                printf("scancode down\n");
                if (fmenu)
                {
                    (num == 3) ? num = 0 : num++;
                    print_menu_opts(font, renderer, num);
                }
                if (fsettings)
                {
                    (nus == 2) ? nus = 0 : nus++;
                    print_settings_opts(font, renderer, nus);
                }
                break;
            case SDL_SCANCODE_LEFT:
                printf("scancode left\n");
                break;
            case SDL_SCANCODE_RIGHT:
                printf("scancode right\n");
                break;
            case SDL_SCANCODE_ESCAPE:
                // TODO: regarde ou est ce qu'on est et si c'est le dernier étage on quitte
                quit = SDL_TRUE;
                break;
            case SDL_SCANCODE_RETURN:
                if (fmenu)
                {
                    printf("You selected : %s\n", *menu[num]);
                    switch (num)
                    {
                    case 0:
                        break;
                    case 1:
                        freplay = 1;
                        break;
                    case 2:
                        fmenu = 0;
                        fsettings = 1;
                        num=0;
                        print_settings_opts(font, renderer, nus);
                        break;
                    case 3:
                        goto Quit;
                        break;
                    default:
                        break;
                    }
                }
                else if (fsettings)
                {
                    printf("You selected : %s\n", *settingsmenu[nus]);
                    switch (nus)
                    {
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2:
                        fmenu = 1;
                        fsettings = 0;
                        nus=0;
                        print_menu_opts(font, renderer, num);
                        break;
                    default:
                        break;
                    }
                }
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
