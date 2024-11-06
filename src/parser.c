#include "goud.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
int parseCondition(Token *tokens, int *index);
void parseStatement(Token *tokens, int *index);
void parseBlock(Token *tokens, int *index);
void skipBlock(Token *tokens, int *index);

// Parser functions
void parse(Token *tokens) {
    int index = 0;
    while (tokens[index].type != TOKEN_EOF) {
        parseStatement(tokens, &index);
    }
}

void parseStatement(Token *tokens, int *index) {
    Token currentToken = tokens[*index];

    switch (currentToken.type) {
        case TOKEN_IF: {
            (*index)++;
            int condition = parseCondition(tokens, index);

            // Expect colon after the condition
            if (tokens[*index].type != TOKEN_COLON) {
                reportError("parser", tokens[*index].line, "Expected ':' after if condition.");
            }
            (*index)++; // Skip the colon

            if (condition) {
                parseBlock(tokens, index); // Execute `if` block

                // After executing the `if` block, check for an `else` and skip it
                if (tokens[*index].type == TOKEN_ELSE) {
                    (*index)++;
                    if (tokens[*index].type != TOKEN_COLON) {
                        reportError("parser", tokens[*index].line, "Expected ':' after else.");
                    }
                    (*index)++; // Skip the colon
                    skipBlock(tokens, index); // Skip the `else` block since `if` was true
                }
            } else {
                // Skip `if` block
                skipBlock(tokens, index);

                // Check for else block
                if (tokens[*index].type == TOKEN_ELSE) {
                    (*index)++;
                    if (tokens[*index].type != TOKEN_COLON) {
                        reportError("parser", tokens[*index].line, "Expected ':' after else.");
                    }
                    (*index)++; // Skip the colon
                    parseBlock(tokens, index); // Execute `else` block
                }
            }
            break;
        }

        case TOKEN_ELSE: {
            reportError("parser", currentToken.line, "Unexpected 'else' without matching 'if'.");
            break;
        }

        case TOKEN_PRINT: {
            (*index)++;
            int value = parseExpression(tokens, index);
            printf("Output: %d\n", value);

            // Skip any newlines
            while (tokens[*index].type == TOKEN_NEWLINE) {
                (*index)++;
            }
            break;
        }

        case TOKEN_IDENTIFIER: {
            if (tokens[*index + 1].type == TOKEN_ASSIGN) {
                Token identifier = tokens[(*index)++];
                (*index)++; // Skip '='
                int value = parseExpression(tokens, index);
                setVariableValue(identifier.lexeme, value);

                // Skip any newlines
                while (tokens[*index].type == TOKEN_NEWLINE) {
                    (*index)++;
                }
            } else {
                reportError("parser", currentToken.line, "Expected assignment after identifier");
            }
            break;
        }

        case TOKEN_NEWLINE: {
            (*index)++; // Skip newlines
            break;
        }

        default: {
            reportError("parser", currentToken.line, "Unexpected statement or token.");
            break;
        }
    }
}

void parseBlock(Token *tokens, int *index) {
    // Parse statements until the end of the block
    while (tokens[*index].type != TOKEN_EOF &&
           tokens[*index].type != TOKEN_ELSE &&
           tokens[*index].type != TOKEN_IF) {

        parseStatement(tokens, index);
    }
}

void skipBlock(Token *tokens, int *index) {
    // Skip statements until the end of the block
    while (tokens[*index].type != TOKEN_EOF &&
           tokens[*index].type != TOKEN_ELSE &&
           tokens[*index].type != TOKEN_IF) {

        // Skip over the current token
        (*index)++;
    }
}

int parseCondition(Token *tokens, int *index) {
    int left = parseExpression(tokens, index);

    Token operatorToken = tokens[(*index)++];
    int right = parseExpression(tokens, index);

    switch (operatorToken.type) {
        case TOKEN_EQUAL:
            return left == right;
        case TOKEN_NOT_EQUAL:
            return left != right;
        case TOKEN_LESS:
            return left < right;
        case TOKEN_GREATER:
            return left > right;
        case TOKEN_LESS_EQUAL:
            return left <= right;
        case TOKEN_GREATER_EQUAL:
            return left >= right;
        default:
            reportError("parser", operatorToken.line, "Invalid conditional operator");
            return 0; // Ensure there's a return statement for all paths
    }
}

int parseExpression(Token *tokens, int *index) {
    int result = parseTerm(tokens, index);
    while (tokens[*index].type == TOKEN_PLUS || tokens[*index].type == TOKEN_MINUS) {
        Token operatorToken = tokens[(*index)++];
        int right = parseTerm(tokens, index);
        if (operatorToken.type == TOKEN_PLUS) {
            result += right;
        } else {
            result -= right;
        }
    }
    return result;
}

int parseTerm(Token *tokens, int *index) {
    int result = parseFactor(tokens, index);
    while (tokens[*index].type == TOKEN_MULTIPLY || tokens[*index].type == TOKEN_DIVIDE) {
        Token operatorToken = tokens[(*index)++];
        int right = parseFactor(tokens, index);
        if (operatorToken.type == TOKEN_MULTIPLY) {
            result *= right;
        } else {
            if (right == 0) {
                reportError("runtime", tokens[*index - 1].line, "Division by zero");
            }
            result /= right;
        }
    }
    return result;
}

int parseFactor(Token *tokens, int *index) {
    Token currentToken = tokens[(*index)++];
    if (currentToken.type == TOKEN_NUMBER) {
        return atoi(currentToken.lexeme);
    } else if (currentToken.type == TOKEN_IDENTIFIER) {
        return getVariableValue(currentToken.lexeme);
    } else if (currentToken.type == TOKEN_LEFT_PAREN) {
        int value = parseExpression(tokens, index);
        if (tokens[*index].type != TOKEN_RIGHT_PAREN) {
            reportError("parser", tokens[*index].line, "Expected ')'");
        }
        (*index)++;
        return value;
    } else {
        reportError("parser", currentToken.line, "Unexpected token");
        return 0; // Added to satisfy return type
    }
}