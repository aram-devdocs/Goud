#include "goud.h"
#include <stdio.h>
#include <stdlib.h>

void parse(Token *tokens)
{
    int index = 0;
    while (tokens[index].type != TOKEN_EOF)
    {
        Token currentToken = tokens[index];

        if (currentToken.type == TOKEN_IDENTIFIER && tokens[index + 1].type == TOKEN_ASSIGN)
        {
            index += 2;
            int value = parseExpression(tokens, &index);
            setVariableValue(currentToken.lexeme, value);
        }
        else if (currentToken.type == TOKEN_PRINT)
        {
            index++;
            int value = parseExpression(tokens, &index);
            printf("Output: %d\n", value);
        }
        else
        {
            reportError("parser", currentToken.line, "Expected assignment or print statement.");
        }
    }
}

int parseExpression(Token *tokens, int *index)
{
    int result = parseTerm(tokens, index);
    while (tokens[*index].type == TOKEN_PLUS || tokens[*index].type == TOKEN_MINUS)
    {
        Token operatorToken = tokens[(*index)++];
        int right = parseTerm(tokens, index);
        result = (operatorToken.type == TOKEN_PLUS) ? result + right : result - right;
    }
    return result;
}

int parseTerm(Token *tokens, int *index)
{
    int result = parseFactor(tokens, index);
    while (tokens[*index].type == TOKEN_MULTIPLY || tokens[*index].type == TOKEN_DIVIDE)
    {
        Token operatorToken = tokens[(*index)++];
        int right = parseFactor(tokens, index);
        result = (operatorToken.type == TOKEN_MULTIPLY) ? result * right : result / right;
    }
    return result;
}

int parseFactor(Token *tokens, int *index)
{
    Token currentToken = tokens[(*index)++];
    if (currentToken.type == TOKEN_NUMBER)
    {
        return atoi(currentToken.lexeme);
    }
    else if (currentToken.type == TOKEN_IDENTIFIER)
    {
        return getVariableValue(currentToken.lexeme);
    }
    else if (currentToken.type == TOKEN_LEFT_PAREN)
    {
        int value = parseExpression(tokens, index);
        if (tokens[*index].type != TOKEN_RIGHT_PAREN)
        {
            reportError("parser", currentToken.line, "Expected ')'");
        }
        (*index)++;
        return value;
    }
    else
    {
        reportError("parser", currentToken.line, "Unexpected token");
    }
}