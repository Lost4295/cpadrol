#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>

int main(void)
{

    puts("------------Commande systeme--------------------------------------------------");
    //system("ipconfig");

    puts("\n\n------------Commande systeme et popen()---------------------------------------\n");
    //const char * commande = "ipconfig";
    //FILE * resultat = popen (commande, "r");
    // pas sous windows --> _popen()
    //int pclose (FILE *stream);

    puts("\n\n----------Fonctions reseaux--------------------------------------------------\n");
    // Voir : gethostname --> http://msdn.microsoft.com/en-us/library/windows/desktop/ms738527%28v=vs.85%29.aspx
    // gethostbyname --> http://msdn.microsoft.com/en-us/library/windows/desktop/ms738524%28v=vs.85%29.aspx
    // struct hostent --> http://msdn.microsoft.com/en-us/library/windows/desktop/ms738552%28v=vs.85%29.aspx

    struct in_addr addr;
    struct hostent *localhost;
    int ret = -1;
    char localname[1000] = {0};

    // Demarrer les services de reseau
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Recuperer le nom du PC local
    ret = gethostname( &(localname[0]), 999);

    // Recuperer une structure decrivant un hote a partir de son nom
    localhost = gethostbyname( &(localname[0]) );

    // Extraire l'adresse (on suppose qu'elle est de type IPv4)
    addr = * ( (struct in_addr*)localhost->h_addr_list[0] );

    // Affichage
    printf("Host : %s (%d)\n", localname, ret);
    printf("IP = %s\n", inet_ntoa( addr ) );

    // Extraire les champs de l'adresse
    u_char b1 = addr.S_un.S_un_b.s_b1;
    u_char b2 = addr.S_un.S_un_b.s_b2;
    u_char b3 = addr.S_un.S_un_b.s_b3;
    u_char b4 = addr.S_un.S_un_b.s_b4;
    printf("4 parties de l'adresse : %u %u %u %u\n", b1, b2, b3, b4);

    return 0;

}