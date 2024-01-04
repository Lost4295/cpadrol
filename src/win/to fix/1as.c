#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
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
void turns();
void verifyadd(int colonne, int *joueur);
void verifywin(int colonne, int *joueur);
void endgame(int *wjoueur, int result, int mode);
void checkligne(int *joueur);
void checkcol(int *joueur);
void checkdiag(int *joueur);
int getcolonne();
void checknull();
int tab[LIGNES][COLONNES];
int jactuel = 1;
int *pjactuel = &jactuel;
int animation = 0;

HANDLE hComm;                       // Handle to the Serial port
char ComPortName[] = "\\\\.\\COM9"; // Name of the Serial port(May Change) to be opened,
BOOL Status;                        // Status of the various operations
DWORD dwEventMask;                  // Event mask to trigger
char TempChar;                      // Temperory Character
char SerialBuffer[20];              // Buffer Containing Rxed Data
DWORD NoBytesRead;                  // Bytes read by ReadFile()
int i = 0;
int counter = 0;
DWORD bytesRead;
char bytes_to_send[2];
char *one = "1";
char *two = "2";
void main(int argc, char *argv[])
{
    printf("\n\n +==========================================+");
    printf("\n |    Serial Port  Reception (Win32 API)    |");
    printf("\n +==========================================+\n");
    /*---------------------------------- Opening the Serial Port -------------------------------------------*/

    hComm = CreateFile(ComPortName,                  // Name of the Port to be Opened
                       GENERIC_READ | GENERIC_WRITE, // Read/Write Access
                       0,                            // No Sharing, ports cant be shared
                       NULL,                         // No Security
                       OPEN_EXISTING,                // Open existing port only
                       0,                            // Non Overlapped I/O
                       NULL);                        // Null for Comm Devices

    if (hComm == INVALID_HANDLE_VALUE)
    {
        printf("\n    Error! - Port %s can't be opened\n", ComPortName);
        exit(1);
    }
    else
    {
        printf("\n    Port %s Opened\n ", ComPortName);
    }
    /*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

    DCB dcbSerialParams = {0}; // Initializing DCB structure
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    Status = GetCommState(hComm, &dcbSerialParams); // retreives  the current settings

    if (Status == FALSE)
    {
        printf("\n    Error! in GetCommState()");
        exit(1);
    }

    dcbSerialParams.BaudRate = CBR_9600;   // Setting BaudRate = 9600
    dcbSerialParams.ByteSize = 8;          // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT; // Setting StopBits = 1
    dcbSerialParams.Parity = NOPARITY;     // Setting Parity = None

    Status = SetCommState(hComm, &dcbSerialParams); // Configuring the port according to settings in DCB

    if (Status == FALSE)
    {
        printf("\n    Error! in Setting DCB Structure");
        exit(1);
    }
    else // If Successfull display the contents of the DCB Structure
    {
        printf("\n\n    Setting DCB Structure Successfull\n");
        printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
        printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
        printf("\n       StopBits = %d", dcbSerialParams.StopBits);
        printf("\n       Parity   = %d", dcbSerialParams.Parity);
    }

    /*------------------------------------ Setting Timeouts --------------------------------------------------*/

    // COMMTIMEOUTS timeouts = {0};
    // timeouts.ReadIntervalTimeout = MAXDWORD;
    // timeouts.ReadTotalTimeoutConstant = 0;
    // timeouts.ReadTotalTimeoutMultiplier = 0;
    // timeouts.WriteTotalTimeoutConstant = 0;
    // timeouts.WriteTotalTimeoutMultiplier = 0;

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 10;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 5000;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (SetCommTimeouts(hComm, &timeouts) == FALSE)
    {
        printf("\n\n    Error! in Setting Time Outs");
        exit(1);
    }
    else
    {
        printf("\n\n    Setting Serial Port Timeouts Successfull");
    }

    /*------------------------------------ Setting Receive Mask ----------------------------------------------*/
    dwEventMask = EV_RXCHAR;
    Status = SetCommMask(hComm, dwEventMask); // Configure Windows to Monitor the serial device for Character Reception

    if (Status == FALSE)
    {
        printf("\n\n    Error! in Setting CommMask");
        exit(1);
    }
    else
    {
        printf("\n\n    Setting CommMask successfull");
    }

    if (argc >= 2)
    {
        animation = 1;
    }

    intro();
    /*------------------------------------ Setting WaitComm() Event   ----------------------------------------*/
}

