#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#define IP_BUFFER_LEN 100
#define BUFFER_SIZE 1000
#define MUSIC 2
unsigned int PORT = 42069;
#define LOOP 3

// TODO : Faire le fichier config.txt (pseudo, couleur, image de fond, musique) ;
// TODO : Implémenter le multijoueur
// TODO : ranger les fichiers dans des dossiers et cleaner tout
// TODO : Handle les errors connes style (config mais pas de config dedans)

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Texture *text;
TTF_Font *font, *tfont, *lfont, *bgfont;
SDL_Rect rect = {100, 100, 100, 100}, dst = {0, 0, 0, 0}, recte = {100, 140, 100, 100}, rectt = {100, 200, 100, 100};
SDL_Rect title = {350, 15, 100, 100};
SDL_Rect authors = {0, 400, 100, 100};
SDL_Color black_color = {0, 0, 0, 255};
SDL_Color red_color = {255, 0, 0, 255};
SDL_Color yellow_color = {255, 255, 0, 255};
SDL_Color white_color = {255, 255, 255, 255};
Mix_Music *actmusic;
Mix_Chunk *sound;

SOCKET sock;

int terr = 0;
char inputText[50];
char ip[50];
int num = 0, nus = 0, nup = 0, nuc = 0, nur = 0, numm = 1;
int flocal = 0, fserver = 0, fclient = 0;
int fmplay = 0, fmreplay = 0, fsettings = 0, fmenu = 0, fplay = 0, fconfig = 1, freplay = 0, fauto = -1, fchmusic = 0, fmauto = 0, floop = 1, fmute = 0;
int ended = 0;
int maxfiles = 0;
char **menu[4];
char **settingsmenu[4];
char **playmenu[4];
char path[50];
char buffer;

char *play = "Play";
char *multiplayer = "Watch a replay";
char *settings = "Settings";
char *quit = "Quit";
char *host = "Héberger la connexion";
char *client = "Se connecter à un serveur";
char *local = "Jouer à deux en local";
char *music = "Changer de musique";
char *retour = "Retour";
char *rtm = "Return to Menu";
char *mscolor = "Modify Screen Color";
char *mpseudo = "Modify Pseudo";
char *loop = "(L)oop music";
char *autoo = "(A)uto play";

#define ROUGE "\033[31;01;51m"
#define JAUNE "\033[33;01m"
#define BLANC "\033[37;07m"
#define NOIR "\033[30;01m"
#define VERT "\033[32;01m"
#define BLEU "\033[34;01m"
#define END "\033[00m"
#define ROUGEW "\033[31;42;01;51m"
#define JAUNEW "\033[33;42;01m"
int width = 1060, height = 880;
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

void musicFinishedCallback()
{
    printf("Music finished.\n");
    (numm == 6) ? (numm = 1) : (numm++);
    char text[50];
    char ftext[50];
    strcpy(text, "music/music");
    char num[2];
    num[0] = numm + '0';
    num[1] = '\0';
    strcat(text, num);
    strcat(text, ".mp3");
    printf("%s\n", text);
    strcpy(ftext, "Music:");
    strcat(ftext, text);
    replacer(MUSIC, ftext);
    replacer(LOOP, "All:1");
    if (!fmute)
    {
        Mix_PlayMusic(Mix_LoadMUS(text), 0);
    }
}

void launchmusic(int i)
{
    if (!fmute)
    {

        char path[30];
        char musique[20];
        char num[2];
        num[0] = i + 1 + '0';
        num[1] = '\0';
        strcpy(path, "Music:");
        strcpy(musique, "music/music");
        strcat(musique, num);
        strcat(musique, ".mp3");
        Mix_HaltMusic();
        Mix_FreeMusic(actmusic);
        strcat(path, musique);
        actmusic = Mix_LoadMUS(musique);
        if (floop == 1)
        {
            Mix_PlayMusic(actmusic, -1);
        }
        else
        {
            Mix_HookMusicFinished(musicFinishedCallback);
            Mix_PlayMusic(actmusic, 0);
        }
        replacer(MUSIC, path);
    }
}

