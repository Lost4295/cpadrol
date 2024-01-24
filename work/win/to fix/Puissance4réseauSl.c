#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>  /* errno */
#include <unistd.h> /* close */
#include <netdb.h>  /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
unsigned int PORT = 42069;
#define LIGNES 6
#define COLONNES 7
#define VIDE 0
#define ROUGE "\033[31;01;51m"
#define JAUNE "\033[33;01m"
#define BLANC "\033[37;07m"
#define NOIR "\033[30;01m"
#define VERT "\033[32;01m"
#define BLEU "\033[34;01m"
#define END "\033[00m"
#define ROUGEW "\033[31;42;01;51m"
#define JAUNEW "\033[33;42;01m"

void intro();
void onConnectdone();
void printtab();
void createtab();
void launchgame();
int turns();
void verifyadd(int colonne, int *joueur);
void verifywin(int colonne, int *joueur);
void endgame(int *wjoueur, int result);
void checkligne(int *joueur);
void checkcol(int *joueur);
void checkdiag(int *joueur);
void checknull();
int tryconnect();
int tab[LIGNES][COLONNES];
int jactuel = 1;
int *pjactuel = &jactuel;
char buffer;
int ended = 0;
int animation = 0;

int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        animation = 1;
    printf("%s\n",argv[1]);
    }
    intro();
    tryconnect(argv);
}

