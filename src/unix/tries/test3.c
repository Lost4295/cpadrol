#include <stdio.h> 
#include <stdlib.h> // For exit() 
#include <string.h>

int main() 
{ 
	FILE *fptr1, *fptr2; 
	char filename[100], c; 

	printf("Enter the filename to open for reading \n"); 
	scanf("%s", filename); 
    char* command="cp";
    char rc[100];
    strcpy(rc,command);
    strcat(rc, " ");
    strcat(rc, filename);
    strcat(rc, " ");
    strcat(rc, "dest.jpeg");
    system(rc);
	return 0; 
}
