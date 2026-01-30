#include <stdio.h>

// Function to check if a character is a digit
int isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Function to check if a character is a letter
int isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Function to check if a character is an operator
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

// Function to print token type
void printToken(char *token, int type) {
    printf("Token: %s, Type: ", token);
    switch(type) {
        case 1: printf("NUMBER\n"); break;
        case 2: printf("IDENTIFIER\n"); break;
        case 3: printf("OPERATOR\n"); break;
        case 4: printf("DELIMITER\n"); break;
        default: printf("UNKNOWN\n");
    }
}

// Simple lexical analyzer
void analyzeLexemes(char *input) {
    char token[100];
    int j = 0;
    
    for(int i = 0; input[i] != '\0'; i++) {
        if(isDigit(input[i])) {
            token[j++] = input[i];
            if(!isDigit(input[i+1])) {
                token[j] = '\0';
                printToken(token, 1);
                j = 0;
            }
        } else if(isLetter(input[i]) || input[i] == '_') {
            token[j++] = input[i];
            if(!isLetter(input[i+1]) && !isDigit(input[i+1]) && input[i+1] != '_') {
                token[j] = '\0';
                printToken(token, 2);
                j = 0;
            }
        } else if(isOperator(input[i])) {
            token[0] = input[i];
            token[1] = '\0';
            printToken(token, 3);
        } else if(input[i] == '(' || input[i] == ')' || input[i] == ';') {
            token[0] = input[i];
            token[1] = '\0';
            printToken(token, 4);
        }
    }
}