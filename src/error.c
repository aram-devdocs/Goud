#include "goud.h"
#include <stdlib.h>

void reportError(const char *file, int line, const char *message) {
    fprintf(stderr, "Error in %s at line %d: %s\n", file, line, message);
    exit(1);
}