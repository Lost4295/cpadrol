#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#define PORT 42069
#define BUF_SIZE 1024
#define ID_SIZE 20
#define IP_SIZE 16

// Structure pour stocker les informations d'un utilisateur
typedef struct User {
    char name[100];
    char id[ID_SIZE];
    char ip[IP_SIZE];
    struct User *next;
} User;

// Prototypes des fonctions
void handleError(const char *message);
void generateUniqueId(char *id);
void updateUser(User **head, char *name, char *id, char *ip);
void displayUsers(const User *head, int *userCount);
void clearUserList(User **head);
int showMenu();
void searchUsers(SOCKET sendSock, SOCKET recvSock, char *userName, char *uniqueId, User **userList, struct sockaddr_in *broadcastAddr);

int main() {
    WSADATA wsaData;
    char userName[100];
    char uniqueId[ID_SIZE];
    User *userList = NULL;

    // Initialisation de Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        handleError("WSAStartup failed");
    }

    generateUniqueId(uniqueId);

    printf("Entrez votre nom: ");
    fgets(userName, sizeof(userName), stdin);
    userName[strcspn(userName, "\n")] = 0;

    // Création des sockets pour l'envoi et la réception
    SOCKET sendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sendSock == INVALID_SOCKET) handleError("Send socket creation failed");

    SOCKET recvSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (recvSock == INVALID_SOCKET) handleError("Receive socket creation failed");

    // Configuration pour permettre le broadcast
    char broadcastPermission = 1;
    if (setsockopt(sendSock, SOL_SOCKET, SO_BROADCAST, &broadcastPermission, sizeof(broadcastPermission)) == SOCKET_ERROR) {
        handleError("Broadcast option failed");
    }

    // Configuration de l'adresse de broadcast
    struct sockaddr_in broadcastAddr;
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(PORT);
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    // Configuration du socket de réception
    struct sockaddr_in recvAddr;
    memset(&recvAddr, 0, sizeof(recvAddr));
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(PORT);
    recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(recvSock, (struct sockaddr *)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR) {
        handleError("Bind failed on receive socket");
    }

    int choice;
    do {
        choice = showMenu();
        switch (choice) {
            case 1:
                searchUsers(sendSock, recvSock, userName, uniqueId, &userList, &broadcastAddr);
                break;
            case 2:
                // Quitter le programme
                break;
        }
    } while (choice != 2);

    // Nettoyage final
    closesocket(sendSock);
    closesocket(recvSock);
    WSACleanup();
    clearUserList(&userList);
    return EXIT_SUCCESS;
}

// Fonction pour gérer les erreurs
void handleError(const char *message) {
    printf("%s: %d\n", message, WSAGetLastError());
    exit(1);
}

// Fonction pour générer un identifiant unique
void generateUniqueId(char *id) {
    srand(time(NULL)); // Initialisation du générateur aléatoire
    sprintf(id, "%d-%d", time(NULL), rand()); // Création de l'ID unique
}

// Fonction pour mettre à jour la liste des utilisateurs
void updateUser(User **head, char *name, char *id, char *ip) {
    User *newUser = (User *)malloc(sizeof(User));
    strcpy(newUser->name, name);
    strcpy(newUser->id, id);
    strcpy(newUser->ip, ip);
    newUser->next = *head;
    *head = newUser;
}

// Fonction pour afficher la liste des utilisateurs
void displayUsers(const User *head, int *userCount) {
    system("cls"); // Nettoyage de la console
    int count = 1;
    const User *current = head;
    while (current != NULL) {
        printf("%d. %s [%s]\n", count++, current->name, current->ip);
        current = current->next;
    }
    *userCount = count;
}

// Fonction pour vider la liste des utilisateurs
void clearUserList(User **head) {
    User *current = *head;
    while (current != NULL) {
        User *next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

// Fonction pour afficher le menu principal
int showMenu() {
    int choice;
    printf("\nMenu:\n");
    printf("1. Chercher des joueurs\n");
    printf("2. Fermer le programme\n");
    printf("Entrez votre choix: ");
    scanf("%d", &choice);
    return choice;
}

// Fonction pour rechercher des utilisateurs
void searchUsers(SOCKET sendSock, SOCKET recvSock, char *userName, char *uniqueId, User **userList, struct sockaddr_in *broadcastAddr) {
    int userChoice, userCount;

    do {
        clearUserList(userList); // Réinitialisation de la liste des utilisateurs

        char sendBuffer[BUF_SIZE];
        sprintf(sendBuffer, "%s:%s", userName, uniqueId);

        if (sendto(sendSock, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr *)broadcastAddr, sizeof(*broadcastAddr)) == SOCKET_ERROR) {
            handleError("Broadcast send failed");
        }

        char buffer[BUF_SIZE];
        struct sockaddr_in fromAddr;
        int fromAddrSize = sizeof(fromAddr);
        int receivedBytes = recvfrom(recvSock, buffer, BUF_SIZE, 0, (struct sockaddr *)&fromAddr, &fromAddrSize);

        if (receivedBytes > SOCKET_ERROR) {
            buffer[receivedBytes] = '\0';
            char *receivedId = strrchr(buffer, ':');
            if (receivedId && strcmp(receivedId + 1, uniqueId) != 0) {
                *receivedId = '\0';
                char ipAddress[IP_SIZE];
                strcpy(ipAddress, inet_ntoa(fromAddr.sin_addr));
                updateUser(userList, buffer, receivedId + 1, ipAddress);
            }
        }

        displayUsers(*userList, &userCount);
        printf("%d. Rechercher a nouveau\n", userCount);
        printf("%d. Retour au menu principal\n", userCount + 1);
        printf("Entrez votre choix: ");
        scanf("%d", &userChoice);

        if (userChoice == userCount) { // Rechercher à nouveau
            continue;
        } else if (userChoice == userCount + 1) { // Retour au menu principal
            break;
        }
    } while (1);
}
