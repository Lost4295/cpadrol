#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void choices(int choix) {
    system("clear");   
    printf("\n================ BIENVENUE =================\n\nChoisissez\n\n");
    printf("%sServeur\n", (choix==1)?"   " : "");
    printf("%sClient\n", (choix==2)?"   " : "");
    printf("%sReplays\n", (choix==3)?"   " : "");
    printf("%sOptions\n", (choix==4)?"   " : "");
    
    printf("\nUtilisez les fleches pour selectionner.\nAppuyez sur \"Entree\" pour confirmer.\nAppuyez sur \"esc\" pour quitter.");
    printf("\n\n\n\n\n COPYRIGHT: All rights reserved (Mathis VAREILLES, Ylan TURIN-KONDI, Zacharie ROGER)");
}

int main(int argc, char *argv[])
{
    int choix = 1;
    int option;
    int exit=1;
        while (exit) {
            choices(choix);
            option = getchar();
            if (option == 27) {
                return 0;      
            } else if (option == 10) {
                exit = 0;
            } else if (option == 65) {
                if(choix == 1){
                    continue;
                } else {
                    choix--;
                }
            } else if (option == 66) {
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
                printf("Vous avez choisi Serveur\n");
                sleep(2);
                system("clear");                
                system("p4s.exe");
                break;
            case 2:
                system("clear");
                printf("Vous avez choisi Client\n");
                sleep(2);
                system("clear");
                system("p4c.exe");
                break;
            case 3:
                system("clear");
                printf("Vous avez choisi Replays\n");
                sleep(2);
                system("clear");
                system("replays.exe");
            case 4: 
                system("clear");
                printf("Vous avez choisi Options\n");
                sleep(2);
                system("clear");
                system("options.exe");
        }
    return 0;
}