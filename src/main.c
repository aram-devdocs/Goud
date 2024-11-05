#include "goud.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <script.gg>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *source = malloc(fileSize + 1);
    fread(source, 1, fileSize, file);
    source[fileSize] = '\0';
    fclose(file);

    Token *tokens = tokenize(source);
    parse(tokens);

    freeTokens(tokens);
    free(source);
    return 0;
}