#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

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
char inputText[50];

int num = 0, nus = 0, nup = 0, nuc = 0;
int fsolo = 0, fserver = 0, fclient = 0;
int fmplay = 0, freplay = 0, fsettings = 0, fmenu = 0, fplay = 0, fconfig = 1;
int ended = 0;
char **menu[4];
char **settingsmenu[3];
char **playmenu[4];
char *play = "Play";
char *multiplayer = "Watch a replay";
char *settings = "Settings";
char *quit = "Quit";
char *host = "Héberger la connexion";
char *client = "Se connecter à un serveur";
char *solo = "Jouer à deux en solo";
char *retour = "Retour";
char *rtm = "Return to Menu";
char *mscolor = "Modify Screen Color";
char *mpseudo = "Modify Pseudo";

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
int j = 1;
int *pj = &j;

void append(char *s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}
void pop(char *s)
{
    int len = strlen(s);
    if (len > 0)
    {
        s[len - 1] = '\0';
    }
    else
    {
        printf("Cannot suppress : empty !");
    }
}
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

void print_play_opts(TTF_Font *font, SDL_Renderer *renderer, int num)
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
            printText(font, renderer, yellow_color, *playmenu[i], &rects[i]);
        }
        else
        {
            printText(font, renderer, white_color, *playmenu[i], &rects[i]);
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

void print_pseudo_maker(TTF_Font *font, SDL_Renderer *renderer, char *texte)
{
    SDL_Rect rects[2];
    rects[0].x = 200;
    rects[1].x = 250;
    rects[0].y = 140;
    rects[1].y = 220;
    rects[0].w = 100;
    rects[1].w = 100;
    rects[0].h = 100;
    rects[1].h = 100;
    printText(font, renderer, white_color, "Merci d'entrer votre pseudo.", &rects[0]);
    printText(font, renderer, white_color, texte, &rects[1]);
    SDL_RenderPresent(renderer);
}

void print_hello(TTF_Font *font, SDL_Renderer *renderer)
{
    SDL_Rect rect;
    rect.x = 180;
    rect.y = 10;
    rect.w = 100;
    rect.h = 100;
    FILE *file = fopen("config.txt", "r");
    char line[250];
    char texte[250];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char d[] = ":";
        char n[] = "\n";
        char *p = strtok(line, d);
        if (strcmp(p, "Pseudo") == 0)
        {
            p = strtok(NULL, d);
            char *ps = strtok(p, n);
            strcpy(texte, "Bonjour, ");
            strcat(texte, ps);
        }
    }
    printText(font, renderer, yellow_color, texte, &rect);

    SDL_RenderPresent(renderer);
}

bool file_exists(const char *filename)
{
    return access(filename, 0) == 0;
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

int makeChooseCircle(SDL_Renderer *renderer, int num)
{
    SDL_SetRenderDrawColor(renderer, 83, 28, 125, 14);
    SDL_RenderFillCircle(renderer, num * 50 + 25, 6 * 50 + 25, 5);
    SDL_Delay(50);
    print_turn();
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
    if (*pj == 1)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    else if (*pj == 2)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    }
    int i;
    for (i = 5; i >= 0; i--)
    {
        if (tableau[i][num] == 0)
        {
            tableau[i][num] = *pj;
            SDL_RenderFillCircle(renderer, num * 50 + 25, i * 50 + 25, 20);
            break;
        }
    }
    verifywin(pj);
    (*pj == 1) ? (*pj = 2) : (*pj = 1);
    print_turn();
}

