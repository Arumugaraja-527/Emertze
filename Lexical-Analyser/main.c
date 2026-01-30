#include "lexical.h"

int main(int argc ,char *argv[])
{
    if(initializeLexer(argv[1]) == e_success)
    {
        Token token;
        while(1)
        {
            token = getNextToken();

            if(token.type == END_OF_FILE)
            {
                break;
            }
            if(token.lexical[0] != '\0')
            {
                printf("Token : %-30s         Type: %s\n", token.lexical, tokenTypeToString(&token));
            }
        }
        return 1;
    }

    return 0;
}