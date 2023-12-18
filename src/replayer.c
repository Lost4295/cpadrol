#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>


bool file_exists(const char *filename)
{
    return access(filename, F_OK) == 0;
}
int main()
{

    FILE *file;
    char filename[250];
    chdir("replays");
    do
    {
        printf("Enter the name of the file you want to see.\n");
        scanf("%s", filename);
        strcat(filename, ".txt");
        if (!file_exists(filename))
        {
            printf("File %s does not exist.\n", filename);
        }
    } while (!file_exists(filename));
    char line[250];
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 1;
    }
    printf("File opened successfully.\n");
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }
    fclose(file);
    return 0;
}
