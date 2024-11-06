#ifndef GOUD_H
#define GOUD_H

#include <stdio.h>

// Error handling
void reportError(const char *file, int line, const char *message) __attribute__((noreturn));

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
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS,
    TOKEN_GREATER,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_COLON,
    TOKEN_EOF,
    TOKEN_NEWLINE  // Added to handle newlines
} TokenType;

typedef struct {
    TokenType type;
    char *lexeme;
    int line;
} Token;

// Lexer function prototypes
Token *tokenize(const char *source);
void freeTokens(Token *tokens);

// Parser function prototypes
void parse(Token *tokens);
int parseExpression(Token *tokens, int *index);
int parseTerm(Token *tokens, int *index);
int parseFactor(Token *tokens, int *index);
void parseBlock(Token *tokens, int *index);
int parseCondition(Token *tokens, int *index);
void parseStatement(Token *tokens, int *index);

// Variable management function prototypes
int getVariableValue(const char *name);
void setVariableValue(const char *name, int value);

#endif