void print_bg()
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
}

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h)
{
    if (0 != SDL_Init(SDL_INIT_EVERYTHING))
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

void printText(TTF_Font *font, SDL_Renderer *renderer, SDL_Color color, char *texte, SDL_Rect *dest, SDL_Color bgcolor)
{
    SDL_Surface *text, *bgtext;

    bgtext = TTF_RenderText_Solid(font, texte, bgcolor);
    if (!bgtext)
    {
        printf("Failed to render bgtext: %s\n", TTF_GetError());
    }
    text = TTF_RenderText_Solid(font, texte, color);
    if (!text)
    {
        printf("Failed to render text: %s\n", TTF_GetError());
    }

    SDL_Texture *text_texture, *bgtext_texture;
    SDL_Rect destbg = {0, 0, bgtext->w, bgtext->h};
    SDL_Rect destn = {0, 0, text->w, text->h};
    destn.x = dest->x;
    destn.y = dest->y;
    destbg.x = dest->x + 2;
    destbg.y = dest->y + 2;
    bgtext_texture = SDL_CreateTextureFromSurface(renderer, bgtext);
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_RenderCopy(renderer, bgtext_texture, NULL, &destbg);
    SDL_RenderCopy(renderer, text_texture, NULL, &destn);
}
void print_main_title()
{
    printText(tfont, renderer, black_color, "Puissance 4", &title, white_color);
    SDL_RenderPresent(renderer);
}
void print_menu_opts(TTF_Font *font, SDL_Renderer *renderer, int num)
{
    print_bg();
    printText(tfont, renderer, black_color, "Puissance 4", &title, white_color);

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
            printText(font, renderer, yellow_color, *menu[i], &rects[i], black_color);
        }
        else
        {
            printText(font, renderer, white_color, *menu[i], &rects[i], black_color);
        }
    }
    SDL_RenderPresent(renderer);
}

void print_play_opts(TTF_Font *font, SDL_Renderer *renderer, int num)
{
print_bg();
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
            printText(font, renderer, yellow_color, *playmenu[i], &rects[i], black_color);
        }
        else
        {
            printText(font, renderer, white_color, *playmenu[i], &rects[i], black_color);
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

    for (int i = 0; i < 4; i++)
    {
        if (num == i)
        {
            printText(font, renderer, yellow_color, *settingsmenu[i], &rects[i], black_color);
        }
        else
        {
            printText(font, renderer, black_color, *settingsmenu[i], &rects[i], white_color);
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
    printText(font, renderer, white_color, "Merci d'entrer votre pseudo.", &rects[0], black_color);
    printText(font, renderer, white_color, texte, &rects[1], black_color);
    SDL_RenderPresent(renderer);
}

void print_ip_renderer(TTF_Font *font, SDL_Renderer *renderer, char *texte)
{
    print_bg();
    SDL_Rect rects[2];
    rects[0].x = 200;
    rects[1].x = 250;
    rects[0].y = 140;
    rects[1].y = 220;
    rects[0].w = 100;
    rects[1].w = 100;
    rects[0].h = 100;
    rects[1].h = 100;
    printText(font, renderer, white_color, "Merci d'entrer l'ip de l'autre personne.", &rects[0], black_color);
    printText(font, renderer, white_color, texte, &rects[1], black_color);
    SDL_RenderPresent(renderer);
}

void get_user_vars(TTF_Font *font, SDL_Renderer *renderer)
{
    SDL_Rect rect;
    rect.x = 480;
    rect.y = 140;
    rect.w = 100;
    rect.h = 100;
    FILE *file = fopen("config.txt", "r");
    char line[250];
    char texte[250];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char d[] = ":";
        char n[] = "\n";
        char m[] = "music/music";
        char *p = strtok(line, d);
        if (strcmp(p, "Pseudo") == 0)
        {
            p = strtok(NULL, d);
            char *ps = strtok(p, n);
            strcpy(texte, "Bonjour, ");
            strcat(texte, ps);
        }
        else if (strcmp(p, "Music") == 0)
        {
            p = strtok(NULL, d);
            char *ps = strtok(p, n);
            strcpy(path, ps);
            char *a = strtok(ps, m);
            char *nt = strtok(a, ".");
            numm = atoi(nt);
        }
        else if (strcmp(p, "All") == 0)
        {
            p = strtok(NULL, d);
            char *ps = strtok(p, n);
            fmauto = atoi(ps);
            (fmauto) ? (floop = 0) : (floop = 1);
        }
    }
    printText(font, renderer, yellow_color, texte, &rect, black_color);

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

int printChooseArrow(SDL_Renderer *renderer, int num)
{
    SDL_Surface *image = IMG_Load("arrow.png");
    SDL_Texture *img_texture = NULL;
    if (!image)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
    }
    img_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_Rect rect = {num * 50, 6 * 50, 50, 50};
    SDL_RenderCopy(renderer, img_texture, NULL, &rect);
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
    if (tableau[0][num] != 0)
    {
        SDL_Rect recter;
        recter.x = 400;
        recter.y = 50;
        printText(font, renderer, red_color, "Colonne pleine !", &recter, black_color);
        return;
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
        printText(lfont, renderer, white_color, turn, &re, black_color);
        SDL_Rect re2;
        re.x = 400;
        re.y = 120;
        if ((fserver && j == 2) || (fclient && j == 1))
        {
            printText(lfont, renderer, white_color, "En attente de l'autre joueur...", &re2, black_color);
        }
    }
}

void turnsreplay(char *p)
{
    int colonne = p[0] - '0';
    printf("\n");
    InsertCoin(renderer, colonne - 1);
}

void createtab()
{

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            tableau[i][j] = 0;
        }
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
            if (tableau[i][j] == 1 || tableau[i][j] == 3)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            else if (tableau[i][j] == 2 || tableau[i][j] == 4)
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
            if (tableau[i][j] == tableau[i][j + 1] && tableau[i][j] == tableau[i][j + 2] && tableau[i][j] == tableau[i][j + 3] && (tableau[i][j] == 1 || tableau[i][j] == 2) && j + 3 < 7)
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
            if (tableau[i][j] == tableau[i + 1][j] && tableau[i][j] == tableau[i + 2][j] && tableau[i][j] == tableau[i + 3][j] && (tableau[i][j] == 1 || tableau[i][j] == 2) && i + 3 < 7)
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
        printText(font, renderer, white_color, egal, &authors, black_color);
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
        printText(font, renderer, white_color, wj, &authors, black_color);
        printf("%d a gagne !\n", *wjoueur);
    }
    int *pended = &ended;
    *pended = 1;
}

