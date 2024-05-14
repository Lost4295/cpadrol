
    #include <winsock2.h>
    typedef int socklen_t;

#include <stdio.h>
#include <stdlib.h>
#define PORT 23
 
 
 
int main(void)
{
        WSADATA WSAData;
        int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);

    SOCKET sock;
    SOCKADDR_IN sin;
    char buffer[32] = "";
    char buffer2[32] = "Hey !\n";
 
    /* Si les sockets Windows fonctionnent */
    if(!erreur)
    {
        /* Création de la socket */
        sock = socket(AF_INET, SOCK_STREAM, 0);
 
        /* Configuration de la connexion */
        sin.sin_addr.s_addr = inet_addr("127.0.0.1");
        sin.sin_family = AF_INET;
        sin.sin_port = htons(PORT);
 
        /* Si l'on a réussi à se connecter */
        if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR)
        {
            printf("Connection à %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
            
            /* Si l'on reçoit des informations : on les affiche à l'écran */
            if(recv(sock, buffer, 32, 0) != SOCKET_ERROR)
                printf("Recu : %s\n", buffer);
            if (send(sock, buffer2, 32, 0) != SOCKET_ERROR)
                printf("Envoyé : %s\n", buffer2);
        }
        /* sinon, on affiche "Impossible de se connecter" */
        else
        {
            printf("Impossible de se connecter\n");
        }
 
        /* On ferme la socket */
        closesocket(sock);
 
        #if defined (WIN32)
            WSACleanup();
        #endif
    }
 
    /* On attend que l'utilisateur tape sur une touche, puis on ferme */
    getchar();
 
    return EXIT_SUCCESS;
}