void intro()
{
    system("cls");
    system("cls");
    printf("%s Bonjour !%s\n", BLEU, END);
    if (animation)
    {
        sleep(1);
    }
    printf("Bienvenue dans le jeu du..\n");
    if (animation)
    {
        usleep(50000);
    }

    printf("\t\t\t%s    ____  __  ________________ ___    _   ______________   __ __        \n", BLANC);
    printf("\t\t\t   / __ \\/ / / /  _/ ___/ ___//   |  / | / / ____/ ____/  / // /        \n");
    printf("\t\t\t  / /_/ / / / // / \\__ \\\\__ \\/ /| | /  |/ / /   / __/    / // /_        \n");
    printf("\t\t\t / ____/ /_/ // / ___/ /__/ / ___ |/ /|  / /___/ /___   /__  __/        \n");
    printf("\t\t\t/_/    \\____/___//____/____/_/  |_/_/ |_/\\____/_____/     /_/           \n");
    printf("\t\t\t                                                                        \n%s", END);
    // Version sans animation
    if (!animation)
    {
        printf("Le but du jeu est d'aligner 4 pions de sa couleur.\n");
        printf("Le premier joueur qui aligne 4 pions de sa couleur gagne la partie.\n");
        printf("Pour jouer, il faut entrer le numero de la colonne dans laquelle vous voulez jouer.\n");
        printf("Le jeu se joue a deux joueurs.\n");
        printf("Le joueur 1 joue avec les pions %srouges%s et le joueur 2 joue avec les pions %sjaunes%s.\n", ROUGE, END, JAUNE, END);
        printf("Bonne chance !\n");
        printf("\n");
        printf("Appuyez sur entree pour commencer le jeu.\n");
    }
    // Version avec animation
    else
    {
        sleep(1);
        char *text1 = "Le but du jeu est d'aligner 4 pions de sa couleur.\nLe premier joueur qui aligne 4 pions de sa couleur gagne la partie.\nPour jouer, il faut entrer le numero de la colonne dans laquelle vous voulez jouer.\nLe jeu se joue a deux joueurs.\nLe joueur 1 joue avec les pions";
        char *text2 = "et le joueur 2 joue avec les pions ";
        char *text3 = "Bonne chance !\n\nAppuyez sur entree pour commencer le jeu.\n";
        for (int x = 0; text1[x] != NULL; x++)
        {
            printf("%c", text1[x]);
            if (text1[x] == '\n')
            {
                sleep(1);
            }
            else
            {
                usleep(1000);
            }
        }
        printf(" %srouges%s ", ROUGE, END);
        sleep(1);
        for (int x = 0; text2[x] != NULL; x++)
        {
            printf("%c", text2[x]);
            if (text2[x] == '\n')
            {
                sleep(1);
            }
            else
            {
                usleep(1000);
            }
        }
        printf("%sjaunes%s. \n", JAUNE, END);
        sleep(1);
        for (int x = 0; text3[x] != NULL; x++)
        {
            printf("%c", text3[x]);
            if (text3[x] == '\n')
            {
                sleep(1);
            }
            else
            {
                usleep(1000);
            }
        }
    }
    getchar();
    printf("\n");
    system("cls");
    printf("Lancons le jeu !\n");
    sleep(1);
    printf("\n");
    printf("\n");
    printf("Creation du tableau de jeu.");
    sleep(1);
    launchgame();
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
        // --------------------------------- Version avec les coordonnées ---------------------------------
        // printf("\t\t%s| %s", BLEU, END);
        // for (int j = 0; j < COLONNES; j++)
        // {

        //     if (tab[i][j] == 1)
        //     {
        //         printf("| %s[%d][%d]%s ", ROUGE, i, j, END);
        //     }
        //     else if (tab[i][j] == 2)
        //     {
        //         printf("| %s[%d][%d]%s ", JAUNE, i, j, END);
        //     }
        //     else if (tab[i][j] == 3)
        //     {
        //         printf("| %s[%d][%d]%s ", ROUGEW, i, j, END);
        //     }
        //     else if (tab[i][j] == 4)
        //     {
        //         printf("| %s[%d][%d]%s ", JAUNEW, i, j, END);
        //     }
        //     else
        //     {
        //         printf("| %s[%d][%d]%s ", NOIR, i, j, END);
        //     }
        // }
        // --------------------------------- Version avec les coordonnées ---------------------------------
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

void launchgame()
{
    createtab();
    system("cls");
    printtab();
    while (1)
    {
        turns();
    }
}
void turns()
{
    int colonne = -1;
    while (colonne < 0)
    {
        printf("Joueur %d, a vous de jouer !\n", *pjactuel);
        printf("Dans quelle colonne voulez-vous jouer ? (Entrez 0 pour quitter la partie.)\n");
        // if(scanf("%d", &colonne) != 1) {
        //     while(getchar() != '\n');
        // }
        colonne = getcolonne();
        printf("colonne = %d\n", colonne);
    }
    printf("\n");
    system("cls");
    verifyadd(colonne, &jactuel);
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
    else if (colonne < 1 || colonne > 7)
    {
        printf("%s==========Erreur !==========\n", ROUGE);
        printf("==>Vous devez entrer un numero de colonne entre 1 et 7.\n");
        printf("==>Veuillez reessayer.\n");
        printf("============================%s\n", END);
        printf("\n");
        printtab();
        return turns();
    }
    else
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
            else if (tab[0][colonne - 1] != VIDE)
            {
                printf("%s==========Erreur !==========\n", ROUGE);
                printf("==>Cette colonne est pleine.\n");
                printf("==>Veuillez reessayer.\n");
                printf("============================%s\n", END);
                printf("\n");
                printtab();
                return turns();
            }
        }
        verifywin(colonne, joueur);
        printtab();
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
        turns();
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
    for (int j = 0; j < sizeof(SerialBuffer); j++)
    { // Clear SerialBuffer
        SerialBuffer[j] = 0;
    }
    DWORD bytes_written, total_bytes_written = 0;
    char *final_bytes = "255";
    if (!WriteFile(hComm, final_bytes, 4, &bytes_written, NULL))
    {
        fprintf(stderr, "Error\n");
        CloseHandle(hComm);
        exit(1);
    }
    CloseHandle(hComm);
    printf("A bientot !\n");

    exit(0);
}

