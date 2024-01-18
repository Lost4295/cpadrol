
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

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

int main(int argc, char *argv[])
{
    
    initscr();
    noecho();

    int choix = 1;
    int option;
    int exit=1;
        while (exit) {
            choices(choix);
            scanf();
            getch();getch();
            option = getch();
            if (option == '\033') {
                return 0;      
            } else if (option == 10) {
                exit = 0;
            } else if (option == 'A') {
                if(choix == 1){
                    continue;
                } else {
                    choix--;
                }
            } else if (option == 'B') {
                if(choix == 4){
                    continue;
                } else {
                    choix++;
                }
            }
        }

        switch(choix) { 
            case 1:
                system("clear");
                printw("Vous avez choisi Serveur\n");
                sleep(2);
                system("clear");                
                system("./p4s");
                break;
            case 2:
                system("clear");
                printw("Vous avez choisi Client\n");
                sleep(2);
                system("clear");
                system("./p4c");
                break;
            case 3:
                system("clear");
                printw("Vous avez choisi Replays\n");
                sleep(2);
                system("clear");
                system("./replays");
                break;
            case 4: 
                system("clear");
                printw("Vous avez choisi Options\n");
                sleep(2);
                system("clear");
                system("./options");
                break;
        }
    endwin();
    return 0;
}