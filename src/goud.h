#ifndef GOUD_H
#define GOUD_H

#include <stdio.h>

// Error handling
void reportError(const char *file, int line, const char *message);

// Token types for the lexer
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_ASSIGN,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_PRINT,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char *lexeme;
    int line;
} Token;

// Function prototypes for lexer, parser, and evaluator
Token *tokenize(const char *source);
void freeTokens(Token *tokens);
void parse(Token *tokens);
int parseExpression(Token *tokens, int *index);
int parseTerm(Token *tokens, int *index);
int parseFactor(Token *tokens, int *index);

#endif