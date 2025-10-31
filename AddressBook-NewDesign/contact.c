/*
NAME                :   ARUMUGARAJA K
DATE                :   17-09-2025
DESCRIPTION         :   ADDRESS BOOK

This project is a menu-driven Address Book application in C that allows
the user to manage contact information effectively. The program supports:

1. Create Contact   – Add a new contact with Name, Phone, and Email. 
2. List Contacts    – Display all contacts in a tabular, sorted format.
3. Search Contact   – Search by Name, Phone, or Email.
4. Edit Contact     – Update details of an existing contact.
5. Delete Contact   – Remove a contact using its serial number.
6. File Handling    – Save contacts to a file and load them at startup.
7. Validation       – Check email format, uniqueness, phone number rules.
8. Sorting          – Automatically sort contacts alphabetically.
9. User-Friendly    – Menu system with clear prompts and error handling.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"
//#include "populate.h"

void listContacts(AddressBook *addressBook)               // Function for displaying the contact
{
    if (addressBook->contactCount == 0)
    {
        printf("\nNo contacts to display!!!!!!!!\n");
        return;
    }

    // Create an array of indexes of original structure array
    int sortedIndexes[MAX_CONTACTS];
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        sortedIndexes[i] = i;
    }

    // Sort the index array based on the contact names (case-insensitive) by bubble sort
    for (int i = 0; i < addressBook->contactCount - 1; i++)
    {
        for (int j = 0; j < addressBook->contactCount - i - 1; j++)
        {
            if (strcasecmp(addressBook->contacts[sortedIndexes[j]].name, addressBook->contacts[sortedIndexes[j + 1]].name) > 0)
            {
                // Swap indexes
                int temp = sortedIndexes[j];
                sortedIndexes[j] = sortedIndexes[j + 1];
                sortedIndexes[j + 1] = temp;
            }
        }
    }

    // Display contacts in sorted order
    printf("\n======================================================================\n");
    printf(" %-3s  %-20s  %-18s   %-23s\n", "No", "Name", "Phone", "Email");
    printf("======================================================================\n");

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        int idx = sortedIndexes[i];
        printf(" %-3d  %-20s  %-18s   %-23s\n", i + 1, addressBook->contacts[idx].name, addressBook->contacts[idx].phone, addressBook->contacts[idx].email);
    }

    printf("======================================================================\n");
}
 
void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;

    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void createContact(AddressBook *addressBook)                  // Function for creating the contact
{
    if (addressBook->contactCount >= MAX_CONTACTS)
    {
        printf("\nAddress book is full. Cannot add more contacts.\n");
        return;
    }

    char temp[100];
    int index = addressBook->contactCount;

    // Input and validate Name
    while (1)
    {
        printf("Enter the Name: ");
        scanf(" %[^\n]", temp);                       // Read full line with spaces

        if (is_valid_name(temp))
        {
            strcpy(addressBook->contacts[index].name, temp);
            break;
        }
        else
        {
            printf("\nInvalid Name! Only alphabets are allowed.\n");
        }
    }

    // Input and validate Phone Number
    while (1)
    {
        printf("Enter the Mobile Number: ");
        scanf(" %s", temp);
        getchar();                             // Consume leftover newline character

        if (isDuplicatePhone(addressBook, temp))
        {
            printf("\nInvalid phone number! This number already exists.\n");
            continue;
        }

        if (is_valid_phone(temp))
        {
            strcpy(addressBook->contacts[index].phone, temp);
            break;
        }
        else
        {
            printf("\nInvalid Phone Number! Only digits are allowed.\n");
        }
    }

    // Input and validate Email ID
    while (1)
    {
        printf("Enter the Email-ID: ");
        scanf(" %s", temp);

        if (isDuplicateEmail(addressBook, temp))
        {
            printf("\nInvalid Email! This email already exists.\n");
            continue;
        }

        if (is_valid_email(temp))
        {
            strcpy(addressBook->contacts[index].email, temp);
            break;
        }
        else
        {
            printf("\nInvalid Email Format!\n");
        }
    }

    addressBook->contactCount++;

    printf("\nContact added successfully!\n");
}

void searchContact(AddressBook *addressBook)                  // function for searching by name, number, mail
{
    int choice;
    char search[50];
    char again;

    while (1)
    {
        printf("\nSearch Menu\n");
        printf("1. NAME\n");
        printf("2. PHONE NUMBER\n");
        printf("3. EMAIL\n");
        printf("4. EXIT FROM SEARCH MENU\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        int found = 0;
        int matchIndexes[100];
        int matchCount = 0;

        switch (choice)
        {
        case 1:
            printf("Enter name to search: ");
            scanf(" %[^\n]", search);
            if (!is_valid_name(search))
            {
                printf("\nInvalid name format!!!\n");
                break;
            }

            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcasecmp(addressBook->contacts[i].name, search) == 0)
                {
                    matchIndexes[matchCount++] = i;
                }
            }

            if (matchCount == 0)
            {
                printf("\nNo contact found with the name '%s'.\n", search);
                break;
            }

            printf("\n=======================================================================\n");
            printf(" %-3s  %-20s  %-18s   %-23s\n", "No", "Name", "Phone", "Email");
            printf("======================================================================\n");

            for (int i = 0; i < matchCount; i++)
            {
                int idx = matchIndexes[i];
                printf(" %-3d  %-20s  %-18s   %-23s\n", i + 1, addressBook->contacts[idx].name, addressBook->contacts[idx].phone, addressBook->contacts[idx].email);
            }
            printf("======================================================================\n");

            break;

        case 2:
            printf("Enter Phone Number to search: ");
            scanf(" %[^\n]", search);
            if (!is_valid_phone(search))
            {
                printf("\nInvalid Phone Number format!!!\n");
                break;
            }

            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].phone, search) == 0)
                {
                    printf("\n======================================================================\n");
                    printf(" %-3s  %-20s  %-18s   %-23s\n", "No", "Name", "Phone", "Email");
                    printf("======================================================================\n");
                    printf(" %-3d  %-20s  %-18s   %-23s\n", 1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
                    printf("======================================================================\n");
                    found = 1;
                    break;
                }
            }

            if (!found)
            {
                printf("\nNo contact found with the Phone Number '%s'.\n", search);
            }
            break;

        case 3:
            printf("Enter EMAIL-ID to search: ");
            scanf(" %[^\n]", search);
            if (!is_valid_email(search))
            {
                printf("\nInvalid Email format!!!\n");
                break;
            }

            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].email, search) == 0)
                {
                    printf("\n=======================================================================\n");
                    printf(" %-3s  %-20s  %-18s   %-23s\n", "No", "Name", "Phone", "Email");
                    printf("======================================================================\n");
                    printf(" %-3d  %-20s  %-18s   %-23s\n",  1, addressBook->contacts[i].name,  addressBook->contacts[i].phone, addressBook->contacts[i].email);
                    printf("======================================================================\n");
                    found = 1;
                    break;
                }
            }

            if (!found)
            {
                printf("\nNo contact found with the EMAIL-ID '%s'.\n", search);
            }
            break;

        case 4:
            printf("Exiting search menu...\n");
            return;

        default:
            printf("\nPlease enter a valid choice (1-4).\n");
            break;
        }

        printf("\nDo you want to search again? (y/n): ");
        scanf(" %c", &again);
        getchar();

        if (again != 'y')
        {
            break;
        }
    }
}

void editContact(AddressBook *addressBook)         // function for edit data
{
    /* Define the logic for Editcontact */
    int index, choice;
    char again = 'y';
    char phone[15];
    char email[50];
    char name[20];

    while (again == 'y')
    {
        printf("\nEdit Menu:\n");
        printf("1. Edit Name\n");
        printf("2. Edit Phone Number\n");
        printf("3. Edit Email-ID\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        index = searchData(addressBook, choice);

        if (index >= 0 && index < addressBook->contactCount)
        {
            switch (choice)
            {
            case 1:
                while (1)
                {
                    printf("Enter new name:");
                    scanf(" %[^\n]", name);
                    getchar();

                    if (is_valid_name(name))
                    {
                        strcpy(addressBook->contacts[index].name, name);
                        break;
                    }
                    else
                    {
                        printf("\nEnter valid name.\n");
                    }
                }
                break;

            case 2:
                while (1)
                {
                    printf("Enter new phone number: ");
                    scanf(" %[^\n]", phone);
                    getchar();

                    if (is_valid_phone(phone))
                    {
                        strcpy(addressBook->contacts[index].phone, phone);
                        break;
                    }
                    else
                    {
                        printf("\nInvalid phone number! Enter a 10-digit numeric phone number.\n");
                    }
                }
                break;

            case 3:
                while (1)
                {
                    printf("Enter new Email-ID: ");
                    scanf(" %[^\n]", email);
                    getchar();

                    if (is_valid_email(email))
                    {
                        strcpy(addressBook->contacts[index].email, email);
                        break;
                    }
                    else
                    {
                        printf("\nInvalid Email-ID format! Please try again.\n");
                    }
                }
                break;

            default:
                printf("\nInvalid choice! Please enter 1-4.\n");
                continue;
            }

            printf("\nEdit Successful.\n");
        }
        else
        {
            printf("\nContact not found.\n");
        }

        printf("Do you want to edit another contact? (y/n): ");
        scanf(" %c", &again);
    }

    printf("Exiting edit menu.\n");
}

