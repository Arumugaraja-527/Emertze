#include <stdio.h>
#include <string.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *file = fopen("contacts.csv", "w");
    if (file == NULL)
    {
        printf("\nError opening file for writing.\n");
        return;
    }

    // Save contact count 
    fprintf(file, "%d\n", addressBook->contactCount);

    // Save each contact
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(file, "%s,%s,%s\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }

    fclose(file);
    printf("\nContacts saved to file successfully...\n");
}



void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *file = fopen("contacts.csv", "r+");
    if (file == NULL)
    {
        printf("\nNo existing file found...\n");
        return;
    }

    int contactCount = 0;
    fscanf(file, "%d\n", &contactCount);

    addressBook->contactCount = 0;

    for (int i = 0; i < contactCount; i++)
    {
        if (addressBook->contactCount >= MAX_CONTACTS)
        {
            break;
        }
        char name[50], phone[20], email[50];

        // Read : name,phone,email
        fscanf(file, "%[^,],%[^,],%[^\n]\n", name, phone, email);

        strcpy(addressBook->contacts[addressBook->contactCount].name, name);
        strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);
        strcpy(addressBook->contacts[addressBook->contactCount].email, email);

        addressBook->contactCount++;
    }

    fclose(file);
    printf("\nContacts loaded from file successfully.\n");
}

