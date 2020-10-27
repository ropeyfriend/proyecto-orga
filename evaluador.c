#include <stdio.h>
#include <stdlib.h>

#include "mapeo.h"

int main() {

    char archivo[] = "D:\\OLC\\proyecto-orga\\headerlista.txt";

    FILE *F = fopen(archivo, "r+");

    char line[1000];
    int i = 1;

    while (!feof(F)) {
        fgets(line, sizeof(line)/sizeof(line[0]), F);
        printf("%s",line);
        i++;
    }

    fclose(F);
}