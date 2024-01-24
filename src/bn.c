#include <stdio.h>
#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif
#define WIDTH 25
#define HEIGHT 8

int startx = 0;
int starty = 0;

char *choices[] = {
    "Multiplayer(Client)",
    "Multiplayer(Server)",
    "Replays",
    "Exit",
};



int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);

int main()
{
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;

    initscr();
    clear();
    noecho();
    cbreak(); /* Line buffering disabled. pass on everything */
    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;
    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, TRUE);
    mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
    refresh();
    print_menu(menu_win, highlight);
    while (1)
    {
        c = wgetch(menu_win);
        switch (c)
        {
        case KEY_UP:
            if (highlight == 1)
                highlight = n_choices;
            else
                --highlight;
            break;
        case KEY_DOWN:
            if (highlight == n_choices)
                highlight = 1;
            else
                ++highlight;
            break;
        case 10:
            choice = highlight;
            break;
        default:
            mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
            refresh();
            break;
        }
        print_menu(menu_win, highlight);
        if (choice != 0) /* User did a choice come out of the infinite loop */
            break;
    }
    if (choice == 4)
    {
        goto end;
    }
    else
    {
        mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
        sleep(2);
    }
    clrtoeol();
    refresh();
    switch (choice)
    {
    case 1:
        system("./a.out||./a.exe");
        break;
    case 2:
        /* TODO : get l'ip puis render*/
        break;
    case 3:
        /* Fonction de replay */

        break;
    case 4:
        goto end;
        break;
    default:
        break;
    }

end:
    endwin();
    return 0;
}

void print_menu(WINDOW *menu_win, int highlight)
{
    int x, y, i;

    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for (i = 0; i < n_choices; ++i)
    {
        if (highlight == i + 1) /* High light the present choice */
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(menu_win);
}
