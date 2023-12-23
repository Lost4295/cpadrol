#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void choices() {
    printf("1. Serveur\n");
    printf("2. Client\n");
    printf("3. Quitter\n");
    printf("Votre choix : ");
}

// Truc rapide, mais à améliorer quoi 
int main(int argc, char *argv[])
{
    int choix;
    while(true) {
        choices();
        scanf("%d", &choix);
        switch(choix)
        { 
            case 1:
                printf("Vous avez choisi Serveur\n");
                time.sleep(2);
                system("cls")                
                system("p4s.exe")
                break
            case 2:
                printf("Vous avez choisi Client\n");
                time.sleep(2);
                system("cls")
                system("p4c.exe")
                break
            case 3:
                printf("Vous avez choisi Quitter\n");
                time.sleep(2);
                break;
        }
    }
    return 0;
}