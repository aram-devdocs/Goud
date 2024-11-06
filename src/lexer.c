#include "goud.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_COUNT 1000 // Increased to handle larger scripts

// Helper function to create a new token
Token createToken(TokenType type, const char *lexeme, int line)
{
    Token token;
    token.type = type;
    token.lexeme = strdup(lexeme);
    token.line = line;
    return token;
}

// Tokenize the source code into an array of tokens
Token *tokenize(const char *source)
{
    Token *tokens = malloc(sizeof(Token) * MAX_TOKEN_COUNT);
    int tokenIndex = 0, line = 1;

    for (int i = 0; source[i] != '\0'; i++)
    {
        char c = source[i];

        // Handle newlines
        if (c == '\n')
        {
            line++;
            tokens[tokenIndex++] = createToken(TOKEN_NEWLINE, "\\n", line);
            continue;
        }

        // Ignore whitespace
        if (isspace(c))
        {
            continue;
        }

        // Skip comments
        if (c == '#')
        {
            while (source[i] != '\n' && source[i] != '\0')
                i++; // Skip to end of line
            i--;     // Adjust because for-loop will increment
            continue;
        }

        // Handle numbers
        if (isdigit(c))
        {
            int start = i;
            while (isdigit(source[i]))
                i++;
            tokens[tokenIndex++] = createToken(TOKEN_NUMBER, strndup(&source[start], i - start), line);
            i--;
        }

        // Handle identifiers and keywords
        else if (isalpha(c))
        {
            int start = i;
            while (isalnum(source[i]))
                i++;
            char *lexeme = strndup(&source[start], i - start);

            // Check for keywords
            if (strcmp(lexeme, "print") == 0)
            {
                tokens[tokenIndex++] = createToken(TOKEN_PRINT, lexeme, line);
            }
            else if (strcmp(lexeme, "if") == 0)
            {
                tokens[tokenIndex++] = createToken(TOKEN_IF, lexeme, line);
            }
            else if (strcmp(lexeme, "else") == 0)
            {
                tokens[tokenIndex++] = createToken(TOKEN_ELSE, lexeme, line);
            }
            else
            {
                tokens[tokenIndex++] = createToken(TOKEN_IDENTIFIER, lexeme, line);
            }
            free(lexeme);
            i--;
        }

        // Handle single-character tokens
        else
        {
            switch (c)
            {
            case '+':
                tokens[tokenIndex++] = createToken(TOKEN_PLUS, "+", line);
                break;
            case '-':
                tokens[tokenIndex++] = createToken(TOKEN_MINUS, "-", line);
                break;
            case '*':
                tokens[tokenIndex++] = createToken(TOKEN_MULTIPLY, "*", line);
                break;
            case '/':
                tokens[tokenIndex++] = createToken(TOKEN_DIVIDE, "/", line);
                break;
            case '=':
                if (source[i + 1] == '=')
                {
                    tokens[tokenIndex++] = createToken(TOKEN_EQUAL, "==", line);
                    i++;
                }
                else
                {
                    tokens[tokenIndex++] = createToken(TOKEN_ASSIGN, "=", line);
                }
                break;
            case '!':
                if (source[i + 1] == '=')
                {
                    tokens[tokenIndex++] = createToken(TOKEN_NOT_EQUAL, "!=", line);
                    i++;
                }
                else
                {
                    reportError("lexer", line, "Unexpected character '!'");
                }
                break;
            case '<':
                if (source[i + 1] == '=')
                {
                    tokens[tokenIndex++] = createToken(TOKEN_LESS_EQUAL, "<=", line);
                    i++;
                }
                else
                {
                    tokens[tokenIndex++] = createToken(TOKEN_LESS, "<", line);
                }
                break;
            case '>':
                if (source[i + 1] == '=')
                {
                    tokens[tokenIndex++] = createToken(TOKEN_GREATER_EQUAL, ">=", line);
                    i++;
                }
                else
                {
                    tokens[tokenIndex++] = createToken(TOKEN_GREATER, ">", line);
                }
                break;
            case '(':
                tokens[tokenIndex++] = createToken(TOKEN_LEFT_PAREN, "(", line);
                break;
            case ')':
                tokens[tokenIndex++] = createToken(TOKEN_RIGHT_PAREN, ")", line);
                break;
            case ':':
                tokens[tokenIndex++] = createToken(TOKEN_COLON, ":", line);
                break;
            default:
                reportError("lexer", line, "Unexpected character encountered.");
            }
        }
    }
    tokens[tokenIndex++] = createToken(TOKEN_EOF, "", line);
    return tokens;
}

// Free allocated memory for tokens
void freeTokens(Token *tokens)
{
    for (int i = 0; tokens[i].type != TOKEN_EOF; i++)
    {
        free(tokens[i].lexeme);
    }
    free(tokens);
}