#include <stdio.h>
#include <string.h>
#include <ctype.h>


void read_line(char *buffer, int size)
{
    if (fgets(buffer, size, stdin)) {
        // Remove newline if present
        buffer[strcspn(buffer, "\n")] = 0;
    }
}

//Fonction that returns a buffer all uppercase ex: bakr --> Tou(bakr) = BAKR;
void To_Upper(char *buffer){
    for(int i = 0; i<strlen(buffer); i++){
    buffer[i] = toupper((unsigned char)buffer[i]);
   }
}
