#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdbool.h>
#include <time.h>

#define PORT 1234
#define MAXLEN 512
#define ROWS 6
#define COLS 7

// Prototypes des fonctions
void initializeSDLNet();
void quitSDLNet();
TCPsocket createServer();
TCPsocket createClient(const char* ip);
void handleGame(TCPsocket socket, int myPlayerId);
void getInput(char* input, int size, const char* prompt);
void initializeBoard(char board[ROWS][COLS]);
void printBoard(char board[ROWS][COLS]);
bool makeMove(char board[ROWS][COLS], int col, char player);
bool checkWinner(char board[ROWS][COLS], char player);
int chooseStartingPlayer();
bool isBoardFull(char board[ROWS][COLS]);
void sendMove(TCPsocket socket, int col);
int receiveMove(TCPsocket socket);

// Initialisation de SDLNet
void initializeSDLNet() {
    if (SDLNet_Init() < 0) {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

// Nettoyage de SDLNet
void quitSDLNet() {
    SDLNet_Quit();
}

// Création d'un serveur TCP
TCPsocket createServer() {
    IPaddress ip;
    TCPsocket server;

    if (SDLNet_ResolveHost(&ip, NULL, PORT) < 0) {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    server = SDLNet_TCP_Open(&ip);
    if (!server) {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    printf("En attente d'une connexion...\n");
    TCPsocket client = NULL;
    while (!client) {
        client = SDLNet_TCP_Accept(server);
        if (client) {
            printf("Connexion établie.\n");
            break;
        }

        SDL_Delay(100); // Attente de 100 ms
    }

    SDLNet_TCP_Close(server); // Fermer le serveur après la connexion du client
    return client;
}

// Connexion à un serveur TCP
TCPsocket createClient(const char* ip) {
    IPaddress ipAddr;
    TCPsocket client;

    if (SDLNet_ResolveHost(&ipAddr, ip, PORT) < 0) {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    client = SDLNet_TCP_Open(&ipAddr);
    if (!client) {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    return client;
}

// Gestion du jeu
void handleGame(TCPsocket socket, int myPlayerId) {
    char board[ROWS][COLS];
    initializeBoard(board);
    printBoard(board);

    int currentPlayer = chooseStartingPlayer();
    char playerSymbols[2] = {'X', 'O'};
    bool gameRunning = true;
    int col;

    while (gameRunning) {
        if (currentPlayer == myPlayerId) {
            printf("Votre tour (Joueur %c). Entrez le numéro de la colonne (0-%d): ", playerSymbols[currentPlayer], COLS - 1);
            if (scanf("%d", &col) != 1) {
                printf("Entrée invalide. Veuillez entrer un nombre.\n");
                while (getchar() != '\n'); // Nettoyer le buffer d'entrée
                continue;
            }

            if (makeMove(board, col, playerSymbols[currentPlayer])) {
                sendMove(socket, col);
                printBoard(board);
                if (checkWinner(board, playerSymbols[currentPlayer])) {
                    printf("Vous avez gagné !\n");
                    gameRunning = false;
                } else if (isBoardFull(board)) {
                    printf("Égalité !\n");
                    gameRunning = false;
                }
            } else {
                printf("Mouvement invalide, essayez à nouveau.\n");
                while (getchar() != '\n'); // Nettoyer le buffer d'entrée
                continue;
            }
        } else {
            // Recevez le coup de l'autre joueur
            printf("Attente du coup de l'autre joueur...\n");
            col = receiveMove(socket);
            if (makeMove(board, col, playerSymbols[currentPlayer])) {
                printBoard(board);
                if (checkWinner(board, playerSymbols[currentPlayer])) {
                    printf("L'autre joueur a gagné !\n");
                    gameRunning = false;
                } else if (isBoardFull(board)) {
                    printf("Égalité !\n");
                    gameRunning = false;
                }
            }
        }
        currentPlayer = (currentPlayer + 1) % 2; // Changer de joueur
    }
}

// Initialiser le plateau de jeu
void initializeBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '.';
        }
    }
}

// Imprimer le plateau de jeu
void printBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Effectuer un mouvement
bool makeMove(char board[ROWS][COLS], int col, char player) {
    if (col < 0 || col >= COLS) {
        return false; // Colonne invalide
    }

    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == '.') {
            board[i][col] = player;
            return true;
        }
    }

    return false; // Colonne pleine
}

// Vérifier si un joueur a gagné
bool checkWinner(char board[ROWS][COLS], char player) {
    // Vérification horizontale
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS - 3; col++) {
            if (board[row][col] == player && board[row][col + 1] == player &&
                board[row][col + 2] == player && board[row][col + 3] == player) {
                return true;
            }
        }
    }

    // Vérification verticale
    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row < ROWS - 3; row++) {
            if (board[row][col] == player && board[row + 1][col] == player &&
                board[row + 2][col] == player && board[row + 3][col] == player) {
                return true;
            }
        }
    }

    // Vérification diagonale (montante et descendante)
    for (int row = 0; row < ROWS - 3; row++) {
        for (int col = 0; col < COLS - 3; col++) {
            if (board[row][col] == player && board[row + 1][col + 1] == player &&
                board[row + 2][col + 2] == player && board[row + 3][col + 3] == player) {
                return true;
            }
            if (board[row + 3][col] == player && board[row + 2][col + 1] == player &&
                board[row + 1][col + 2] == player && board[row][col + 3] == player) {
                return true;
            }
        }
    }

    return false;
}

// Choisir le joueur de départ
int chooseStartingPlayer() {
    srand(time(NULL));
    return rand() % 2; // Retourne 0 ou 1 aléatoirement
}

// Vérifier si le plateau est plein
bool isBoardFull(char board[ROWS][COLS]) {
    for (int col = 0; col < COLS; col++) {
        if (board[0][col] == '.') {
            return false;
        }
    }
    return true;
}

// Envoyer un mouvement
void sendMove(TCPsocket socket, int col) {
    int32_t netCol = SDLNet_Read32(&col);
    SDLNet_TCP_Send(socket, &netCol, sizeof(netCol));
}

// Recevoir un mouvement
int receiveMove(TCPsocket socket) {
    int32_t netCol;
    SDLNet_TCP_Recv(socket, &netCol, sizeof(netCol));
    return SDLNet_Read32(&netCol);
}

// Obtenir une entrée de l'utilisateur
void getInput(char* input, int size, const char* prompt) {
    printf("%s", prompt);
    fgets(input, size, stdin);
    input[strcspn(input, "\n")] = 0; // Enlever le saut de ligne
}

int main(int argc, char **argv) {
    char playerName[256];
    char ip[256];
    TCPsocket socket;
    int myPlayerId;

    // Initialisation de SDLNet
    initializeSDLNet();

    // Obtention du nom du joueur
    getInput(playerName, sizeof(playerName), "Entrez votre pseudo: ");

    // Demander si l'utilisateur veut être serveur ou client
    getInput(ip, sizeof(ip), "Entrez l'IP du serveur ou 's' pour être le serveur: ");

    if (strcmp(ip, "s") == 0) {
        socket = createServer();
        myPlayerId = 0;
    } else {
        socket = createClient(ip);
        myPlayerId = 1;
    }

    // Gérer le jeu
    handleGame(socket, myPlayerId);

    // Nettoyage
    SDLNet_TCP_Close(socket);
    quitSDLNet();

    return 0;
}
