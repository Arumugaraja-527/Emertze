#include "APC.h"

int addition (Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{

	Dlist *temp1 = *tail1;    // Traverse Right to Left
	Dlist *temp2 = *tail2;
	int carry = 0;

	while (temp1 || temp2)     // If values exist in either of the list
	{
		if (temp1 && temp2)
		{
			insert_first(headR, tailR, (temp1->data + temp2->data + carry) % 10); // Adding result to DLL

			if (temp1->data + temp2->data + carry > 9) // result is greater than 9
				carry = 1;
			else
				carry = 0;

			if ((temp1 == *head1 && temp2 == *head2)) //check if both are last nodes
			{
				if (carry == 1)
					insert_first(headR, tailR, carry);
				carry = 0;
			}
			temp1 = temp1->prev;
			temp2 = temp2->prev;
		}
		else if (temp1 != NULL && temp2 == NULL) // list1>list2
		{
			insert_first(headR, tailR, (temp1->data + carry) % 10); // Adding result to DLL

			if (temp1->data + carry > 9) // result is greater than 9
				carry = 1;
			else
				carry = 0;

			if (temp1 == *head1)
			{
				if (carry == 1)
					insert_first(headR, tailR, carry);
				carry = 0;
			}
			temp1 = temp1->prev;
		}
		else if (temp1 == NULL && temp2 != NULL)  //list2>list1
		{
			insert_first(headR, tailR, (temp2->data + carry) % 10); // Adding result to DLL

			if (temp2->data + carry > 9) // result is greater than 9
				carry = 1;
			else
				carry = 0;

			if (temp2 == *head2)
			{
				if (carry == 1)
					insert_first(headR, tailR, carry);
				carry = 0;
				break;
			}
			temp2 = temp2->prev;
		}
	}
	return SUCCESS;
}