int tryconnect(char * argv[])
{
    SOCKADDR_IN sin = {0};
    SOCKET sock;
    SOCKET csock;
    SOCKADDR_IN csin;
    socklen_t recsize = sizeof(csin);
    int sock_err;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /* Si la socket est valide */
    if (sock != INVALID_SOCKET)
    {
        printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);
        /* Configuration */
        //inet_pton(AF_INET, argv[1], &(sin.sin_addr));
        sin.sin_addr.s_addr = INADDR_ANY; /* Adresse IP automatique */
        //sin.sin_addr.s_addr = inet_addr(argv[1]);
        sin.sin_family = AF_INET;         /* Protocole familial (IP) */
        sin.sin_port = htons(PORT);       /* Listage du port */
        sock_err = bind(sock, (SOCKADDR *)&sin, sizeof(sin));
        /* Si la socket fonctionne */
        if (sock_err != SOCKET_ERROR)
        {
            /* Démarrage du listage (mode server) */
            sock_err = listen(sock, 5);
            printf("Tentative de connexion au client du port %d...\n", PORT);
            /* Si la socket fonctionne */
            if (sock_err != SOCKET_ERROR)
            {
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
                onConnectdone();
                createtab();
                system("cls");
                printtab();
                while (ended != 1)
                {
                    if (*pjactuel == 1)
                    {
                        int num = turns();
                        buffer = num + '0';
                    sender:
                        sock_err = send(csock, &buffer, sizeof(buffer), 0);
                        if (sock_err != SOCKET_ERROR)
                        {
                            *pjactuel = 2;
                        }
                        else
                        {
                            printf("Erreur de transmission\n");
                            goto sender;
                        }
                    }
                    else
                    {
                        printtab();
                        printf("En attente du joueur %d...\n", *pjactuel);
                        char buffer2;
                        if (recv(csock, &buffer2, sizeof(buffer2), 0) != SOCKET_ERROR)
                        {
                            system("cls");
                            int rnum = buffer2 - '0';
                            verifyadd(rnum, &jactuel);
                            *pjactuel = 1;
                        }
                    }
                }
                shutdown(csock, 2);
            }
        }
        else
        {
            printf("Errno bind(): %d : %s\n", errno, strerror((errno)));
            perror("Perror bind()");
            ;
        }
        /* Fermeture de la socket */

        printf("Fermeture de la socket...\n");
        closesocket(sock);
        printf("Fermeture du serveur terminee ! Appuyez sur entree pour continuer.\n");
    }
    getchar();
    return EXIT_SUCCESS;
}
void intro()
{
    system("cls");
    printf("%s Bonjour !%s\n", BLEU, END);
    printf("Bienvenue dans le jeu du..\n");
    printf("\t\t\t%s    ____  __  ________________ ___    _   ______________   __ __        \n", BLANC);
    printf("\t\t\t   / __ \\/ / / /  _/ ___/ ___//   |  / | / / ____/ ____/  / // /        \n");
    printf("\t\t\t  / /_/ / / / // / \\__ \\\\__ \\/ /| | /  |/ / /   / __/    / // /_        \n");
    printf("\t\t\t / ____/ /_/ // / ___/ /__/ / ___ |/ /|  / /___/ /___   /__  __/        \n");
    printf("\t\t\t/_/    \\____/___//____/____/_/  |_/_/ |_/\\____/_____/     /_/           \n");
    printf("\t\t\t                                                                        \n%s", END);
    printf("Le but du jeu est d'aligner 4 pions de sa couleur.\n");
    printf("Le premier joueur qui aligne 4 pions de sa couleur gagne la partie.\n");
    printf("Pour jouer, il faut entrer le numero de la colonne dans laquelle vous voulez jouer.\n");
    printf("Le jeu se joue a deux joueurs.\n");
    printf("Le joueur 1 joue avec les pions %srouges%s et le joueur 2 joue avec les pions %sjaunes%s.\n", ROUGE, END, JAUNE, END);
    printf("Bonne chance !\n");
    printf("\n");
    printf("Appuyez sur entree pour commencer le jeu.\n");
    getchar();
    printf("\n");
    system("cls");
}
void onConnectdone()
{
    printf("Lancons le jeu !\n");
    sleep(1);
    printf("\n");
    printf("\n");
    printf("Creation du tableau de jeu.");
    sleep(1);
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
int turns()
{
    int colonne;
    do
    {
        printtab();
        printf("Joueur %d, a vous de jouer !\n", *pjactuel);
        printf("Dans quelle colonne voulez-vous jouer ? (Entrez 0 pour quitter la partie.)\n");
        scanf("%d", &colonne);
        printf("\n");
        system("cls");
        if (colonne < 1 || colonne > 7)
        {
            printf("%s==========Erreur !==========\n", ROUGE);
            printf("==>Vous devez entrer un numero de colonne entre 1 et 7.\n");
            printf("==>Veuillez reessayer.\n");
            printf("============================%s\n", END);
            printf("\n");
            continue;
        }
        if (tab[0][colonne - 1] != VIDE)
        {
            printf("%s==========Erreur !==========\n", ROUGE);
            printf("==>Cette colonne est pleine.\n");
            printf("==>Veuillez reessayer.\n");
            printf("============================%s\n", END);
            printf("\n");
            continue;
        }
    } while (colonne <= 0 || colonne > 7);
    verifyadd(colonne, &jactuel);
    return colonne;
}
void verifyadd(int colonne, int *joueur)
{
    if (colonne == 0)
    {
        printf("Vous avez quitte la partie.\n");
        printf("\t\t\t%s     ______  ________   ______  ________   __   __     \n", BLANC);
        printf("\t\t\t     / __ ) \\/ / ____/  / __ ) \\/ / ____/  / /  / /    \n");
        printf("\t\t\t    / __  |\\  / __/    / __  |\\  / __/    / /  / /     \n");
        printf("\t\t\t   / /_/ / / / /___    / /_/ / / / /___   /_/  /_/       \n");
        printf("\t\t\t  /_____/ /_/_____/   /_____/ /_/_____/  (_)  (_)        \n");
        printf("\t\t\t                                                         \n%s", END);
        exit(0);
    }
    else
    {
        for (int i = LIGNES - 1; i >= 0; i--)
        {
            if (tab[i][colonne - 1] == VIDE)
            {
                tab[i][colonne - 1] = *pjactuel;
                break;
            }
        }
        verifywin(colonne, joueur);
    }
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
    else
    {
        printtab();
    }
}
void checknull()
{
    if (tab[0][0] && tab[0][1] && tab[0][2] && tab[0][3] && tab[0][4] && tab[0][5] && tab[0][6])
    {
        return endgame(NULL, 0);
    }
}
void checkligne(int *joueur)
{
    for (int i = 0; i < LIGNES; i++)
    {
        for (int j = 0; j < COLONNES; j++)
        {
            if (tab[i][j] == tab[i][j + 1] && tab[i][j] == tab[i][j + 2] && tab[i][j] == tab[i][j + 3] && (tab[i][j] == 1 || tab[i][j] == 2))
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
                return endgame(joueur, 1);
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
            if (tab[i][j] == tab[i + 1][j] && tab[i][j] == tab[i + 2][j] && tab[i][j] == tab[i + 3][j] && (tab[i][j] == 1 || tab[i][j] == 2))
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
                return endgame(joueur, 1);
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
            if (tab[i][j] == tab[i + 1][j + 1] && tab[i][j] == tab[i + 2][j + 2] && tab[i][j] == tab[i + 3][j + 3] && (tab[i][j] == 1 || tab[i][j] == 2))
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
                return endgame(joueur, 1);
            }
            else if (tab[i][j] == tab[i + 1][j - 1] && tab[i][j] == tab[i + 2][j - 2] && tab[i][j] == tab[i + 3][j - 3] && (tab[i][j] == 1 || tab[i][j] == 2))
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
                return endgame(joueur, 1);
            }
        }
    }
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
void endgame(int *wjoueur, int result)
{
    printtab();
    if (result == 0)
    {
        printf("Aucun joueur n'a réussi a aligner 4 pions. C'est une égalité !");
    }
    else if (result == 1)
    {
        printf("Le joueur %d a gagne !\n", *wjoueur);
    }
    int *pended = &ended;
    *pended = 1;
}