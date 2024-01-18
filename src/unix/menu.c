#include "functions.c"

void append(char *s, char c);
void pop(char *s);
void launchmusic(int i);
void print_bg();
void print_ip_renderer(TTF_Font *font, SDL_Renderer *renderer, char *texte);
int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);
SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);
int setWindowColor(SDL_Renderer *renderer, SDL_Color color);
void printText(TTF_Font *font, SDL_Renderer *renderer, SDL_Color color, char *texte, SDL_Rect *dest, SDL_Color bgcolor);
void print_menu_opts(TTF_Font *font, SDL_Renderer *renderer, int num);
void print_play_opts(TTF_Font *font, SDL_Renderer *renderer, int num);
void print_settings_opts(TTF_Font *font, SDL_Renderer *renderer, int num);
void print_pseudo_maker(TTF_Font *font, SDL_Renderer *renderer, char *texte);
void get_user_vars(TTF_Font *font, SDL_Renderer *renderer);
bool file_exists(const char *filename);
void endgame(int *wjoueur, int result);
void print_turn();
void checkligne(int *joueur);
void checkcol(int *joueur);
void checknull();
void checkdiag(int *joueur);
void verifywin(int *joueur);
int SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius);
int printChooseArrow(SDL_Renderer *renderer, int num);
int createTableau(SDL_Renderer *renderer);
int InsertCoin(SDL_Renderer *renderer, int num, FILE *replayfile);
void turnsreplay(char *p);
void createtab();
void printtab();
int loadTableau(SDL_Renderer *renderer);
void print_main_title();
void print_music_title();
void printFileProperties(struct stat stats);
const char *get_ip();
int printreplayfiles();
int printmusicfiles(TTF_Font *font, SDL_Renderer *renderer, int num);
void print_replay_title();
void print_files(TTF_Font *font, SDL_Renderer *renderer, int num);
void replayGame(int num);
void print_ask_auto();
void musicFinishedCallback();
SOCKET tryconnectc(char *ip);
SOCKET tryconnects();
void closesock(SOCKET sock);
int replacer(int line, char *value);
int convertvalue(int v);
void createConfFile();

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(SECRET, argv[1]) == 0)
    {
        printf("You found the secret !\n");
        printf("You can now play with the secret music !\n");
        secret = 1;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Secret", "You found the secret !\nYou can now play with the secret music !", NULL);
    }
    else
    {
        printf("You didn't find the secret :(\n");
        printf("You can't play with the secret music :(\n");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Secret", "You didn't find the secret :(\nYou can't play with the secret music :(", NULL);
        secret = 0;
    }
    menu[0] = &play;
    menu[1] = &replay;
    menu[2] = &settings;
    menu[3] = &quit;

    settingsmenu[0] = &mscolor;
    settingsmenu[1] = &mpseudo;
    settingsmenu[2] = &music;
    settingsmenu[3] = &rtm;

    playmenu[0] = &host;
    playmenu[1] = &client;
    playmenu[2] = &local;
    playmenu[3] = &lookup;
    playmenu[4] = &retour;

    color_knob.y = color_rect.y;
    color_knob.h = color_rect.h;
    color_knob.w = 20;
    color_knob.x = (color_rect.x + color_rect.w) - color_knob.w;
    color_knob2.y = color_rect2.y;
    color_knob2.h = color_rect2.h;
    color_knob2.w = 20;
    color_knob2.x = (color_rect2.x + color_rect2.w) - color_knob2.w;
    color_knob3.y = color_rect3.y;
    color_knob3.h = color_rect3.h;
    color_knob3.w = 20;
    color_knob3.x = (color_rect3.x + color_rect3.w) - color_knob3.w;
    color_knob4.y = color_rect4.y;
    color_knob4.h = color_rect4.h;
    color_knob4.w = 20;
    color_knob4.x = (color_rect4.x + color_rect4.w) - color_knob4.w;

    /* On fait toutes nos initialisations ici */
    int statut = EXIT_FAILURE;
    if (init(&window, &renderer, width, height) < 0)
    {
        goto Quit;
    }
    SDL_Delay(1000);
    SDL_SetWindowMaximumSize(window, 1920, 1080);
    SDL_SetWindowMinimumSize(window, 800, 600);
    DIR *test = opendir("replays");
    long int res = findSize("config.txt");
    // TODO :check si tous les dossiers sont là et sinon les créer
    if (file_exists("config.txt") && test && res)
    {
        fconfig = 0;
        fmenu = 1;
        closedir(test);
        print_menu_opts(font, renderer, num);
        get_user_vars(font, renderer);
        if (path[0] != '\0')
        {
            char text[50];
            strcpy(text, "music/music");
            char num[2];
            num[0] = numm + '0';
            num[1] = '\0';
            strcat(text, num);
            actmusic = Mix_LoadMUS(path);
            if (!actmusic)
            {
                printf("Error loading music: %s\n", Mix_GetError());
                return false;
            }
            // Play music forever
            if (!fmute)
            {
                if (fmauto)
                {
                    Mix_PlayMusic(actmusic, 0);
                    Mix_HookMusicFinished(musicFinishedCallback);
                }
                else
                {
                    Mix_PlayMusic(actmusic, -1);
                }
            }
        }
    }
    else
    {
        fconfig = 1;
        fmenu = 0;
        int dir = mkdir("replays", 777);
        if (!dir)
        {
            printf("Directory created.\n");
        }
        else
        {
            printf("Unable to create directory. Maybe it is already there ?\n");
            DIR *dir = opendir("replays");
            if (dir)
            {
                printf("Directory already exists.\n");
                closedir(dir);
            }
            else if (ENOENT == errno)
            {
                printf("Directory doesn't exist.\n");
            }
            else
            {
                printf("Error while checking if directory exists.\n");
            }
        }
        createConfFile();
        SDL_StartTextInput();
        setWindowColor(renderer, black_color);
        print_pseudo_maker(font, renderer, " ");
    }
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    SDL_bool resized = SDL_FALSE;
    while (!quit)
    {
        SDL_bool renderText = SDL_FALSE;
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
            quit = SDL_TRUE;
        else if (event.type == SDL_TEXTINPUT)
        {
            if (fconfig)
            {
                append(inputText, *event.text.text);
                renderText = SDL_TRUE;
            }
            if (fclient && strlen(inputText) < 15 && (*event.text.text == '.' || *event.text.text == '0' || *event.text.text == '1' || *event.text.text == '2' || *event.text.text == '3' || *event.text.text == '4' || *event.text.text == '5' || *event.text.text == '6' || *event.text.text == '7' || *event.text.text == '8' || *event.text.text == '9'))
            {
                append(inputText, *event.text.text);
                renderText = SDL_TRUE;
            }
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_BACKSPACE || event.key.keysym.sym == SDLK_DELETE && (fconfig || fclient))
            {
                printf("pop !");
                pop(inputText);
                renderText = SDL_TRUE;
            }
            else
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    printf("sym up\n");
                    if (fmenu)
                    {
                        (num == 0) ? num = 3 : num--;
                        print_menu_opts(font, renderer, num);
                    }
                    if (fmplay)
                    {
                        (nup == 0) ? nup = 4 : nup--;
                        print_play_opts(font, renderer, nup);
                    }
                    if (fsettings)
                    {
                        (nus == 0) ? nus = 3 : nus--;
                        print_settings_opts(font, renderer, nus);
                    }
                    if (fmreplay)
                    {
                        (nur == 0) ? nur = maxfiles - 2 : nur--;
                        print_files(font, renderer, nur);
                    }
                    if (fchmusic)
                    {
                        (numm == 0) ? numm = 5 : numm--;
                        printmusicfiles(font, renderer, numm);
                    }
                    break;
                case SDLK_DOWN:
                    printf("sym down\n");
                    if (fmenu)
                    {
                        (num == 3) ? num = 0 : num++;
                        print_menu_opts(font, renderer, num);
                    }
                    if (fmplay)
                    {
                        (nup == 4) ? nup = 0 : nup++;
                        print_play_opts(font, renderer, nup);
                    }
                    if (fsettings)
                    {
                        (nus == 3) ? nus = 0 : nus++;
                        print_settings_opts(font, renderer, nus);
                    }
                    if (fmreplay)
                    {
                        (nur == maxfiles - 2) ? nur = 0 : nur++;
                        print_files(font, renderer, nur);
                    }
                    if (fchmusic)
                    {
                        (numm == 5) ? numm = 0 : numm++;
                        printmusicfiles(font, renderer, numm);
                    }
                    break;
                case SDLK_LEFT:
                    printf("sym left\n '%s'\n", player1ps);
                    if (fplay && flocal && !ended)
                    {
                        (nuc == 0) ? nuc = 6 : nuc--;
                        printf("sym left : nuc = %d\n", nuc);
                        loadTableau(renderer);
                        SDL_Delay(50);
                        printChooseArrow(renderer, nuc);
                    }
                    else if (fplay && fserver && j == 1)
                    {
                        (nuc == 0) ? nuc = 6 : nuc--;
                        printf("sym left : nuc = %d\n", nuc);
                        loadTableau(renderer);
                        SDL_Delay(50);
                        printChooseArrow(renderer, nuc);
                    }
                    else if (fplay && fclient && j == 2)
                    {
                        (nuc == 0) ? nuc = 6 : nuc--;
                        printf("sym left : nuc = %d\n", nuc);
                        loadTableau(renderer);
                        SDL_Delay(50);
                        printChooseArrow(renderer, nuc);
                    }
                    break;
                case SDLK_RIGHT:
                    printf("sym right\n");
                    if (fplay && flocal && !ended)
                    {
                        (nuc == 6) ? nuc = 0 : nuc++;
                        printf("sym right : nuc = %d\n", nuc);
                        loadTableau(renderer);
                        SDL_Delay(50);
                        printChooseArrow(renderer, nuc);
                    }
                    else if (fplay && fserver && j == 1)
                    {
                        (nuc == 6) ? nuc = 0 : nuc++;
                        printf("sym right : nuc = %d\n", nuc);
                        loadTableau(renderer);
                        SDL_Delay(50);
                        printChooseArrow(renderer, nuc);
                    }
                    else if (fplay && fclient && j == 2)
                    {
                        (nuc == 6) ? nuc = 0 : nuc++;
                        printf("sym right : nuc = %d\n", nuc);
                        loadTableau(renderer);
                        SDL_Delay(50);
                        printChooseArrow(renderer, nuc);
                    }
                    break;
                case SDLK_s: // à enlever
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
                case SDLK_ESCAPE:
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
                    else if (fmreplay)
                    {
                        fmenu = 1;
                        fmreplay = 0;
                        print_menu_opts(font, renderer, num);
                    }
                    else if (fmplay)
                    {
                        fmenu = 1;
                        fmplay = 0;
                        print_menu_opts(font, renderer, num);
                    }
                    else if (fplay && flocal)
                    {
                        fmenu = 1;
                        fplay = 0;
                        print_menu_opts(font, renderer, num);
                    }
                    else if (flocal)
                    {
                        fplay = 1;
                        flocal = 0;
                        print_play_opts(font, renderer, nup);
                    }
                    else if (fconfig)
                    {
                        quit = SDL_TRUE;
                    }
                    else if (fchmusic)
                    {
                        fsettings = 1;
                        fchmusic = 0;
                        print_settings_opts(font, renderer, nus);
                    }
                    else if (fserver)
                    {
                        fmenu = 1;
                        fserver = 0;
                        SDLNet_TCP_Close(tcpsock);
                        print_menu_opts(font, renderer, num);
                    }
                    else if (fclient)
                    {
                        fmenu = 1;
                        fclient = 0;
                        SDLNet_TCP_Close(tcpsock);
                        print_menu_opts(font, renderer, num);
                    }
                    else if (freplay)
                    {
                        fmreplay = 1;
                        freplay = 0;
                        SDL_RenderClear(renderer);
                        print_replay_title();
                        maxfiles = printreplayfiles();
                        SDL_RenderPresent(renderer);
                    }
                    else if (fccolor)
                    {
                        fccolor = 0;
                        fsettings = 1;
                        char text[20];
                        char text2[20];
                        char text3[20];
                        char text4[20];
                        strcpy(text, "Red:");
                        strcpy(text2, "Green:");
                        strcpy(text3, "Blue:");
                        strcpy(text4, "Alpha:");
                        char num[4];
                        sprintf(num, "%d", colorR);
                        strcat(text, num);
                        sprintf(num, "%d", colorG);
                        strcat(text2, num);
                        sprintf(num, "%d", colorB);
                        strcat(text3, num);
                        sprintf(num, "%d", colorA);
                        strcat(text4, num);
                        replacer(RED, text);
                        replacer(GREEN, text2);
                        replacer(BLUE, text3);
                        replacer(ALPHA, text4);
                        print_settings_opts(font, renderer, nus);
                    }
                    else if (ended)
                    {
                        ended = 0;
                        fplay = 0;
                        fmenu = 1;
                        flocal = 0;
                        nuc = 0;
                        print_menu_opts(font, renderer, num);
                    }
                    break;
                case SDLK_f: // à enlever
                    printf("Flags :");
                    if (fmenu)
                    {
                        printf(" fmenu");
                    }
                    if (fsettings)
                    {
                        printf(" fsettings");
                    }
                    if (fmreplay)
                    {
                        printf(" fmreplay");
                    }
                    if (fmplay)
                    {
                        printf(" fmplay");
                    }
                    if (fplay)
                    {
                        printf(" fplay");
                    }
                    if (flocal)
                    {
                        printf(" flocal");
                    }
                    if (fconfig)
                    {
                        printf(" fconfig");
                    }
                    if (fserver)
                    {
                        printf(" fserver");
                    }
                    if (fclient)
                    {
                        printf(" fclient");
                    }
                    if (freplay)
                    {
                        printf(" freplay");
                    }
                    if (fauto)
                    {
                        printf(" fauto");
                    }
                    if (ended)
                    {
                        printf(" ended");
                    }
                    if (fchmusic)
                    {
                        printf(" fchmusic");
                    }
                    if (fmauto)
                    {
                        printf(" fmauto");
                    }
                    if (floop)
                    {
                        printf(" floop");
                    }
                    if (fmute)
                    {
                        printf(" fmute");
                    }
                    if (secret)
                    {
                        printf(" secret");
                    }
                    printf("\n");
                    printf("Variables :");
                    printf(" num = %d", num);
                    printf(" nus = %d", nus);
                    printf(" nup = %d", nup);
                    printf(" nur = %d", nur);
                    printf(" nuc = %d", nuc);
                    printf("numm = %d", numm);
                    printf(" j = %d", j);
                    printf("colorR = %d", colorR);
                    printf(" colorG = %d", colorG);
                    printf(" colorB = %d", colorB);
                    printf(" colorA = %d", colorA);
                    printf(" pj = %d", j);
                    printf(" maxfiles = %d", maxfiles);
                    printf("\n");
                    printf("\n");
                    break;
                case SDLK_RETURN:
                    if (fconfig)
                    {
                        char etet[70];
                        strcpy(etet, "Pseudo:");
                        strcat(etet, inputText);
                        replacer(PSEUDO, etet);
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
                            fmreplay = 1;
                            fmenu = 0;
                            SDL_RenderClear(renderer);
                            print_replay_title();
                            maxfiles = printreplayfiles();
                            SDL_RenderPresent(renderer);
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
                            fsettings = 0;
                            fccolor = 1;
                            print_bg();
                            print_color();
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Changement de Couleur", "Appuyez sur Echap pour quitter.", NULL);
                            break;
                        case 1:
                            fsettings = 0;
                            fconfig = 1;
                            SDL_StartTextInput();
                            print_pseudo_maker(font, renderer, " ");
                            break;
                        case 2:
                            fsettings = 0;
                            fchmusic = 1;
                            printmusicfiles(font, renderer, numm);
                            break;
                        case 3:
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
                            fmplay = 0;
                            fserver = 1;
                            print_bg();
                            printText(font, renderer, white_color, "Votre IP est :", &rect, black_color);
                            printText(font, renderer, white_color, get_ip(), &recte, black_color);
                            printText(font, renderer, white_color, "Donnez cette adresse IP à votre adversaire !", &rectt, black_color);
                            SDL_RenderPresent(renderer);

                            tcpsock = createServer();
                            fplay = 1;
                            SDL_RenderClear(renderer);
                            createtab();
                            createReplay();
                            createTableau(renderer);
                            loadTableau(renderer);
                            break;
                        case 1:
                            SDL_StartTextInput();
                            fmplay = 0;
                            fclient = 1;
                            print_ip_renderer(font, renderer, " ");
                            break;
                        case 2:
                            createtab();
                            SDL_SetRenderDrawColor(renderer, 147, 172, 234, 145);
                            SDL_RenderFillRect(renderer, NULL);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(500);
                            loadSounds();
                            fmplay = 0;
                            nup = 0;
                            fplay = 1;
                            flocal = 1;
                            createReplay();
                            createTableau(renderer);
                            print_turn();
                            break;
                        case 3:
                            fmplay = 0;
                            fclient = 1;
                            // todo faire le menu avec ZACHARIE
                            break;
                        case 4:
                            fmenu = 1;
                            fmplay = 0;
                            nup = 0;
                            print_menu_opts(font, renderer, num);
                            break;
                        default:
                            break;
                        }
                    }
                    else if (fplay && flocal && !ended)
                    {
                        SDL_RenderClear(renderer);
                        loadTableau(renderer);
                        SDL_Delay(50);

                        InsertCoin(renderer, nuc, replayfile);
                        SDL_RenderPresent(renderer);
                    }
                    else if (ended)
                    {
                        ended = 0;
                        fplay = 0;
                        fmenu = 1;
                        flocal = 0;
                        nuc = 0;
                        j = 1;
                        closereplay();
                        createtab();
                        print_menu_opts(font, renderer, num);
                    }
                    else if (fmreplay)
                    {

                        replayGame(nur);
                        createtab();
                        fmreplay = 0;
                        fmenu = 1;
                    }
                    else if (fchmusic)
                    {
                        launchmusic(numm);
                        if (fmauto)
                        {
                            replacer(LOOP, "All:1");
                        }
                        else
                        {
                            replacer(LOOP, "All:0");
                        }
                        fchmusic = 0;
                        fsettings = 1;
                        print_settings_opts(font, renderer, nus);
                    }
                    else if (fclient && !fplay)
                    {
                        printf("Trying to connect to server...\n");
                        SDL_StopTextInput();
                        char *ip = inputText;
                        printf("IP : %s\n", ip);
                        tcpsock = createClient(ip);
                        fplay = 1;
                        SDL_RenderClear(renderer);
                        createtab();
                        createReplay();
                        createTableau(renderer);
                        loadTableau(renderer);
                        print_turn();
                    }
                    else if (fclient && fplay && !ended && j == 2) // client turn -> serveur
                    {
                        SDL_RenderClear(renderer);
                        loadTableau(renderer);
                        SDL_Delay(50);
                        sendMove(tcpsock, nuc);
                        InsertCoin(renderer, nuc, replayfile);
                        SDL_RenderPresent(renderer);
                    }
                    else if (fserver && fplay && !ended && j == 1)
                    {
                        SDL_RenderClear(renderer);
                        loadTableau(renderer);
                        SDL_Delay(50);
                        sendMove(tcpsock, nuc);
                        // TODO erase
                        //    buffer = nuc + '0';
                        // senders:
                        //    if (send(sock, &buffer, sizeof(buffer), 0) == SOCKET_ERROR)
                        //    {
                        //        printf("Erreur de transmission\n");
                        //        sendErrNum++;
                        //        if (sendErrNum < 5)
                        //        {
                        //            goto senders;
                        //        }
                        //        else
                        //        {
                        //            printf("Connexion perdue.");
                        //            closesocket(sock);
                        //            SDL_Rect recter;
                        //            recter.x = 400;
                        //            recter.y = 50;
                        //            printText(font, renderer, red_color, "La connexion a ete perdue.", &recter, black_color);
                        //            SDL_Delay(2000);
                        //            fplay = 0;
                        //            fmenu = 1;
                        //            j = 1;
                        //            fclient = 0;
                        //            fserver = 0;
                        //        }
                        //    }
                        // TODO enderase
                        InsertCoin(renderer, nuc, replayfile);
                        SDL_RenderPresent(renderer);
                    }
                    else if (adv && needenter)
                    {
                        needenter = false;
                    }
                    break;
                case SDLK_m:
                    // mute music and all sounds
                    fmute = !fmute;
                    if (fmute)
                    {
                        Mix_PauseMusic();
                        Mix_Pause(-1);
                        replacer(MUTE, "Mute:1");
                    }
                    else
                    {
                        Mix_ResumeMusic();
                        Mix_Resume(-1);
                        replacer(MUTE, "Mute:0");
                    }
                    break;
                case SDLK_p:
                {
                    // pause music
                    if (Mix_PausedMusic() == 1)
                    {
                        Mix_ResumeMusic();
                    }
                    else
                    {
                        Mix_PauseMusic();
                    }
                    break;
                }
                case SDLK_a:
                    // loop through music
                    printf("aaaaa\n");
                    if (fchmusic)
                    {
                        printmusicfiles(font, renderer, numm);
                        floop = 0;
                        fmauto = 1;
                        Mix_HookMusicFinished(musicFinishedCallback);
                    }
                    break;
                case SDLK_l:
                    printf("lllll\n");
                    // auto play music
                    if (fchmusic)
                    {
                        printmusicfiles(font, renderer, numm);
                        Mix_PlayMusic(actmusic, -1);
                        fmauto = 0;
                        floop = 1;
                    }
                    break;
                case SDLK_PLUS:
                case SDLK_KP_PLUS:
                    if (Mix_VolumeMusic(-1) < 128)
                    {
                        Mix_VolumeMusic(Mix_VolumeMusic(-1) + 2);
                    }
                    break;
                case SDLK_MINUS:
                case SDLK_KP_MINUS:
                    if (Mix_VolumeMusic(-1) > 0)
                    {
                        Mix_VolumeMusic(Mix_VolumeMusic(-1) - 2);
                    }
                    break;
                default:
                    break;
                }
            }
        }
        if (event.type == SDL_WINDOWEVENT)
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                SDL_Log("Window %d resized to %dx%d",
                        event.window.windowID, event.window.data1,
                        event.window.data2);
                resized = SDL_TRUE;
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                SDL_Log("Window %d size changed to %dx%d",
                        event.window.windowID, event.window.data1,
                        event.window.data2);
                resized = SDL_TRUE;
                break;
            }
            if (resized)
            {
                resized = SDL_FALSE;
                reprint(renderer);
            }
        }

        else if (event.type == SDL_MOUSEMOTION)
        {
            const SDL_Point pt = {event.motion.x, event.motion.y};
            if (fccolor)
            {
                if (SDL_PointInRect(&pt, &hcolor_rect) && (event.motion.state & SDL_BUTTON_LMASK))
                {
                    const float fx = (float)(pt.x - color_rect.x);
                    color_slider_value = (fx / ((float)color_rect.w)); // a value between 0.0f and 1.0f
                    printf("SLIDING 1! At %dx%d (%d percent)\n", pt.x, pt.y, (int)SDL_round(color_slider_value * 100.0f));
                    color_knob.x = pt.x - (color_knob.w / 2);
                    color_knob.x = SDL_max(color_knob.x, color_rect.x);
                    color_knob.x = SDL_min(color_knob.x, (color_rect.x + color_rect.w) - color_knob.w);
                }
                else if (SDL_PointInRect(&pt, &hcolor_rect2) && (event.motion.state & SDL_BUTTON_LMASK))
                {
                    const float fx = (float)(pt.x - color_rect2.x);
                    color_slider_value2 = (fx / ((float)color_rect2.w)); // a value between 0.0f and 1.0f
                    printf("SLIDING 2 ! At %dx%d (%d percent)\n", pt.x, pt.y, (int)SDL_round(color_slider_value2 * 100.0f));
                    color_knob2.x = pt.x - (color_knob2.w / 2);
                    color_knob2.x = SDL_max(color_knob2.x, color_rect2.x);
                    color_knob2.x = SDL_min(color_knob2.x, (color_rect2.x + color_rect2.w) - color_knob2.w);
                }
                else if (SDL_PointInRect(&pt, &hcolor_rect3) && (event.motion.state & SDL_BUTTON_LMASK))
                {
                    const float fx = (float)(pt.x - color_rect3.x);
                    color_slider_value3 = (fx / ((float)color_rect3.w)); // a value between 0.0f and 1.0f
                    printf("SLIDING 3! At %dx%d (%d percent)\n", pt.x, pt.y, (int)SDL_round(color_slider_value3 * 100.0f));
                    color_knob3.x = pt.x - (color_knob3.w / 2);
                    color_knob3.x = SDL_max(color_knob3.x, color_rect3.x);
                    color_knob3.x = SDL_min(color_knob3.x, (color_rect3.x + color_rect3.w) - color_knob3.w);
                }
                else if (SDL_PointInRect(&pt, &hcolor_rect4) && (event.motion.state & SDL_BUTTON_LMASK))
                {
                    const float fx = (float)(pt.x - color_rect4.x);
                    color_slider_value4 = (fx / ((float)color_rect4.w)); // a value between 0.0f and 1.0f
                    printf("SLIDING 4! At %dx%d (%d percent)\n", pt.x, pt.y, (int)SDL_round(color_slider_value4 * 100.0f));
                    color_knob4.x = pt.x - (color_knob4.w / 2);
                    color_knob4.x = SDL_max(color_knob4.x, color_rect4.x);
                    color_knob4.x = SDL_min(color_knob4.x, (color_rect4.x + color_rect4.w) - color_knob4.w);
                }
                print_bg();
                print_color();
            }
        }
        if (fplay && !ended && (fclient || fserver))
        {
            print_turn();
            printf("En attente du joueur %d...\n", *pj);
            if (fclient && j == 1)
            {
                SDL_RenderPresent(renderer);
                int ec = receiveMove(tcpsock);
                SDL_RenderClear(renderer);
                printtab();
                loadTableau(renderer);
                InsertCoin(renderer, ec, replayfile);
                SDL_RenderPresent(renderer);
                // TODO erase
                // char cbuffer;
                // int ec;
                // ec = recv(sock, &cbuffer, sizeof(cbuffer), 0);
                // if (ec != SOCKET_ERROR)
                //{
                //    int rnum = cbuffer - '0';
                //    InsertCoin(renderer, rnum, replayfile);
                //    SDL_RenderPresent(renderer);
                //}
                // else if (ec == 0 || ec == SOCKET_ERROR)
                //{
                //    printf("Connexion perdue.");
                //    closesocket(sock);
                //    SDL_Rect recter;
                //    recter.x = 400;
                //    recter.y = 50;
                //    printText(font, renderer, red_color, "La connexion a ete perdue.", &recter, black_color);
                //    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Connexion perdue", "La connexion a ete perdue!", NULL);
                //    SDL_Delay(2000);
                //    fplay = 0;
                //    fmenu = 1;
                //    j = 1;
                //    fclient = 0;
                //    fserver = 0;
                //}
                // TODO enderase
            }
            else if (fserver && j == 2)
            {
                SDL_RenderPresent(renderer);
                int ec = receiveMove(tcpsock);
                SDL_RenderClear(renderer);
                printtab();
                loadTableau(renderer);
                InsertCoin(renderer, ec, replayfile);
                SDL_RenderPresent(renderer);
                // TODO erase
                // char buffer2;
                // int ec;
                // ec = recv(sock, &buffer2, sizeof(buffer2), 0);
                // if (ec != SOCKET_ERROR)
                //{
                //    int rnum = buffer2 - '0';
                //    InsertCoin(renderer, rnum, replayfile);
                //    SDL_RenderPresent(renderer);
                //}
                // else if (ec == 0 || ec == SOCKET_ERROR)
                //{
                //    printf("Connexion perdue.");
                //    closesocket(sock);
                //    SDL_Rect recter;
                //    recter.x = 400;
                //    recter.y = 50;
                //    printText(font, renderer, red_color, "La connexion a ete perdue.", &recter, black_color);
                //    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Connexion perdue", "La connexion a ete perdue!", NULL);
                //    SDL_Delay(2000);
                //    fplay = 0;
                //    fmenu = 1;
                //    j = 1;
                //    fclient = 0;
                //    fserver = 0;
                //}
                // TODO enderase
            }
        }
        if (renderText)
        {
            // Text is not empty
            if (strlen(inputText) > 0)
            {
                // Render new text
                if (fconfig)
                {
                    setWindowColor(renderer, black_color);
                    print_pseudo_maker(font, renderer, inputText);
                }
                if (fclient)
                {
                    print_bg();
                    print_ip_renderer(font, renderer, inputText);
                    printf("gothee\n"); // effacer
                }
            }
            // Text is empty
            else
            {
                printf("Empty text\n");
                // Render space texture
                if (fconfig)
                {
                    setWindowColor(renderer, black_color);
                    print_pseudo_maker(font, renderer, " ");
                }
                if (fclient)
                {
                    print_bg();
                    print_ip_renderer(font, renderer, " ");
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
    TTF_CloseFont(lfont);
    TTF_CloseFont(tfont);
    TTF_Quit();
    SDLNet_Quit();
    Mix_FreeChunk(sound1);
    Mix_FreeChunk(sound2);
    Mix_FreeChunk(sound3);
    Mix_FreeChunk(sound4);
    Mix_FreeChunk(error);
    Mix_FreeChunk(win);
    Mix_FreeChunk(actsound);
    Mix_FreeMusic(actmusic);
    SDL_Quit();
    actmusic = NULL;
    actsound = NULL;
    return statut;
}