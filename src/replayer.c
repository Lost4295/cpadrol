#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define LIGNES 6
#define COLONNES 7
#define VIDE 0
#define ROUGE "\033[31;01;51m"
#define ROUGEW "\033[31;42;01;51m"
#define JAUNEW "\033[33;42;01m"
#define JAUNE "\033[33;01m"
#define BLANC "\033[37;07m"
#define NOIR "\033[30;01m"
#define VERT "\033[32;01m"
#define BLEU "\033[34;01m"
#define END "\033[00m"

void intro();
void printtab();
void createtab();
void launchgame();
void turns(char *p);
void verifyadd(int colonne, int *joueur);
void verifywin(int colonne, int *joueur);
void endgame(int *wjoueur, int result, int mode);
void checkligne(int *joueur);
void checkcol(int *joueur);
void checkdiag(int *joueur);
void checknull();
int tab[LIGNES][COLONNES];
int jactuel = 1;
int *pjactuel = &jactuel;
int animation = 0;
bool end = false;

bool file_exists(const char *filename)
{
    return access(filename, F_OK) == 0;
}
int main()
{

    FILE *file;
    char filename[250];
    chdir("replays");
    do
    {
        printf("Enter the name of the file you want to see.\n");
        scanf("%s", filename);
        strcat(filename, ".txt");
        if (!file_exists(filename))
        {
            printf("File %s does not exist.\n", filename);
        }
    } while (!file_exists(filename));
    char line[250];
    int i = 1;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 1;
    }
    printf("File opened successfully.\n");
    createtab();
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (end)
        {
            break;
        }
        if (i == 1)
        {
            char d[] = "-";
            char *p = strtok(line, d);
            printf("Player 1: %s\n", p);
            p = strtok(NULL, d);
            printf("Player 2: %s\n", p);
            i++;
            continue;
        }
        else
        {
            printf("---------------------------------\nLigne %d : %s\n\n", i, line);
            char d[] = " ";
            char *p = strtok(line, d);
            bool jorc = true;
            while (p != NULL)
            {
                jorc = !jorc;
                if (jorc)
                {
                    printf("fait le Coup %s. \n", p);
                    turns(p);
                }
                else
                {
                    printtab();
                    printf("Joueur %s ", p);
                }
                p = strtok(NULL, d);
            }
            sleep(1);
        }
    }
    fclose(file);
    return 0;
}
void turns(char *p)
{
    int colonne = p[0] - '0';
    printf("\n");
    system("cls");
    verifyadd(colonne, &jactuel);
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
    for (int i = 0; i < LIGNES; i++)
    {
        printf("%s| %s", BLEU, END);
        for (int j = 0; j < COLONNES; j++)
        {
            if (tab[i][j] == 1)
            {
                printf("| %s%c%s ", ROUGE, 48, END);
            }
            else if (tab[i][j] == 2)
            {
                printf("| %s%c%s ", JAUNE, 48, END);
            }
            else if (tab[i][j] == 3)
            {
                printf("| %s%c%s ", ROUGEW, 48, END);
            }
            else if (tab[i][j] == 4)
            {
                printf("| %s%c%s ", JAUNEW, 48, END);
            }
            else
            {
                printf("| %s%c%s ", NOIR, 32, END);
            }
        }
        printf("|\t%s|%s\n", BLEU, END);
    }
    printf("%s ===============================%s\n", BLEU, END);
}

void createtab()
{

    for (int i = 0; i < LIGNES; i++)
    {
        for (int j = 0; j < COLONNES; j++)
        {
            tab[i][j] = VIDE;
        }
    }
}

void verifyadd(int colonne, int *joueur)
{
    for (int i = LIGNES - 1; i >= 0; i--)
    {
        if (tab[i][colonne - 1] == VIDE)
        {
            // commenter ces lignes pour enlever l'animation
            if (animation)
            {
                for (int f = 0; f < i - 1; f++)
                {
                    tab[f][colonne - 1] = *joueur;
                    printtab();
                    usleep(2000);
                    system("cls");
                    tab[f][colonne - 1] = VIDE;
                }
            }
            // ---------------------------------------
            tab[i][colonne - 1] = *joueur;
            break;
        }
    }
    verifywin(colonne, joueur);
    printtab();
}

void verifywin(int colonne, int *joueur)
{
    checkligne(joueur);
    checkcol(joueur);
    checkdiag(joueur);
    checknull();
    if (*joueur == 1)
    {
        *pjactuel = 2;
    }
    else if (*joueur == 2)
    {
        *pjactuel = 1;
    }
}
void checknull()
{
    if (tab[0][0] && tab[0][1] && tab[0][2] && tab[0][3] && tab[0][4] && tab[0][5] && tab[0][6])
    {
        return endgame(NULL, 0, 9);
    }
}

