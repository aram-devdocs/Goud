#include "goud.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_COUNT 100

Token createToken(TokenType type, const char *lexeme, int line) {
    Token token;
    token.type = type;
    token.lexeme = strdup(lexeme);
    token.line = line;
    return token;
}

Token *tokenize(const char *source) {
    Token *tokens = malloc(sizeof(Token) * MAX_TOKEN_COUNT);
    int tokenIndex = 0, line = 1;

    for (int i = 0; source[i] != '\0'; i++) {
        char c = source[i];
        if (isspace(c)) { if (c == '\n') line++; continue; }

        if (isdigit(c)) {
            int start = i;
            while (isdigit(source[i])) i++;
            tokens[tokenIndex++] = createToken(TOKEN_NUMBER, strndup(&source[start], i - start), line);
            i--;
        } else if (isalpha(c)) {
            int start = i;
            while (isalnum(source[i])) i++;
            char *lexeme = strndup(&source[start], i - start);

            if (strcmp(lexeme, "print") == 0) {
                tokens[tokenIndex++] = createToken(TOKEN_PRINT, lexeme, line);
            } else {
                tokens[tokenIndex++] = createToken(TOKEN_IDENTIFIER, lexeme, line);
            }
            free(lexeme);
            i--;
        } else {
            switch (c) {
                case '+': tokens[tokenIndex++] = createToken(TOKEN_PLUS, "+", line); break;
                case '-': tokens[tokenIndex++] = createToken(TOKEN_MINUS, "-", line); break;
                case '*': tokens[tokenIndex++] = createToken(TOKEN_MULTIPLY, "*", line); break;
                case '/': tokens[tokenIndex++] = createToken(TOKEN_DIVIDE, "/", line); break;
                case '=': tokens[tokenIndex++] = createToken(TOKEN_ASSIGN, "=", line); break;
                case '(': tokens[tokenIndex++] = createToken(TOKEN_LEFT_PAREN, "(", line); break;
                case ')': tokens[tokenIndex++] = createToken(TOKEN_RIGHT_PAREN, ")", line); break;
                default:
                    reportError("lexer", line, "Unexpected character encountered.");
            }
        }
    }
    tokens[tokenIndex++] = createToken(TOKEN_EOF, "", line);
    return tokens;
}