void printFileProperties(struct stat stats)
{
    struct tm dt;
    // File permissions
    printf("\nFile access: ");
    if (stats.st_mode & R_OK)
        printf("read ");
    if (stats.st_mode & W_OK)
        printf("write ");
    if (stats.st_mode & X_OK)
        printf("execute");

    // File size
    printf("\nFile size: %d", stats.st_size);

    // Get file creation time in seconds and
    // convert seconds to date and time format
    dt = *(gmtime(&stats.st_ctime));
    printf("\nCreated on: %d-%d-%d %d:%d:%d", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
           dt.tm_hour, dt.tm_min, dt.tm_sec);

    // File modification time
    dt = *(gmtime(&stats.st_mtime));
    printf("\nModified on: %d-%d-%d %d:%d:%d", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
           dt.tm_hour, dt.tm_min, dt.tm_sec);
}

const char *get_ip()
{
    // Read out "hostname -I" command output
    FILE *fd = popen("hostname -I", "r");
    if (fd == NULL)
    {
        fprintf(stderr, "Could not open pipe.\n");
        return NULL;
    }
    // Put output into a string (static memory)
    static char buffer[IP_BUFFER_LEN];
    fgets(buffer, IP_BUFFER_LEN, fd);

    // Only keep the first ip.
    for (int i = 0; i < IP_BUFFER_LEN; ++i)
    {
        if (buffer[i] == ' ')
        {
            buffer[i] = '\0';
            break;
        }
    }

    char *ret = malloc(strlen(buffer) + 1);
    memcpy(ret, buffer, strlen(buffer));
    ret[strlen(buffer)] = '\0';
    printf("%s\n", ret);
    return ret;
}

int printreplayfiles()
{
    struct stat stats;
    DIR *d;
    struct dirent *dir;
    int cpt = 1, i = 0;
    d = opendir("replays");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                cpt++;
            }
        }
        closedir(d);
    }
    printf("cpt= %d\n", cpt);
    chdir("replays");
    SDL_Rect rects[cpt];
    for (int j = 0; j < cpt; j++)
    {
        if (cpt <= 10)
        {
            rects[j].x = width / 2;
            rects[j].y = height / cpt * (j + 1);
            rects[j].h = 100;
            rects[j].w = 100;
        }
        else
        {
            if (j < 10)
            {
                rects[j].x = width / 4 - 50;
                rects[j].y = height / cpt * (j + 1) + 20;
                rects[j].h = 100;
                rects[j].w = 100;
            }
            else
            {
                rects[j].x = width / 4 * 3 - 50;
                rects[j].y = height / (cpt % 10) * (j % 10 + 1);
                rects[j].h = 100;
                rects[j].w = 100;
            }
        }
    }
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                printText(font, renderer, white_color, dir->d_name, &rects[i], black_color);
                i++;
            }
        }
        closedir(d);
    }
    chdir("../");
    return cpt;
}

