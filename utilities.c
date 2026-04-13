#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "sqlite3.h"

void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;
    }
}

void To_Upper(char *buffer) {
    for(int i = 0; i < (int)strlen(buffer); i++){
        buffer[i] = toupper((unsigned char)buffer[i]);
    }
}