void print_turn()
{
    if (!ended)
    {
        SDL_Rect re;
        re.x = 400;
        re.y = 50;
        char *t = "Joueur ";
        char *a = " , à toi de jouer !";
        char n[2];
        n[0] = *pj + '0';
        n[1] = '\0';
        char turn[50];
        strcpy(turn, t);
        strcat(turn, n);
        strcat(turn, a);
        printText(lfont, renderer, white_color, turn, &re);
    }
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

void verifywin(int *joueur)
{
    checkligne(joueur);
    checkcol(joueur);
    checkdiag(joueur);
    checknull();
}
void checknull()
{
    if (tableau[0][0] && tableau[0][1] && tableau[0][2] && tableau[0][3] && tableau[0][4] && tableau[0][5] && tableau[0][6])
    {
        return endgame(NULL, 0);
    }
}
void checkligne(int *joueur)
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (tableau[i][j] == tableau[i][j + 1] && tableau[i][j] == tableau[i][j + 2] && tableau[i][j] == tableau[i][j + 3] && (tableau[i][j] == 1 || tableau[i][j] == 2))
            {
                if (*joueur == 1)
                {
                    tableau[i][j] = 3;
                    tableau[i][j + 1] = 3;
                    tableau[i][j + 2] = 3;
                    tableau[i][j + 3] = 3;
                }
                else
                {
                    tableau[i][j] = 4;
                    tableau[i][j + 1] = 4;
                    tableau[i][j + 2] = 4;
                    tableau[i][j + 3] = 4;
                }
                return endgame(joueur, 1);
            }
        }
    }
}
void checkcol(int *joueur)
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (tableau[i][j] == tableau[i + 1][j] && tableau[i][j] == tableau[i + 2][j] && tableau[i][j] == tableau[i + 3][j] && (tableau[i][j] == 1 || tableau[i][j] == 2))
            {
                if (*joueur == 1)
                {
                    tableau[i][j] = 3;
                    tableau[i + 1][j] = 3;
                    tableau[i + 2][j] = 3;
                    tableau[i + 3][j] = 3;
                }
                else
                {
                    tableau[i][j] = 4;
                    tableau[i + 1][j] = 4;
                    tableau[i + 2][j] = 4;
                    tableau[i + 3][j] = 4;
                }
                return endgame(joueur, 1);
            }
        }
    }
}
void checkdiag(int *joueur)
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (tableau[i][j] == tableau[i + 1][j + 1] && tableau[i][j] == tableau[i + 2][j + 2] && tableau[i][j] == tableau[i + 3][j + 3] && (tableau[i][j] == 1 || tableau[i][j] == 2))
            {
                if (*joueur == 1)
                {
                    tableau[i][j] = 3;
                    tableau[i + 1][j + 1] = 3;
                    tableau[i + 2][j + 2] = 3;
                    tableau[i + 3][j + 3] = 3;
                }
                else
                {
                    tableau[i][j] = 4;
                    tableau[i + 1][j + 1] = 4;
                    tableau[i + 2][j + 2] = 4;
                    tableau[i + 3][j + 3] = 4;
                }
                return endgame(joueur, 1);
            }
            else if (tableau[i][j] == tableau[i + 1][j - 1] && tableau[i][j] == tableau[i + 2][j - 2] && tableau[i][j] == tableau[i + 3][j - 3] && (tableau[i][j] == 1 || tableau[i][j] == 2))
            {
                if (*joueur == 1)
                {
                    tableau[i][j] = 3;
                    tableau[i + 1][j - 1] = 3;
                    tableau[i + 2][j - 2] = 3;
                    tableau[i + 3][j - 3] = 3;
                }
                else
                {
                    tableau[i][j] = 4;
                    tableau[i + 1][j - 1] = 4;
                    tableau[i + 2][j - 2] = 4;
                    tableau[i + 3][j - 3] = 4;
                }
                return endgame(joueur, 1);
            }
        }
    }
}