int printmusicfiles(TTF_Font *font, SDL_Renderer *renderer, int num)
{
    print_bg();
    struct stat stats;
    DIR *d;
    struct dirent *dir;
    int cpt = 1, i = 0;
    d = opendir("music");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                cpt++;
            }
        }
        closedir(d);
    }
    printf("cpt= %d\n", cpt);
    chdir("music");
    SDL_Rect rects[cpt];
    for (int j = 0; j < cpt; j++)
    {
        if (cpt <= 10)
        {
            rects[j].x = width / 2;
            rects[j].y = height / cpt * (j + 1);
            rects[j].h = 100;
            rects[j].w = 100;
        }
        else
        {
            if (j < 10)
            {
                rects[j].x = width / 4 - 50;
                rects[j].y = height / cpt * (j + 1) + 20;
                rects[j].h = 100;
                rects[j].w = 100;
            }
            else
            {
                rects[j].x = width / 4 * 3 - 50;
                rects[j].y = height / (cpt % 10) * (j % 10 + 1);
                rects[j].h = 100;
                rects[j].w = 100;
            }
        }
    }
    d = opendir(".");
    if (d)
    {
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (dir->d_type == DT_REG)
                {
                    char textmus[25];
                    char numt = i + 1 + '0';
                    strcpy(textmus, "Musique ");
                    strcat(textmus, &numt);
                    textmus[9] = '\0';
                    if (i == num)
                    {
                        printText(font, renderer, yellow_color, textmus, &rects[i], black_color);
                        printf(" actual : %d = %s\n", i, dir->d_name);
                    }
                    else
                    {
                        printText(font, renderer, white_color, textmus, &rects[i], black_color);
                    }
                    printf("%d = %s\n", i, dir->d_name);
                    i++;
                }
            }
            closedir(d);
        }
    }
    chdir("../");
    print_music_title();
    return cpt;
}

void print_music_title()
{
    SDL_Rect rects;
    rects.x = 0;
    rects.y = 20;
    rects.w = 100;
    rects.h = 100;
    printText(font, renderer, white_color, "Choisissez une musique à lancer.", &rects, black_color);
    SDL_Rect rect;
    rect.x = 800;
    rect.y = 120;
    rect.w = 100;
    rect.h = 100;
    SDL_Rect rect2;
    rect2.x = 800;
    rect2.y = 200;
    rect2.w = 100;
    rect2.h = 100;
    if (floop)
    {
        printText(font, renderer, yellow_color, loop, &rect, black_color);
    }
    else
    {

        printText(font, renderer, white_color, loop, &rect, black_color);
    }
    if (fmauto)
    {
        printText(font, renderer, yellow_color, autoo, &rect2, black_color);
    }
    else
    {
        printText(font, renderer, white_color, autoo, &rect2, black_color);
    }
    SDL_RenderPresent(renderer);
}

void print_replay_title()
{
    SDL_Rect rects;
    rects.x = 0;
    rects.y = 20;
    rects.w = 100;
    rects.h = 100;
    printText(font, renderer, white_color, "Choisissez un replay à jouer.", &rects, black_color);
    SDL_RenderPresent(renderer);
}

