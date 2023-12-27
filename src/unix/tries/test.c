
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char **options[4];
    char *play = "Play";
    char *multiplayer = "Multiplayer";
    char *settings = "Options";
    char *quit = "Quit";
    options[0] = &play;
    options[1] = &multiplayer;
    options[2] = &settings;
    options[3] = &quit;
int select = 0;
char * choose;
int exit = 0;
    while (!exit)
    {
        for (int i = 0; i < 4; i++)
        {
            if (select)
            {
                printf("->");
            }
            printf("%s\n", *options[i]);
        }
        scanf("%c", choose);
        switch (*choose)
        {
        case 'u':
            (select==3)?select=0:select++;
            break;
        case 'd':
            (select==0)?select=3:select--;
        break;
        case 'e':
            exit=1; 
            break;
        default:
            break;
        }
    }
    return 0;
}