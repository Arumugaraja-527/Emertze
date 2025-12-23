#include "APC.h"

int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR, char *argv[])
{
	/* Definition goes here */
	digit_to_list(head1, tail1, head2, tail2, argv[1], argv[3]);
	remove_zeros(head1, tail1);
	remove_zeros(head2, tail2);

	if ((*head1)->data==0 || (*head2)->data==0)
	{
		printf("Result: 0");
		return SUCCESS;
	}
	
	int carry = 0, count = 0;
	Dlist *temp1 = *tail1, *temp2 = *tail2;
	Dlist *headR1 = NULL, *tailR1 = NULL, *headR2 = NULL, *tailR2 = NULL; // To store result of mul

	while (temp2 != NULL)
	{
		for (int i = 0; i < count; i++) // Adding Zeros
		{
			insert_first(&headR2, &tailR2, 0);
		}

		while (temp1 != NULL)
		{
			// Multiplication logic
			int digit = (temp1->data * temp2->data) + carry;
			//	printf("mul: %d\n", digit);
			if (digit > 9) // its two digit number and there is carry
			{
				carry = digit / 10;
				if (temp2->next == NULL)
					insert_first(&headR1, &tailR1, digit % 10);
				else
				{
					insert_first(&headR2, &tailR2, digit % 10);
					// printf("Each digit adding:");
					// print_list(headR2);
				}
			}
			else
			{
				carry = 0;
				if (temp2->next == NULL)
					insert_first(&headR1, &tailR1, digit % 10);
				else
					insert_first(&headR2, &tailR2, digit % 10);
			}

			temp1 = temp1->prev;
		}
		

		if (carry != 0)
		{

			if (temp2->next == NULL)
			{
				// printf("carry is %d", carry);
				insert_first(&headR1, &tailR1, carry);
			}
			else
			{
				insert_first(&headR2, &tailR2, carry);
			}
			carry = 0;
		}
		// printf("\n-- Before addition --\nHeadR1: ");
		// print_list(headR1);
		// printf("HeadR2: ");
		// print_list(headR2);

		if (temp2->next != NULL)
		{

			addition(&headR1, &tailR1, &headR2, &tailR2, headR, tailR);

			// printf("After addition: ");
			// print_list(*headR);
			// printf("\n");

			dl_delete_list(&headR1, &tailR1);
			dl_delete_list(&headR2, &tailR2);
			headR1 = *headR;
			tailR1 = *tailR;
			*headR = *tailR = NULL;
		}

		temp2 = temp2->prev;
		temp1 = *tail1;
		count++;
	}

	// printf("Final Result:");
	remove_zeros(&headR1, &tailR1);
	print_list(headR1);
}
