#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#define IP_BUFFER_LEN 100
#define BUFFER_SIZE 1000
#define ID_SIZE 20
#define IP_SIZE 16
typedef struct User
{
    char name[100];
    char id[ID_SIZE];
    char ip[IP_SIZE];
    struct User *next;
} User;

IPaddress bc_recv_ip = {INADDR_ANY, (Uint16)9999};
UDPsocket bc_recv = NULL;
UDPpacket *packetr = NULL;
IPaddress bc_send_ip = {INADDR_BROADCAST, SDL_SwapBE16((Uint16)9999)};
UDPsocket bc_send = NULL;
UDPpacket *packets = NULL;
void sendPseudo()
{
    strcpy(packets->data, "toto");
    packets->len = strlen("toto") + 1;
    fprintf(stderr, "Sending packet of %d bytes\n", packets->len);
    if (SDLNet_UDP_Send(bc_send, 0, packets) < 0)
    {
        fprintf(stderr, "Couldn’t send packet!\n");
        SDLNet_UDP_Close(bc_send);
        SDLNet_FreePacket(packets);
        return (1);
    }
}

void recievePseudo()
{
    int tick = SDL_GetTicks64();
    int recieved = 0;

    fprintf(stderr, "Waiting for packet.\n");

    while (((SDL_GetTicks64() - tick) < 10000) && (!recieved))
    {
        if (SDLNet_UDP_Recv(bc_recv, packetr) > 0)
            recieved = 1;
        else
            SDL_Delay(100);
    }

    if (recieved)
        fprintf(stderr, "UDP speaketh:  %s\n", packetr->data);
    else
        fprintf(stderr, "Timeout;  no packet.\n");
}

// int showMenu();
// TODO Ajouter ça

//    generateUniqueId(uniqueId);

//// Fonction pour générer un identifiant unique
void generateUniqueId(char *id)
{
    srand(time(NULL));                        // Initialisation du générateur aléatoire
    sprintf(id, "%d-%d", time(NULL), rand()); // Création de l'ID unique
}
//
// Fonction pour mettre à jour la liste des utilisateurs
void updateUser(User **head, char *name, char *id, char *ip)
{
    User *newUser = (User *)malloc(sizeof(User));
    strcpy(newUser->name, name);
    strcpy(newUser->id, id);
    strcpy(newUser->ip, ip);
    newUser->next = *head;
    *head = newUser;
}
//
// Fonction pour afficher la liste des utilisateurs
void displayUsers(const User *head, int *userCount)
{
    system("cls"); // Nettoyage de la console
    int count = 1;
    const User *current = head;
    while (current != NULL)
    {
        printf("%d. %s [%s]\n", count++, current->name, current->ip);
        current = current->next;
    }
    *userCount = count;
}

// Fonction pour vider la liste des utilisateurs
void clearUserList(User **head)
{
    User *current = *head;
    while (current != NULL)
    {
        User *next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

// Fonction pour afficher le menu principal
// int showMenu() {
//    int choice;
//    printf("\nMenu:\n");
//    printf("1. Chercher des joueurs\n");
//    printf("2. Fermer le programme\n");
//    printf("Entrez votre choix: ");
//    scanf("%d", &choice);
//    return choice;
//}
//

// Fonction pour rechercher des utilisateurs
void searchUsers(char *userName, char *uniqueId, User **userList, struct sockaddr_in *broadcastAddr)
{
    packetr = createUDPRconn();

    int userChoice, userCount;

    do
    {
        clearUserList(userList); // Réinitialisation de la liste des utilisateurs

        char sendBuffer[BUFFER_SIZE];
        sprintf(sendBuffer, "%s:%s", userName, uniqueId);
        packets = createUDPSconn();
        sendPseudo();
// à remove
        char buffer[BUFFER_SIZE];
        struct sockaddr_in fromAddr;
        int fromAddrSize = sizeof(fromAddr);
        int receivedBytes = recvfrom(recvSock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&fromAddr, &fromAddrSize);
        recievePseudo();
        if (receivedBytes > SOCKET_ERROR)
        {
            strcpy(buffer, packetr->data);
            buffer[receivedBytes] = '\0';
            char *receivedId = strrchr(buffer, ':');
            if (receivedId && strcmp(receivedId + 1, uniqueId) != 0)
            {
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

        if (userChoice == userCount)
        { // Rechercher à nouveau
            continue;
        }
        else if (userChoice == userCount + 1)
        { // Retour au menu principal
            break;
        }
    } while (1);
}

UDPpacket *createUDPRconn()
{
    bc_recv = SDLNet_UDP_Open(9999);
    if (bc_recv == NULL)
    {
        fprintf(stderr, "Couldn’t open bc_recv\n");
        SDLNet_UDP_Close(bc_recv);
        exit(1);
    }

    if (SDLNet_UDP_Bind(bc_recv, -1, &bc_recv_ip) < 0)
    {
        fprintf(stderr, "Couldn’t bind broadcast recieving address\n");
        SDLNet_UDP_Close(bc_recv);
        exit(1);
    }

    fprintf(stderr, "Recieving port opened!\n");

    packetr = SDLNet_AllocPacket(1024);
    if (packetr == NULL)
    {
        fprintf(stderr, "Couldn’t allocate packet!\n");
        SDLNet_UDP_Close(bc_recv);
        exit(1);
    }
    return packetr;
}

void cleanUDPrConn()
{
    SDLNet_UDP_Close(bc_recv);
    SDLNet_FreePacket(packetr);
}

UDPpacket *createUDPSconn()
{

    bc_send = SDLNet_UDP_Open(9999);
    if (bc_send == NULL)
    {
        fprintf(stderr, "Couldn’t open bc_send\n");
        return (1);
    }

    if (SDLNet_UDP_Bind(bc_send, -1, &bc_send_ip) < 0)
    {
        fprintf(stderr, "Couldn’t bind broadcast sending address\n");
        SDLNet_UDP_Close(bc_send);
        return (1);
    }

    fprintf(stderr, "Broadcast socket opened!\n");

    packets = SDLNet_AllocPacket(1024);
    if (packets == NULL)
    {
        fprintf(stderr, "Couldn’t allocate packet!\n");
        SDLNet_UDP_Close(bc_send);
        return (1);
    }
    return bc_send;
}

void cleanUDPsConn()
{

    SDLNet_UDP_Close(bc_send);
    SDLNet_FreePacket(packets);

    fprintf(stderr, "Done!\n");

    return (0);
}