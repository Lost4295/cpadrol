#include <stdio.h>
#include <stdlib.h>


// Truc rapide, mais à améliorer quoi 
int main(int argc, char *argv[])
{
    int choix;
    printf("1. Serveur\n");
    printf("2. Client\n");
    printf("3. Quitter\n");
    printf("Votre choix : ");
    scanf("%d", &choix);
    switch(choix)
    { //TODO mettre les liens vers les fichiers
        case 1:
            printf("Vous avez choisi Serveur\n");
            break;
        case 2:
            printf("Vous avez choisi Client\n");
            break;
        case 3:
            printf("Vous avez choisi Quitter\n");
            break;
        default:
            printf("Choix invalide\n");
            break;
    }
    return 0;
}