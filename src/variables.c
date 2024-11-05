#include "goud.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char *name;
    int value;
} Variable;

Variable symbolTable[100]; // Define the symbol table here
int symbolCount = 0;       // Define the symbol count here

// Retrieve a variable's value
int getVariableValue(const char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return symbolTable[i].value;
        }
    }
    fprintf(stderr, "Error: Undefined variable '%s'\n", name);
    exit(1);
}

// Set or update a variable's value
void setVariableValue(const char *name, int value) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            symbolTable[i].value = value;
            return;
        }
    }
    symbolTable[symbolCount++] = (Variable){strdup(name), value};
}