void endgame(int *wjoueur, int result)
{
    printtab();
    if (result == 0)
    {
        char *egal = "Aucun joueur n'a réussi a aligner 4 pions. C'est une égalité !";
        printText(font, renderer, white_color, egal, &authors);
        printf("%s\n", egal);
    }
    else if (result == 1)
    {
        char *t = "Le joueur ";
        char *a = " a gagne !";
        char n[2];
        n[0] = *pj + '0';
        n[1] = '\0';
        char wj[50];
        strcpy(wj, t);
        strcat(wj, n);
        strcat(wj, a);
        printText(font, renderer, white_color, wj, &authors);
        printf("%d a gagne !\n", *wjoueur);
    }
    int *pended = &ended;
    *pended = 1;
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

    playmenu[0] = &host;
    playmenu[1] = &client;
    playmenu[2] = &solo;
    playmenu[3] = &retour;
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
    if (file_exists("config.txt"))
    {
        fconfig = 0;
        fmenu = 1;
    }
    else
    {
        fconfig = 1;
        fmenu = 0;
    }
    if (fmenu)
    {
        print_menu_opts(font, renderer, num);
        print_hello(font, renderer);
    }
    else
    {
        SDL_StartTextInput();
        setWindowColor(renderer, black_color);
        print_pseudo_maker(font, renderer, " ");
    }
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while (!quit)
    {
        SDL_bool renderText = SDL_FALSE;
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
            quit = SDL_TRUE;
        else if (event.type == SDL_TEXTINPUT && fconfig)
        {
            // Append character
            append(inputText, *event.text.text);
            renderText = SDL_TRUE;
        }
        if (renderText)
        {
            // Text is not empty
            if (strlen(inputText) > 0)
            {
                // Render new text
                setWindowColor(renderer, black_color);
                print_pseudo_maker(font, renderer, inputText);
            }
            // Text is empty
            else
            {
                printf("Empty text\n");
                // Render space texture
                setWindowColor(renderer, black_color);
                print_pseudo_maker(font, renderer, " ");
            }
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE || event.key.keysym.scancode == SDL_SCANCODE_DELETE && fconfig)
            {
                printf("pop !");
                pop(inputText);
                renderText = SDL_TRUE;
            }
            else
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
                    if (fmplay)
                    {
                        (nup == 0) ? nup = 3 : nup--;
                        print_play_opts(font, renderer, nup);
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
                    if (fmplay)
                    {
                        (nup == 3) ? nup = 0 : nup++;
                        print_play_opts(font, renderer, nup);
                    }
                    if (fsettings)
                    {
                        (nus == 2) ? nus = 0 : nus++;
                        print_settings_opts(font, renderer, nus);
                    }
                    break;
                case SDL_SCANCODE_LEFT:
                    printf("scancode left\n");
                    if (fplay && fsolo)
                    {
                        (nuc == 0) ? nuc = 6 : nuc--;
                        printf("scancode left : nuc = %d\n", nuc);
                        loadTableau(renderer);
                        SDL_Delay(50);
                        makeChooseCircle(renderer, nuc);
                    }

                    break;
                case SDL_SCANCODE_RIGHT:
                    printf("scancode right\n");
                    if (fplay && fsolo)
                    {
                        (nuc == 6) ? nuc = 0 : nuc++;
                        printf("scancode right : nuc = %d\n", nuc);
                        loadTableau(renderer);
                        SDL_Delay(50);
                        makeChooseCircle(renderer, nuc);
                    }

                    break;
                case SDL_SCANCODE_S: // à enlever
                    FILE *file = fopen("config.txt", "r");
                    char line[250];
                    char texte[250];
                    int lc = 0;
                    while (fgets(line, sizeof(line), file) != NULL)
                    {
                        printf("Settings file :\nLine %d :\n", lc);
                        char d[] = ":";
                        char n[] = "\n";
                        printf("Content : '%s'\n", line);
                        char *p = strtok(line, d);
                        printf("Key : %s\n", p);
                        p = strtok(NULL, d);
                        char *ps = strtok(p, n);
                        printf("Value : '%s'\n", ps);
                    }
                    break;
                case SDL_SCANCODE_ESCAPE:
                    if (fmenu)
                    {
                        quit = SDL_TRUE;
                    }
                    else if (fsettings)
                    {
                        fmenu = 1;
                        fsettings = 0;
                        print_menu_opts(font, renderer, num);
                    }
                    break;
                case SDL_SCANCODE_RETURN:
                    if (fconfig)
                    {
                        FILE *fptr;
                        printf("Wrinting '%s' into file\n", inputText);
                        fptr = fopen("config.txt", "w+");
                        fprintf(fptr, "Pseudo:%s\n", inputText);
                        fclose(fptr);
                        fmenu = 1;
                        fconfig = 0;
                        print_menu_opts(font, renderer, num);
                        SDL_StopTextInput();
                    }
                    else if (fmenu)
                    {
                        printf("You selected : %s\n", *menu[num]); // A enlever
                        switch (num)
                        {
                        case 0:
                            fmenu = 0;
                            fmplay = 1;
                            print_play_opts(font, renderer, nup);
                            break;
                        case 1:
                            freplay = 1;
                            break;
                        case 2:
                            fmenu = 0;
                            fsettings = 1;
                            num = 0;
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
                        printf("You selected : %s\n", *settingsmenu[nus]); // A enlever
                        switch (nus)
                        {
                        case 0:
                            break;
                        case 1:
                            fsettings = 0;
                            fconfig = 1;
                            SDL_StartTextInput();
                            print_pseudo_maker(font, renderer, " ");
                            break;
                        case 2:
                            fmenu = 1;
                            fsettings = 0;
                            nus = 0;
                            print_menu_opts(font, renderer, num);
                            break;
                        default:
                            break;
                        }
                    }
                    else if (fmplay)
                    {
                        printf("You selected : %s\n", *playmenu[nup]); // A enlever
                        switch (nup)
                        {
                        case 0:
                            break;
                        case 1:
                            break;
                        case 2:
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
                            print_turn();
                            fmplay = 0;
                            nup = 0;
                            fplay = 1;
                            fsolo = 1;
                            break;
                        case 3:
                            fmenu = 1;
                            fmplay = 0;
                            nup = 0;
                            print_menu_opts(font, renderer, num);
                            break;
                        default:
                            break;
                        }
                    }
                    else if (fplay && fsolo && !ended)
                    {
                        SDL_RenderClear(renderer);
                        loadTableau(renderer);
                        SDL_Delay(50);
                        InsertCoin(renderer, nuc);
                        SDL_RenderPresent(renderer);
                    }
                    else if (ended)
                    {
                        ended = 0;
                        fplay = 0;
                        fmenu = 1;
                        nuc = 0;
                        print_menu_opts(font, renderer, num);
                    }
                    break;
                default:
                    break;
                }
            }
        }
        SDL_Delay(20);
    }
    statut = EXIT_SUCCESS;

    SDL_StopTextInput();
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