void checkligne(int *joueur)
{
    for (int i = 0; i < LIGNES; i++)
    {
        for (int j = 0; j < COLONNES; j++)
        {
            if (tab[i][j] == tab[i][j + 1] && tab[i][j] == tab[i][j + 2] && tab[i][j] == tab[i][j + 3] && (tab[i][j] == 1 || tab[i][j] == 2) && (j + 3 <= 6))
            {
                if (*joueur == 1)
                {
                    tab[i][j] = 3;
                    tab[i][j + 1] = 3;
                    tab[i][j + 2] = 3;
                    tab[i][j + 3] = 3;
                }
                else
                {
                    tab[i][j] = 4;
                    tab[i][j + 1] = 4;
                    tab[i][j + 2] = 4;
                    tab[i][j + 3] = 4;
                }
                return endgame(joueur, 1, 1);
            }
        }
    }
}
void checkcol(int *joueur)
{
    for (int i = 0; i < LIGNES; i++)
    {
        for (int j = 0; j < COLONNES; j++)
        {
            if (tab[i][j] == tab[i + 1][j] && tab[i][j] == tab[i + 2][j] && tab[i][j] == tab[i + 3][j] && (tab[i][j] == 1 || tab[i][j] == 2) && (i + 3 <= 6))
            {
                if (*joueur == 1)
                {
                    tab[i][j] = 3;
                    tab[i + 1][j] = 3;
                    tab[i + 2][j] = 3;
                    tab[i + 3][j] = 3;
                }
                else
                {
                    tab[i][j] = 4;
                    tab[i + 1][j] = 4;
                    tab[i + 2][j] = 4;
                    tab[i + 3][j] = 4;
                }
                return endgame(joueur, 1, 2);
            }
        }
    }
}
void checkdiag(int *joueur)
{
    for (int i = 0; i < LIGNES; i++)
    {
        for (int j = 0; j < COLONNES; j++)
        {
            if (tab[i][j] == tab[i + 1][j + 1] && tab[i][j] == tab[i + 2][j + 2] && tab[i][j] == tab[i + 3][j + 3] && (tab[i][j] == 1 || tab[i][j] == 2) &&
                (i + 3 <= 6 && j + 3 <= 6))
            {
                if (*joueur == 1)
                {
                    tab[i][j] = 3;
                    tab[i + 1][j + 1] = 3;
                    tab[i + 2][j + 2] = 3;
                    tab[i + 3][j + 3] = 3;
                }
                else
                {
                    tab[i][j] = 4;
                    tab[i + 1][j + 1] = 4;
                    tab[i + 2][j + 2] = 4;
                    tab[i + 3][j + 3] = 4;
                }
                return endgame(joueur, 1, 3);
            }
            else if (tab[i][j] == tab[i + 1][j - 1] && tab[i][j] == tab[i + 2][j - 2] && tab[i][j] == tab[i + 3][j - 3] && (tab[i][j] == 1 || tab[i][j] == 2) &&
                     ((i + 3 <= 6 && j - 3 >= 0)))
            {
                if (*joueur == 1)
                {
                    tab[i][j] = 3;
                    tab[i + 1][j - 1] = 3;
                    tab[i + 2][j - 2] = 3;
                    tab[i + 3][j - 3] = 3;
                }
                else
                {
                    tab[i][j] = 4;
                    tab[i + 1][j - 1] = 4;
                    tab[i + 2][j - 2] = 4;
                    tab[i + 3][j - 3] = 4;
                }
                return endgame(joueur, 1, 4);
            }
        }
    }
}
void endgame(int *wjoueur, int result, int mode)
{
    printtab();
    if (result == 0)
    {
        printf("Aucun joueur n'a réussi a aligner 4 pions. C'est une égalité !");
    }
    else if (result == 1)
    {
        printf("Le joueur %d a gagne !\n", *wjoueur);

        // -------Debug de la fonction qui a trouvé le gagnant-------
        // switch (mode)
        // {
        // case 1:
        //     printf("ligne");
        //     break;
        // case 2:
        //     printf("colonne");
        //     break;
        // case 3:
        //     printf("diagonale qui monte vers la droite");
        //     break;
        // case 4:
        //     printf("diagonale qui monte vers la gauche");
        //     break;
        // }
        // -------Debug de la fonction qui a trouvé le gagnant-------
    }
    sleep(2);
    end = true;
}