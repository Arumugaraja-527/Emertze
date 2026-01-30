#include "lexical.h"


static const char *keywords[MAX_KEYWORDS] = { // maximum keywords value 20 given

    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct" };

const char *operators[] = {

    "==", "!=", "<=", ">=", "&&", "||", "++", "--",
    "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=",
    "<<", ">>",
    "+", "-", "*", "/", "%", "=", "!", "<", ">", "|", "&", "^" };

static const char *specialCharacters = ",;{}()[]";
FILE *fptr;

Status initializeLexer(const char *filename) // Opens and prepares the input file for tokenization.
{
    if ((fptr = fopen(filename, "r")) == NULL)
    {
        perror("File open failed");
        return e_failure;
    }
    return e_success;
}

Token getNextToken() /*scans the input file character by character, groups characters into meaningful tokens,and returns them one at a time.*/
{
    int ch, index = 0; // for tracking the index of token;
    Token c_tokens;    // lexical[],tokentype

    while ((ch = fgetc(fptr)) != EOF)
    {
        /***checking for space until encoutering first non space character***/
        if (isspace(ch)) // if space enters loop
        {
            if (index > 0) // Now we already have a token so return it
            {
                c_tokens.lexical[index] = '\0';
                break;
            }
            continue;
        }
        /***checking for integer and float***/
        else if (index == 0 && ((ch >= '0' && ch <= '9') || ch == '.'))
        {
            c_tokens.lexical[index++] = ch; // saving first char to index
            isIntFloat(ch, &c_tokens, &index);
            return c_tokens;
        }
        /***checking for delimitter,identifier,string,character,operators,preprocessor,specialcharacter***/
        else if (ispunct(ch))
        {
            if (index > 0) // act as delimitter or (special case) int_xyz-------->Identifier
            {
                if (ch == '_')
                {
                    c_tokens.lexical[index++] = ch;
                    continue;
                }
                else
                {
                    // now punctuation act as delimitter
                    ungetc(ch, fptr);
                    c_tokens.lexical[index] = '\0';
                    break;
                }
            }
            // INDEX ==0
            else if (ch == '_')
            {
                c_tokens.lexical[index++] = ch;
                continue;
            }
            else if (ch == '\"') // strings
            {
                c_tokens.lexical[index++] = ch;
                while ((ch = fgetc(fptr)) != EOF)
                {
                    if (ch == '\"')
                    {
                        c_tokens.lexical[index++] = '\"';
                        break;
                    }
                    else
                    {
                        c_tokens.lexical[index++] = ch;
                    }
                }
                c_tokens.lexical[index] = '\0';
                c_tokens.type = STRING_LITERAL;
                return c_tokens;
            }
            else if (ch == '\'') // character
            {
                c_tokens.lexical[index++] = ch;
                while ((ch = fgetc(fptr)) != EOF)
                {
                    if (ch == '\'')
                    {
                        c_tokens.lexical[index++] = '\'';
                        break;
                    }
                    else
                    {
                        c_tokens.lexical[index++] = ch;
                    }
                }
                c_tokens.lexical[index] = '\0';
                c_tokens.type = CHARACTER_CONSTANT;
                return c_tokens;
            }
            else if (strchr("+-*/%=!<>|&^", ch) != NULL) // check for operators
            {
                c_tokens.lexical[index++] = ch;
                // check for 2 character operator
                int ch2 = fgetc(fptr);

                    c_tokens.lexical[index++] = ch2; // add second character to the buffer
                    c_tokens.lexical[index] = '\0';
                  
                    for (int i = 0; i < NUM_OPERATORS; i++)
                    {
                        if (strcmp(operators[i], c_tokens.lexical) == 0)
                        {
                            c_tokens.type = OPERATOR;
                            return c_tokens;
                        }
                    }
                
                // if not 2 character operator
                ungetc(ch2, fptr);
                c_tokens.lexical[--index] = '\0';
                c_tokens.type = OPERATOR;
                return c_tokens;
            }
            else if (ch == '#')      // A preprocessor directive starts with # and continues till end of the line \n
            {
                int num = index;
                c_tokens.lexical[index++] = ch;
                while (((ch = fgetc(fptr)) != EOF) && ch != '\n')
                {
                    if(ch != '\r')     // carriage return
                        c_tokens.lexical[index++] = ch;
                }
                c_tokens.lexical[index] = '\0';
                c_tokens.type = PREPROCESSOR_DIRECTIVE;
                return c_tokens;
            }
            else
            {
                // single character punctuation(special character) ( ) { } ; , [ ]
                c_tokens.lexical[0] = ch;
                c_tokens.lexical[1] = '\0';
                break;
            }
        } 

        else  // normal characters accumulation keywords, identifiers until delimiter found
        {
            c_tokens.lexical[index++] = ch;
        }
    }

    if (ch == EOF && index == 0)
    {
        c_tokens.type = END_OF_FILE;
        strcpy(c_tokens.lexical, "EOF");
        return c_tokens;
    }

    categorizeToken(&c_tokens);

    return c_tokens;
}

Status categorizeToken(Token *token) // Assigns a TokenType to a token based on its lexical.
{

    // If none of the above match
    // token->type = UNKNOWN;
    /*isKeyword → YES
    isIdentifier → also YES
    But keyword must win. so we are checking keyword first
    */
    if ((isKeyword(token->lexical)) == e_success)
        token->type = KEYWORD;
    else if ((isSpecialCharacter(token->lexical)) == e_success)
        token->type = SPECIAL_CHARACTER;
    else if ((isIdentifier(token->lexical)) == e_success)
        token->type = IDENTIFIER;
    else
        token->type = UNKNOWN;
}

// Status isOperator(const char *str)
// {
//     for (int i = 0; i < NUM_OPERATORS; i++)
//     {
//         if (strcmp(str, operators[i]) == 0)
//             return e_success;
//     }
//     return e_failure;
// }

Status isSpecialCharacter(const char *str)
{

    if (strchr(specialCharacters, str[0]) != NULL)
        return e_success;
    else
        return e_failure;
}

Status isKeyword(const char *str)
{
    for (int i = 0; i < MAX_KEYWORDS; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
        {
            return e_success;
        }
    }
    return e_failure;
}

/*
Identifier :
   1. Not empty
   2. First char → letter or _
   3. Rest → alnum or _
   4. Not a keyword
*/
Status isIdentifier(const char *str)
{

    if (str == NULL || str[0] == '\0')
        return e_failure;

    // First character must be a letter or underscore
    if (!(isalpha(str[0]) || str[0] == '_'))
        return e_failure;

    // Remaining characters must be letters, digits, or underscores
    for (int i = 1; str[i] != '\0'; i++)
    {
        if (!(isalnum(str[i]) || str[i] == '_'))
            return e_failure;
    }

    // Must not be a keyword
    if (isKeyword(str) == e_success)
        return e_failure;

    return e_success;
}

/*
Continues reading digits
Tracks . using dotCheck
Pushes back invalid char
Assigns:

   INTEGER_CONSTANT
   FLOATING_POINT_CONSTANT
   UNKNOWN (. only)
*/
Status isIntFloat(int ch, Token *token, int *index)
{
    // 1.Checking for integer and float : switch_const ==0
    int dotCheck = (ch == '.'); // If the number starts with . → dotCheck = 1 Else → dotCheck = 0
    int temp_ch;
    while ((temp_ch = fgetc(fptr)) != EOF)
    {
        if (temp_ch >= '0' && temp_ch <= '9')
        {
            token->lexical[(*index)++] = temp_ch;
        }
        else if (temp_ch == '.' && !dotCheck)
        {
            token->lexical[(*index)++] = temp_ch; // if dot is at 0 th pos then now it points to 1
            dotCheck = 1;
        }
        else
        {
            ungetc(temp_ch, fptr);
            break;
        }
    }
    token->lexical[*index] = '\0';
    if ((*index == 1) && dotCheck) // Only one character and it is a dot
    {

        token->type = UNKNOWN;
        return e_success;
    }
    else if (dotCheck)
    {

        token->type = FLOATING_POINT_CONSTANT;
        return e_success;
    }
    else
    {
        token->type = INTEGER_CONSTANT;
        return e_success;
    }
}

const char *tokenTypeToString(Token *token)
{
    switch (token->type)
    {
    case KEYWORD:
        return "KEYWORD";
    case OPERATOR:
        return "OPERATOR";
    case SPECIAL_CHARACTER:
        return "SPECIAL_CHARACTER";
    case INTEGER_CONSTANT:
        return "INTEGER_CONSTANT";
    case FLOATING_POINT_CONSTANT:
        return "FLOATING_POINT_CONSTANT";
    case CHARACTER_CONSTANT:
        return "CHARACTER_CONSTANT";
    case STRING_LITERAL:
        return "STRING_LITERAL";
    case IDENTIFIER:
        return "IDENTIFIER";
    case PREPROCESSOR_DIRECTIVE:
        return "PREPROCESSOR_DIRECTIVE";
    case END_OF_FILE:
        return "END_OF_FILE";
    case UNKNOWN:
        return "UNKNOWN";
    }
}