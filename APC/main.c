/***************************************************************
NAME         : ARUMUGARAJA K
DATE         : 06-12-2025
DESCRIPTION  : Arbitrary Precision Calculator (APC)
 
 *  This project performs arithmetic operations on very large
 *  numbers that cannot be stored in normal C data types.
 *  Each number is stored as a doubly linked list, with one
 *  digit per node. Using this structure, the program supports:
 
       • Addition
       • Subtraction
       • Multiplication
       • Division (if included)
 
  Steps in the project:
       1. Convert input numbers into doubly linked lists.
       2. Remove leading zeros from both numbers.
       3. Validate operators and inputs.
       4. Compare numbers when required.
       5. Perform the chosen operation using list-based logic.
       6. Store the result in a new linked list.
       7. Print the final result in normal number format.
 
    This approach allows handling numbers of unlimited length,
  making the calculator accurate for very large integer values.
 ***************************************************************/

#include "APC.h"

int main(int argc, char *argv[])
{
    if(argc != 4)      // Minimum value of argc should be 4 to perform the operation
    {
        printf("Usage : %s <number1> <operator> <number2>\n", argv[0]);
        return FAILURE;
    }

    if (argv[2][0] != '+' && argv[2][0] != '-' && argv[2][0] != 'x' && argv[2][0] != '/')
	{
		printf("Error: Invalid operator '%s'\nValid operators: +,-,x,/", argv[2]);
		return FAILURE;
	}

    if (is_valid_number(argv[1]) == FAILURE)     // checks whether the given operand is a number digit
	{
		printf("Error: Invalid character in first number: %s\n", argv[1]);
		return FAILURE;
	}

	if (is_valid_number(argv[3]) == FAILURE)
	{
		printf("Error: Invalid character in second number: %s\n", argv[3]);
		return FAILURE;
	}

    Dlist *head1 = NULL, *tail1 = NULL;      // Store operator1
	Dlist *head2 = NULL, *tail2 = NULL;      // Store operator 2
	Dlist *headR = NULL, *tailR = NULL;      // Store result

	char operator = argv[2][0];           // operator passed as 2 nd argument
	char *oper1 = argv[1];
	char *oper2 = argv[3];
     
    switch (operator)
	{
	   case '+':

		    digit_to_list(&head1, &tail1, &head2, &tail2, oper1, oper2);
		    addition(&head1, &tail1, &head2, &tail2, &headR, &tailR);
		    printf("%s + %s = ", argv[1], argv[3]);
		    print_list(headR);
		    break;

	   case '-':

		   subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR, argv);
		    printf("%s - %s = ", argv[1], argv[3]);
		    print_list(headR);
		    break;

	  case 'x':

		   	printf("%s x %s = ", argv[1], argv[3]);
		    multiplication(&head1, &tail1, &head2, &tail2, &headR, &tailR, argv);
		    break;

	  case '/':

	       digit_to_list(&head1, &tail1, &head2, &tail2, oper1, oper2);
		   printf("%s / %s = ", argv[1], argv[3]);
		   division(&head1, &tail1, &head2, &tail2, &headR, &tailR);
		   break;

	  default:

		printf("Invalid Input:-( Try again...\n");
		break;
	}

	return 0;
}
