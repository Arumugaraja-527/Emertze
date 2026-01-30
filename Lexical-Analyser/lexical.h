#ifndef LEXICAL_H
#define LEXICAL_H

#define MAX_KEYWORDS 20
#define MAX_TOKEN_SIZE 100
#define NUM_OPERATORS (sizeof(operators) / sizeof(operators[0]))

#include "types.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    KEYWORD,//0
    OPERATOR,//1
    SPECIAL_CHARACTER,//2
    INTEGER_CONSTANT,//3
    FLOATING_POINT_CONSTANT,//4
    CHARACTER_CONSTANT,//5
    STRING_LITERAL,//6
    IDENTIFIER,//7
    PREPROCESSOR_DIRECTIVE,//8
    END_OF_FILE,//9
    UNKNOWN//10
} TokenType;

typedef struct
{
    char lexical[MAX_TOKEN_SIZE];   // Stores token read from c file
    TokenType type;                 // Stores an integer indicating what kind of token is it
}Token;

Status initializeLexer(const char* filename);    //Opens and prepares the input file for tokenization.

Token getNextToken();                          /*scans the input file character by character, groups characters into meaningful tokens,
                                                and returns them one at a time.*/

Status categorizeToken(Token* token);             //Assigns a TokenType to a token based on tokenization.
Status isKeyword(const char *str);
//Status isOperator(const char*str);
Status isSpecialCharacter(const char*str);
Status isIntFloat(int ch,Token *token,int *index);
Status isIdentifier(const char *str);
const char* tokenTypeToString(Token* token);

#endif