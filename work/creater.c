#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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
    system("ls||dir");
    printf("\nPress enter to continue.\n");
    getch();
    clrscr();

    printf("Creating replay file.\n");
    sleep(1);
    getch();
    FILE *fptr;
    char filename[250];
    bool choosename = false;
    int choice = -1;

    do
    {
        printf("Do you want to choose the name of the file ?\n");
        printf("1. Yes\n");
        printf("2. No\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            choosename = true;
            break;
        }
        else if (choice == 2)
        {
            choosename = false;
            break;
        }
        else
        {
            printf("Invalid choice.\n");
        }
    } while (choice != 1 || choice != 2);

    if (choosename)
    {
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
    }
    else
    {
        bool createit = false;
        strcpy(filename, "replay1.txt");
        do
        {
            if (!file_exists(filename))
            {
                fptr = fopen(filename, "w");
                fclose(fptr);
                printf("File %s created.\n", filename);
            }
            else
            {
                printf("File %s already exists.\n", filename);
                createit = true;
            }
            i++;
            sprintf(filename, "replay%d.txt", i);
        } while (file_exists(filename));
        if (createit)
        {
            fptr = fopen(filename, "w");
            fclose(fptr);
            printf("File %s created.\n", filename);
        }
    }

    printf("Press enter to continue.\n");
    getch();
    clrscr();
    exit(0);
}

void clrscr()
{
    system("@cls||clear");
}
