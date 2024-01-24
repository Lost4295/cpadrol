#include <stdio.h>  // Including the standard input-output header file for functions like printf.
#include <stdlib.h> // Including the standard library header for functions like malloc and exit.
#include <string.h> // Including the string header for string manipulation functions.

/**
 * Allows the user to edit the contents of a char array.
 *
 * @param array: A pointer to the char array.
 * @param size: The size of the char array.
 */
void editCharArray(char *array, int size)
{
    printf("Enter text (maximum %d characters):\n", size - 1);
    fgets(array, size, stdin); // Read user input and store it in the char array.

    // Remove the newline character from the end of the input.
    int length = strlen(array);
    if (array[length - 1] == '\n')
    {
        array[length - 1] = '\0';
    }

    printf("Editing mode: Enter 'q' to quit.\n");

    char input[10];
    while (1)
    {
        printf("Enter command: ");
        fgets(input, sizeof(input), stdin); // Read user command.

        if (input[0] == 'q')
        {
            break; // Quit editing mode if 'q' is entered.
        }
        else if (input[0] == 'r')
        {
            // Replace a substring in the char array.
            char oldSubstr[100];
            char newSubstr[100];
            printf("Enter substring to replace: ");
            fgets(oldSubstr, sizeof(oldSubstr), stdin);
            printf("Enter new substring: ");
            fgets(newSubstr, sizeof(newSubstr), stdin);

            // Remove the newline character from the end of the substrings.
            int oldSubstrLength = strlen(oldSubstr);
            if (oldSubstr[oldSubstrLength - 1] == '\n')
            {
                oldSubstr[oldSubstrLength - 1] = '\0';
            }
            int newSubstrLength = strlen(newSubstr);
            if (newSubstr[newSubstrLength - 1] == '\n')
            {
                newSubstr[newSubstrLength - 1] = '\0';
            }

            // Replace the old substring with the new substring.
            char *position = strstr(array, oldSubstr);
            if (position != NULL)
            {
                int oldSubstrIndex = position - array;
                int oldSubstrSize = strlen(oldSubstr);
                int newSubstrSize = strlen(newSubstr);
                int newSize = strlen(array) - oldSubstrSize + newSubstrSize + 1;

                // Allocate memory for the new array.
                char *newArray = (char *)malloc(newSize * sizeof(char));
                if (newArray == NULL)
                {
                    printf("Memory allocation failed.\n");
                    exit(EXIT_FAILURE);
                }

                // Copy the contents of the old array to the new array.
                strncpy(newArray, array, oldSubstrIndex);
                newArray[oldSubstrIndex] = '\0';

                // Concatenate the new substring to the new array.
                strcat(newArray, newSubstr);

                // Concatenate the remaining part of the old array to the new array.
                strcat(newArray, position + oldSubstrSize);

                // Free the memory of the old array.
                free(array);

                // Update the pointer to the new array.
                array = newArray;
            }
            else
            {
                printf("Substring not found.\n");
            }
        }
        else if (input[0] = 'p')
        {
            printf("String : %s\n", array);
        }
        else
        {
            printf("Invalid command.\n");
        }
    }
}

int main()
{
    int size = 100;                                    // Size of the char array.
    char *array = (char *)malloc(size * sizeof(char)); // Allocate memory for the char array.
    if (array == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    editCharArray(array, size); // Call the function to edit the char array.

    printf("Edited text: %s\n", array);

    free(array); // Free the memory of the char array.

    return 0;
}