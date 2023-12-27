
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int main(void)
{
    initscr();
    cbreak();
    noecho();
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
    char *choose;
    int exit = 0;
    printf("hi!");
    while (!exit)
    {
        for (int i = 0; i < 4; i++)
        {
            if (select == i)
            {
                printw("->");
            }
            else
            {
                printw("  ");
            }
            printw(" %s\n", *options[i]);
        }
        if (getch() == '\033')
        {            // if the first value is esc
            getch(); // skip the [
            switch (getch())
            { // the real value
            case 'A':
                // code for arrow up
                (select == 0) ? select = 3 : select--;
                clear();
                break;
            case 'B':
                // code for arrow down
                (select == 3) ? select = 0 : select++;
                clear();
                break;
            case 'C':
                printw("Option selected : %s\n\n", *options[select]);
                break;
            case 'D':
                // code for arrow left
                endwin();
                exit = 1;
                break;
            default:
                break;
            }
        }
    }
    endwin();
    return 0;
}