void deleteContact(AddressBook *addressBook)          // Function for delete data
{   
    /* Define the logic for deleteContact here */
    int index, i, choice;
    char again = 'y';

    do
    {
        printf("\nDelete Menu:\n");
        printf("1. Delete by name\n");
        printf("2. Delete by phone number\n");
        printf("3. Delete by Email-ID\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        index = searchData(addressBook, choice);

        if (index >= 0 && index < addressBook->contactCount)
        {
            for (i = index; i < addressBook->contactCount - 1; i++)
            {
                addressBook->contacts[i] = addressBook->contacts[i + 1];
            }

            addressBook->contactCount--;

            printf("\nContact deleted successfully.\n");
        }
        else
        {
            printf("\nContact not found.\n");
        }

        printf("Do you want to delete another contact? (y/n): ");
        scanf(" %c", &again);
        getchar();

    } while (again == 'y');

    printf("Exiting delete menu.\n");
}

// validate name
int is_valid_name(char *name)
{

    int i = 0;
    while (name[i] != '\0')
    {
        if ( !( isalpha(name[i]) ) )
        {
            return 0;                       // invalid character
        }
        i++;
    }
    return 1;                               // valid

}

// validate phone number
int is_valid_phone(char *phone)
{
    if(strlen(phone) != 10)
    {
        printf("Phone number should be exactly 10 digits only.\n");
        return 0;
    }

    for(int i = 0; phone[i] != '\0'; i++)
    {
        if ( !isdigit(phone[i]) )
        {
            printf("Phone number should contain only digits.\n");
            return 0;
        }
    }

    return 1;
}

// validate E-mail
int is_valid_email(char *email)
{
    int len = strlen(email);

    // 1. Check minimum length
    if (len < 6) 
    {
        return 0;
    }

    // 2. must end with ".com"
    if (strcmp(email + len - 4, ".com") != 0)
    {
        return 0;
    }

    // 3. validate @ only once & not first or last
    int atCount = 0;
    for (int i = 0; i < len; i++)
    {
        if (email[i] == '@')
        {
            atCount++;
            if (i == 0)  
            {
                printf("Error: @ cannot be the first character\n");
                return 0;
            }
        }
    //4. consecutive dots
        if (email[i] == '.' && i > 0 && email[i - 1] == '.')
        {
            printf("Error: Email cannot contain consecutive dots\n");
            return 0;
        }
    }

    if (atCount != 1)
    {
        printf("Error: Email must contain exactly one @\n");
        return 0;
    }

    // 5. Check for consecutive dots
    for (int i = 0; i < len - 1; i++)
    {
        if (email[i] == '.' && email[i + 1] == '.')
        {
            return 0;
        }
    }

    // 6. Check characters between @ and .com
    char *atPos = strchr(email, '@');
    char *dotPos = strstr(email, ".com");

    if (dotPos - atPos <= 1)
    {
        printf("Error: Must have at least one character between @ and .com\n");
        return 0;
    }

    return 1;              // valid email
}

//check for unique Phone Number
int isDuplicatePhone(AddressBook *addressBook, const char *phone)
{
    int duplicateCount = 0;

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            duplicateCount++;
        }
    }
    if (duplicateCount == 0)
        return 0;
    else
        return 1;
}

