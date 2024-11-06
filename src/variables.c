#include "goud.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    int value;
} Variable;

#define SYMBOL_TABLE_SIZE 100

Variable symbolTable[SYMBOL_TABLE_SIZE];
int symbolCount = 0;

// Retrieve a variable's value
int getVariableValue(const char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return symbolTable[i].value;
        }
    }
    reportError("runtime", -1, "Undefined variable");
    return 0; // Add this line to handle the case when a variable is not found
}

// Set or update a variable's value
void setVariableValue(const char *name, int value) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            symbolTable[i].value = value;
            return;
        }
    }
    if (symbolCount >= SYMBOL_TABLE_SIZE) {
        reportError("runtime", -1, "Symbol table overflow");
    }
    symbolTable[symbolCount].name = strdup(name);
    symbolTable[symbolCount].value = value;
    symbolCount++;
}