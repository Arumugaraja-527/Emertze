#include "APC.h"

int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
	int count_list1, count_list2, count_sub = 0, non_zero_count = 0;

	Dlist *temp2 = *head2;
	count_list2 = 0;

	while (temp2 != NULL)
	{
		count_list2++;                               // count the no of digits in divisior
		if (temp2->data != 0)          
		{
			non_zero_count++;           
		}
		temp2 = temp2->next;
	}
	// printf("nonzerocount : %d\n",non_zero_count);
	if (non_zero_count == 0)
	{
		printf("Error: Divisor is empty list. Division by zero is not allowed.\n");
		return FAILURE;
	}

	while (1)
	{
		Dlist *temp1 = *head1;
		count_list1 = 0;

		while (temp1 != NULL)
		{
			count_list1++;                     // count the no of digits in the dividend
			temp1 = temp1->next;
		}

		// printf("list1 count = %d , list2 count = %d", count_list1, count_list2);
        // Dividend is greater than divisor
		if (count_list1 > count_list2)
		{
			subtraction(head1, tail1, head2, tail2, headR, tailR, NULL);
			dl_delete_list(head1, tail1);
			*head1 = *headR;
			*tail1 = *tailR;
			*headR = *tailR = NULL;
			count_sub++;             // counts the no of times subraction happens

			// printf("If count_list1>count_list2\n");
			// print_list(*head1);
		}

		else if (count_list1 < count_list2)
		{
			// printf("Result: %d", count_sub);
			// printf("If count_list1<count_list2\n");
			// print_list(*head1);
			break;
		}
		else          // If counts equal, compare digit by digit
		{
			temp1 = *head1;
			temp2 = *head2;
			while (temp1 != NULL && temp2 != NULL)
			{
				if (temp1->data > temp2->data) // list1>list2
				{
					subtraction(head1, tail1, head2, tail2, headR, tailR, NULL);
					dl_delete_list(head1, tail1);
					*head1 = *headR;
					*tail1 = *tailR;
					*headR = *tailR = NULL;
					count_sub++;
					// printf("Equal case: First list is greater than Second list\n");
					// print_list(*head1);
					break; 
				}
				else if (temp1->data < temp2->data)
				{
					// printf("Equal case: First list is less than Second list\n");
					// print_list(*head1);
					// printf("Result: %d\n", count_sub);
					printf("%d\n", count_sub);
					return SUCCESS;
				}

				temp1 = temp1->next;
				temp2 = temp2->next;
			}
			// If all digits equal, it means exactly divisible
			if (temp1 == NULL && temp2 == NULL)
			{
				count_sub++;
				break;
			}
		}
	}
	printf("%d\n", count_sub);
	return SUCCESS;
}