
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int main(void)
{
    initscr();
    cbreak();
    noecho();
    char **options[4];
    char *play = "Play";
    char *multiplayerc = "Multiplayer (Client)";
    char *multiplayers = "Multiplayer (Server)";
    char *settings = "Options";
    char *replays = "Replays";
    options[0] = &play;
    options[1] = &multiplayerc;
    options[2] = &multiplayers;
    options[3] = &settings;
    options[4] = &replays;
    int select = 0;
    char *choose;
    int exit = 0;
    char option;
    printw("hi!");
    while (!exit)
    {
        choices(select);
        option = getch();
        if (option == '\033')
        {            // if the first value is esc
            getch(); // skip the [
            switch (getch())
            { // the real value
            case 'A':
                // code for arrow up
                (select == 0) ? select = 4 : select--;
                clear();
                break;
            case 'B':
                // code for arrow down
                (select == 4) ? select = 0 : select++;
                clear();
                break;
            }
        }
        else
            switch(option) {
                case 10:
                printw("Option selected : %s\n\n", *options[select]);
                switch (select)
                    {
                    case 0:
                        printw("Let's play ! Hahahaha \n\n\n");
                        system("./p4");
                        break;
                    case 1:
                        printw("Multiplayer play !!!!\n\n\n");
                        system("./p4c");
                        break;
                    case 2:
                        printw("Multiplayer play !!!!\n\n\n");
                        system("./p4s");
                        break;
                    case 3:
                        printw("Here are the settings.\n\n");
                        system("./ps");
                        break;
                    case 4:
                        printw("Lest's take a look at the replays !\n\n");
                        system("./replayer");
                        break;
                    default:
                        break;
                    }
                break;
                case 27:
                    end:
                    endwin();
                    exit = 1;
                    break;
                default:
                    break;
                }
            }
    endwin();
    return 0;
}

void choices(int choix) {
    system("clear");   
    printw("\n================ BIENVENUE =================\n\nChoisissez\n\n");
    printw("%sSingle Player\n", (choix==0)?"   " : "");
    printw("%sMultiplayer (Client)\n", (choix==1)?"   " : "");
    printw("%sMultiplayer (Server)\n", (choix==2)?"   " : "");
    printw("%sOptions\n", (choix==3)?"   " : "");
    printw("%sReplays\n", (choix==4)?"   " : "");
    
    printw("\nUtilisez les fleches pour selectionner.\nAppuyez sur \"Entree\" pour confirmer.\nAppuyez sur \"esc\" pour quitter.");
    printw("\n\n\n\n\n COPYRIGHT: All rights reserved (Mathis VAREILLES, Ylan TURIN-KONDI, Zacharie ROGER)");
}