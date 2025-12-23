#include "APC.h"

int is_valid_number(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] < '0' || str[i] > '9')
            return FAILURE;         // not valid
    }
    return SUCCESS;                // valid
}

int process_operand(char *oper, Dlist **head, Dlist **tail)
{
    int i = 0;
    while (oper[i])
    {
        // Check if character is a valid digit
        if (oper[i] < '0' || oper[i] > '9') 
        {
            printf("Error: Invalid digit '%c' in first operand\n", oper[i]);
            return FAILURE;
        }
        
        if (insert_last(head, tail, oper[i] - '0') != SUCCESS) 
        {
            printf("Error: Failed to insert digit into list1\n");
            return FAILURE;
        }

        i++;
    }
    return SUCCESS;
}

void print_list(Dlist *head)
{
    /* Cheking the list is empty or not */
    if (head == NULL)
    {
        printf("INFO : List is empty\n");
    }
    else
    {
        //printf("\nResult : ");
        while (head)
        {
            /* Printing the list */
            printf("%d", head->data);

            /* Travering in forward direction */
            head = head->next;
        }
        printf("\n");
    }
}

int insert_last(Dlist **head, Dlist **tail, int data)
{
    Dlist *new = (Dlist*) malloc( sizeof(Dlist) );   // create new node
    if (new == NULL)                                 // validate node
    {
        return FAILURE;
    }
    new->data = data;
    new->prev = NULL;
    new->next = NULL;

    if (*head == NULL)
    {
        *head = *tail = new;                          // Address stored
        return SUCCESS;
    }

    (*tail)->next = new;
    new->prev = *tail;
    *tail = new;

    return SUCCESS;
}

int insert_first(Dlist **head, Dlist **tail, int data)
{
    Dlist *new = (Dlist*) malloc(sizeof(Dlist));        // Create new node
    if (new == NULL)                                   // Validate node
    {  
        return FAILURE;
    }
    new->data = data;                                 // Updating data and link
    new->prev = new->next = NULL;

    // empty list
    if (*head == NULL)                             
    {
        *head = *tail = new;
        new->next = new->prev = NULL;
        return SUCCESS;
    }

   // Non empty list
    new->prev = NULL;
    new->next = *head;
    (*head)->prev = new;
    *head = new;

    return SUCCESS;
}

int remove_zeros(Dlist **head, Dlist **tail)
{
    // delete first
    if (*head == NULL)
    {
        return FAILURE;
    }
    if (*head == *tail) // Single node
    {
        // only one node
        return SUCCESS;
    }
    while ((*head)->data == 0)
    {
        *head = (*head)->next; // update head
        free((*head)->prev);   // free last node
        (*head)->prev = NULL;  // make previous node as null
        
    }
    return SUCCESS;
}

int dl_delete_list(Dlist **head, Dlist **tail)
{
    if(*head == NULL)
    {
        return FAILURE;
    }
    while( (*head)->next  != NULL)          // will not enter for last node
    {
        //update head
        *head = (*head)->next;
        free((*head)->prev);
        (*head)->prev = NULL;
        
    }
    free(*head);
    *head = * tail = NULL;
    return SUCCESS;

}


