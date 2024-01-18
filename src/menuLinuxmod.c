
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
    char *multiplayer = "Multiplayer";
    char *settings = "Options";
    char *quit = "Quit";
    options[0] = &play;
    options[1] = &multiplayer;
    options[2] = &settings;
    options[3] = &quit;
    int select = 0;
    char *choose;
    int exit = 0;
    printw("hi!");
    while (!exit)
    {
        choices(select);
        if (getch() == '\033')
        {            // if the first value is esc
            getch(); // skip the [
            switch (getch())
            { // the real value
            case 'A':
                // code for arrow up
                (select == 0) ? select = 3 : select--;
                clear();
                break;
            case 'B':
                // code for arrow down
                (select == 3) ? select = 0 : select++;
                clear();
                break;
            case 'C':
                printw("Option selected : %s\n\n", *options[select]);
                switch (select)
                {
                case 0:
                    printw("Let's play ! Hahahaha \n\n\n");
                    break;
                case 1:
                    printw("Multiplayer play !!!!\n\n\n");
                    break;
                case 2:
                    printw("Here are the options.\n\n");
                    break;
                case 3:
                    printw("Goodbye !\n\n");
                    goto end;
                    break;
                default:
                    break;
                }
                break;
            case 'D':
            end:
                // code for arrow left
                endwin();
                exit = 1;
                break;
            default:
                break;
            }
        }
    }
    endwin();
    return 0;
}

void choices(int choix) {
    system("clear");   
    printw("\n================ BIENVENUE =================\n\nChoisissez\n\n");
    printw("%sServeur\n", (choix==1)?"   " : "");
    printw("%sClient\n", (choix==2)?"   " : "");
    printw("%sReplays\n", (choix==3)?"   " : "");
    printw("%sOptions\n", (choix==4)?"   " : "");
    
    printw("\nUtilisez les fleches pour selectionner.\nAppuyez sur \"Entree\" pour confirmer.\nAppuyez sur \"esc\" pour quitter.");
    printw("\n\n\n\n\n COPYRIGHT: All rights reserved (Mathis VAREILLES, Ylan TURIN-KONDI, Zacharie ROGER)");
}