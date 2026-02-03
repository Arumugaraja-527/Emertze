#include "header.h"
 
//Global Variables
char input_string[50];
char prompt[20];

int main()
{
    system("clear");

    strcpy(prompt, "Minishell$:~");

    scan_input(prompt, input_string); // fn call
}