int getcolonne()
{
    int j = 0;
    int i = 0;
    for (j = 0; j < sizeof(SerialBuffer); j++)
    { // Clear SerialBuffer
        SerialBuffer[j] = 0;
    }
    printf("\n\nEn attente de la reception de donnee du joueur %d...\n", *pjactuel);
    DWORD bytes_written, total_bytes_written = 0;
    (jactuel == 1) ? (strcpy(bytes_to_send, one)) : (strcpy(bytes_to_send, two));
    strcat(bytes_to_send, "\0");
    if (!WriteFile(hComm, bytes_to_send, 5, &bytes_written, NULL))
    {
        fprintf(stderr, "Error\n");
        CloseHandle(hComm);
        return 1;
    }
    // printf("Charactrer sent\n"); // Debug
    j = 0;
    for (j = 0; j < sizeof(SerialBuffer); j++)
    { // Clear SerialBuffer
        SerialBuffer[j] = 0;
    }
    Status = WaitCommEvent(hComm, &dwEventMask, NULL); // Wait for the character to be received

    /*-------------------------- Program will Wait here till a Character is received ------------------------*/
    int io = 0;
    if (Status == FALSE)
    {
        printf("\n    Error! in Setting WaitCommEvent()");
    }
    else // If  WaitCommEvent()==True Read the RXed data using ReadFile();
    {
        do
        {
            Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
            if (Status == TRUE)
            {
                if (NoBytesRead > 0)
                {
                    SerialBuffer[io] = TempChar;
                }
                io++;
            }
            else
            {
                printf("\n    Error! in Setting ReadFile()");
            }
        } while (NoBytesRead);
        for (j = 0; j < io; j++)
        {
            if (SerialBuffer[j] >= '0' && SerialBuffer[j] <= '9')
            {
                printf("%c", SerialBuffer[j]);
                break;
            }
        }
        return SerialBuffer[j] - '0';
    }
}