#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

bool file_exists(const char *filename)
{
    return access(filename, F_OK) == 0;
}
int main()
{
    int i = 1;
    clrscr();
    clrscr();
    printf("Hello, World!\n");
    sleep(1);
    clrscr();
    printf("Creating replays folder.\n");
    int check;
    char *dirname = "replays";
    check = mkdir(dirname);
    if (!check)
    {
        printf("Directory created.\n");
    }
    else
    {
        printf("Unable to create directory. Maybe it is already there ?\n");
        DIR *dir = opendir(dirname);
        if (dir)
        {
            printf("Directory already exists.\n");
            closedir(dir);
        }
        else if (ENOENT == errno)
        {
            printf("Directory doesn't exist.\n");
        }
        else
        {
            printf("Error while checking if directory exists.\n");
        }
    }
    sleep(1);
    printf("Press enter to continue.\n");
    getch();
    clrscr();
    printf("Showing contents of directory.\n");
    sleep(1);
    chdir(dirname);
    system("dir");
    printf("\nPress enter to continue.\n");
    getch();
    clrscr();

    printf("Creating replay file.\n");
    sleep(1);
    getch();
    FILE *fptr;

    // Create a file

    // Close the file
    char filename[250];
    do
    {
        printf("Enter the name of the file you want to create.\n");
        scanf("%s", filename);
        strcat(filename, ".txt");
        if (!file_exists(filename))
        {
            fptr = fopen(filename, "w");
            fclose(fptr);
            printf("File %s created.\n", filename);
        }
        else
        {
            printf("File %s already exists.\n", filename);
        }
    } while (!file_exists(filename));

    printf("Press enter to continue.\n");
    getch();
    clrscr();
    exit(0);
}

void clrscr()
{
    system("@cls||clear");
}