void print_files(TTF_Font *font, SDL_Renderer *renderer, int num)
{
    SDL_RenderClear(renderer);
    SDL_Surface *image = IMG_Load("images.jpeg");
    SDL_Texture *img_texture = NULL;
    if (!image)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
    }
    img_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, img_texture, NULL, NULL);
    print_replay_title();

    struct stat stats;
    DIR *d;
    struct dirent *dir;
    SDL_Rect rects[maxfiles];
    int i = 0;
    for (int j = 0; j < maxfiles; j++)
    {
        if (maxfiles <= 10)
        {
            rects[j].x = width / 2;
            rects[j].y = height / maxfiles * (j + 1);
            rects[j].h = 100;
            rects[j].w = 100;
        }
        else
        {
            if (j < 10)
            {
                rects[j].x = width / 4 - 50;
                rects[j].y = height / maxfiles * (j + 1) + 20;
                rects[j].h = 100;
                rects[j].w = 100;
            }
            else
            {
                rects[j].x = width / 4 * 3 - 50;
                rects[j].y = height / (maxfiles % 10) * (j % 10 + 1);
                rects[j].h = 100;
                rects[j].w = 100;
            }
        }
    }
    d = opendir("replays");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                if (i == num)
                {
                    printText(font, renderer, yellow_color, dir->d_name, &rects[i], black_color);
                    printf(" actual : %d = %s\n", i, dir->d_name);
                }
                else
                {
                    printText(font, renderer, white_color, dir->d_name, &rects[i], black_color);
                }
                printf("%d = %s\n", i, dir->d_name);
                i++;
            }
        }
        closedir(d);
    }
    SDL_RenderPresent(renderer);
}

void getfile(int num)
{
    struct stat stats;
    DIR *d;
    FILE *f;
    struct dirent *dir;
    int i = 0;
    d = opendir("replays");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                if (i == num)
                {
                    chdir("replays");
                    f = fopen(dir->d_name, "r");
                    if (f == NULL)
                    {
                        printf("Failed to open the file.\n");
                        return 1;
                    }
                    printf("File opened successfully.\n");
                    char line[250];
                    int lcount = 1;
                    while (fgets(line, sizeof(line), f) != NULL)
                    {
                        if (ended)
                        {
                            break;
                        }
                        if (lcount == 1)
                        {
                            SDL_Rect re;
                            re.x = 400;
                            re.y = 50;
                            SDL_Rect re2;
                            re2.x = 400;
                            re2.y = 100;
                            SDL_Rect re3;
                            re3.x = 400;
                            re3.y = 150;
                            char d[] = "-";
                            char *p = strtok(line, d);
                            printf("Player 1: %s\n", p);
                            printText(font, renderer, red_color, p, &re, black_color);
                            printText(font, renderer, white_color, "vs", &re2, black_color);
                            p = strtok(NULL, d);
                            printf("Player 2: %s\n", p);
                            printText(font, renderer, yellow_color, p, &re3, black_color);
                            lcount++;
                            SDL_RenderPresent(renderer);
                            SDL_Delay(3000);
                            continue;
                        }
                        else
                        {
                            printf("---------------------------------\nLigne %d : %s\n\n", lcount, line);
                            lcount++;
                            char d[] = " ";
                            char *p = strtok(line, d);
                            bool jorc = true;
                            while (p != NULL)
                            {
                                jorc = !jorc;
                                if (jorc)
                                {
                                    printf("fait le Coup %s. \n", p);
                                    turnsreplay(p);
                                    // if (adv)
                                    //{
                                    //     printf("Press enter to continue.\n");
                                    //     getchar();
                                    // } else {
                                    SDL_Delay(500);
                                    //}
                                }
                                else
                                {
                                    printtab();
                                    loadTableau(renderer);
                                    printf("Joueur %s ", p);
                                }
                                p = strtok(NULL, d);
                            }
                        }
                    }
                    if (!ended)
                    {
                        char *wj = "The game is not finished.";
                        char *wj2 = "There is no winner : the file you have given is not a valid replay.";
                        printText(lfont, renderer, white_color, wj, &authors, black_color);
                        SDL_Rect authors2;
                        authors2.x = authors.x;
                        authors2.y = authors.y + 50;
                        authors2.w = 0;
                        authors2.h = 0;
                        printText(lfont, renderer, white_color, wj2, &authors2, black_color);
                        printf("%s\n", wj);
                        printf("%s\n", wj2);
                        SDL_RenderPresent(renderer);
                    }
                    fclose(f);
                    chdir("../");
                }
                i++;
            }
        }
        closedir(d);
    }
}

void print_ask_auto()
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
    SDL_Rect rects;
    rects.x = 0;
    rects.y = 20;
    rects.w = 100;
    rects.h = 100;
    printText(font, renderer, black_color, "Voulez vous que le replay soit automatique ? ", &rects, white_color);
}

