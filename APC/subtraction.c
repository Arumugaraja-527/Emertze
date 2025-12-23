#include "APC.h"

int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,  Dlist **headR, Dlist **tailR, char *argv[])
{
    int flag = 0, borrow = 0;

    // Step 1: Decide which number is larger and swap if needed
    if (argv != NULL)
    {
        if (strlen(argv[1]) < strlen(argv[3])) // swap
        {
            digit_to_list(head1, tail1, head2, tail2, argv[3], argv[1]);
            flag = 1;
        }
        
        else if (strlen(argv[1]) > strlen(argv[3])) // no swap
        {
            digit_to_list(head1, tail1, head2, tail2, argv[1], argv[3]);
            flag = 0;
        }
        else
        {
            if (strcmp(argv[1], argv[3]) < 0) // same length, compare values
            {
                digit_to_list(head1, tail1, head2, tail2, argv[3], argv[1]); // swap
                flag = 1;
            }
            else
            {
                digit_to_list(head1, tail1, head2, tail2, argv[1], argv[3]); // no swap
                flag = 0;
            }
        }
    }

    Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;

    // Step 2: Perform subtraction from rightmost digit LSB
    while (temp1 || temp2)
    {
        if (temp1 && temp2)
        {
            // Handle borrow if active
            if (borrow == 1)
            {
                if (temp1->data - 1 < 0)
                {
                    temp1->data = 9;
                    borrow = 1;
                }
                else
                {
                    temp1->data = temp1->data - 1;
                    borrow = 0;
                }
            }

            // Perform subtraction
            if ((temp1->data) < (temp2->data))
            {
                borrow = 1;
                insert_first(headR, tailR, (temp1->data + 10) - temp2->data);
            }
            else
            {
                insert_first(headR, tailR, temp1->data - temp2->data);
                borrow = 0;
            }

            temp1 = temp1->prev;
            temp2 = temp2->prev;
        }
        else if (temp2 == NULL && temp1 != NULL)
        {
            // Remaining digits in temp1
            if (borrow == 1)
            {
                if (temp1->data - 1 < 0)
                {
                    insert_first(headR, tailR, 9);
                    borrow = 1;
                }
                else
                {
                    insert_first(headR, tailR, temp1->data - 1);
                    borrow = 0;
                }
            }
            else
            {
                insert_first(headR, tailR, temp1->data);
            }

            temp1 = temp1->prev;
        }
        else
        {
            // temp2 longer (won’t happen since swapped earlier)
            break;
        }
    }

    // Step 3: Remove leading zeros
    remove_zeros(headR, tailR);

    // Step 4: Apply negative sign if needed
    if (flag == 1)
    {
        (*headR)->data *= -1;
    }

    return SUCCESS;
}