// check for unique E-mail id
int isDuplicateEmail(AddressBook *addressBook, const char *mail)
{
    int duplicateCount = 0;

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if ( strcmp(addressBook->contacts[i].email, mail) == 0 )
        {
            duplicateCount++;
        }
    }
    if (duplicateCount == 0)
        return 0;
    else
        return 1;
}

int searchData(AddressBook *addressBook, int choice)
{
    char search[100];
    int matchIndexes[100];
    int matchCount = 0;

    switch (choice)
    {
    case 1:   // Search by Name
    {
        printf("Enter name to search: ");
        scanf(" %[^\n]", search);

        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcasecmp(addressBook->contacts[i].name, search) == 0)
            {
                matchIndexes[matchCount++] = i;
            }
        }

        if (matchCount == 0)
        {
            printf("\nName not found!\n");
            return -1;
        }

        if (matchCount == 1)
        {
            int idx = matchIndexes[0];
            printf("\n=======================================================================\n");
            printf(" %-3s  %-20s  %-18s   %-23s\n", "No", "Name", "Phone", "Email");
            printf("=======================================================================\n");
            printf(" %-3d  %-20s  %-18s   %-23s\n", 1, addressBook->contacts[idx].name, addressBook->contacts[idx].phone, addressBook->contacts[idx].email);
            printf("=======================================================================\n");
            return idx;
        }

        // Multiple matches
        printf("\n=======================================================================\n");
        printf(" %-3s  %-20s  %-18s   %-23s\n", "No", "Name", "Phone", "Email");
        printf("=======================================================================\n");
        for (int i = 0; i < matchCount; i++)
        {
            int idx = matchIndexes[i];
            printf(" %-3d  %-20s  %-18s   %-23s\n", i + 1, addressBook->contacts[idx].name, addressBook->contacts[idx].phone, addressBook->contacts[idx].email);
        }
        printf("=======================================================================\n");

        int serialChoice;
        printf("\nMultiple contacts found!!\n");
        printf("\nEnter the serial number of the contact: ");
        scanf("%d", &serialChoice);

        if (serialChoice < 1 || serialChoice > matchCount)
        {
            printf("\nInvalid serial number!\n");
            return -1;
        }

        return matchIndexes[serialChoice - 1];
    }

    case 2:   // Search by Phone
    {
        printf("Enter phone number to search: ");
        scanf("%s", search);

        if (!is_valid_phone(search))
        {
            printf("\nInvalid phone number format.\n");
            return -1;
        }

        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].phone, search) == 0)
            {
                printf("\n=======================================================================\n");
                printf(" %-3s  %-20s  %-18s   %-23s\n", "No", "Name", "Phone", "Email");
                printf("=======================================================================\n");
                printf(" %-3d  %-20s  %-18s   %-23s\n",  1,  addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
                printf("=======================================================================\n");
                return i;
            }
        }

        printf("\nPhone number not found!\n");
        return -1;
    }

    case 3:   // Search by Email
    {
        printf("Enter email to search: ");
        scanf("%s", search);

        if (!is_valid_email(search))
        {
            printf("\nInvalid email format.\n");
            return -1;
        }

        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].email, search) == 0)
            {
                printf("\n=======================================================================\n");
                printf(" %-3s  %-20s  %-18s   %-23s\n", "No", "Name", "Phone", "Email");
                printf("=======================================================================\n");
                printf(" %-3d  %-20s  %-18s   %-23s\n", 1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
                printf("=======================================================================\n");
                return i;
            }
        }

        printf("\nEmail not found!\n");
        return -1;
    }

    default:
        printf("\nInvalid search choice!\n");
        return -1;
    }
}