int replacer(int line, char *wrline)
{
    /* File pointer to hold reference of input file */
    FILE *fPtr;
    FILE *fTemp;
    char *path = "config.txt";
    char buffer[BUFFER_SIZE];
    char printline[BUFFER_SIZE];
    char newline[BUFFER_SIZE];
    int count;

    strcpy(printline, wrline);
    strcpy(newline, printline);
    strcat(newline, "\n");
    /*  Open all required files */
    fTemp = fopen("replace.tmp", "w");
    usleep(50);
    fPtr = fopen("config.txt", "r");
    /* fopen() return NULL if unable to open file in given mode. */
    if (fPtr == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open file conf. \n");
        printf("Please check whether file exists and you have read/write privilege.\n");
        exit(EXIT_SUCCESS);
    }
    if (fTemp == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open file temp. \n");
        printf("Please check whether file exists and you have read/write privilege.\n");
        exit(EXIT_SUCCESS);
    }
    /*
     * Read line from source file and write to destination
     * file after replacing given line.
     */
    count = 0;
    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL)
    {
        count++;
        /* If current line is line to replace */
        if (count == line)
            fputs(newline, fTemp);
        else
            fputs(buffer, fTemp);
    }
    /* Close all files to release resource */
    fclose(fPtr);
    fclose(fTemp);
    /* Delete original source file */
    remove("config.txt");
    /* Rename temporary file as original file */
    rename("replace.tmp", "config.txt");
    printf("\nSuccessfully replaced '%d' line with '%s'.\n", line, printline);
    return 0;
}
SOCKET tryconnects()
{
    SOCKADDR_IN sin = {0};
    SOCKET sock;
    SOCKET csock;
    SOCKADDR_IN csin;

    socklen_t recsize = sizeof(csin);
    int sock_err;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /* Si la socket est valide */
    if (sock == INVALID_SOCKET)
    {
        printf("Erreur de creation de la socket client\n");
        return EXIT_FAILURE;
    }
    printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);
    /* Configuration */
    sin.sin_addr.s_addr = INADDR_ANY; /* Adresse IP automatique */
    sin.sin_family = AF_INET;         /* Protocole familial (IP) */
    sin.sin_port = htons(PORT);       /* Listage du port */
    sock_err = bind(sock, (SOCKADDR *)&sin, sizeof(sin));
    /* Si la socket fonctionne */
    if (sock_err == SOCKET_ERROR)
    {
        printf("Errno bind(): %d : %s\n", errno, strerror((errno)));
        perror("Perror bind()");
    }
    /* Démarrage du listage (mode server) */
    sock_err = listen(sock, 5);
    printf("Tentative de connexion au client du port %d...\n", PORT);
    /* Si la socket fonctionne */
    if (sock_err == SOCKET_ERROR)
    {
        printf("Errno listen(): %d : %s\n", errno, strerror((errno)));
        perror("Perror listen()");
        ;
    }
    /* Attente pendant laquelle le client se connecte */
    printf("Patientez pendant que le client se connecte sur le port %d...\n", PORT);
    csock = accept(sock, (SOCKADDR *)&csin, &recsize);
    if (csock == INVALID_SOCKET)
    {
        printf("Erreur lors de l'acceptation de la connexion du client.\n");
        closesocket(sock);
        return EXIT_FAILURE;
    }
    printf("Un client se connecte avec la socket %d de %s:%d\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
    sleep(1);
    createtab();
    system("cls");
    printtab();
    return csock;
}

void closesock(SOCKET sock)
{ /* Fermeture de la socket */
    printf("Fermeture de la socket...\n");
    closesocket(sock);
    printf("Fermeture du serveur terminee ! Appuyez sur entree pour continuer.\n");
    getchar();
    return EXIT_SUCCESS;
}

SOCKET tryconnectc(char *ip)
{
    SOCKET sock;
    SOCKADDR_IN sin;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /* Si la socket est valide */
    if (sock == INVALID_SOCKET)
    {
        printf("Impossible de creer la socket client\n");
        sleep(2);
    }

    printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);
    /* Configuration */
    sin.sin_addr.s_addr = inet_addr(ip); /* Adresse IP automatique */
    sin.sin_family = AF_INET;            /* Protocole familial (IP) */
    sin.sin_port = htons(PORT);          /* Listage du port */

    if (connect(sock, (SOCKADDR *)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("La connexion au serveur a echoue.\n");
        printf("Fermeture de la socket...\n");
        closesocket(sock);
        sleep(2);
    }
    printf("Connection a %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
    sleep(1);
    createtab();
    system("cls");
    printtab();
    return sock;
}