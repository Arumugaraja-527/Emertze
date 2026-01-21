/*
NAME        : ARUMUGARAJA K
DATE        : 07-12-2025
DESCRIPTION : INVERTED SERACH

create_database() → builds database from input .txt files

Save_database() → saves hash table into backup file

update_database() → reads backup file and reconstructs the hash table

display_database() → prints the hash table

*/

#include "inverted.h"

int main(int argc, char *argv[])
{
    Node *head = NULL;

    printf("\n============================================================\n");
    printf("               WELCOME TO INVERTED SEARCH ENGINE             \n");
    printf("============================================================\n");

    if (argc < 2)
    {
        printf("\n============================================================\n");
        printf("                   INVERTED SEARCH ENGINE                    \n");
        printf("============================================================\n");
        printf("Usage:\n");
        printf("   %s <input_file1> <input_file2> ...\n", argv[0]);
        printf("Example:\n");
        printf("   %s sample1.txt sample2.txt\n", argv[0]);
        printf("============================================================\n");
        return FAILURE;
    }

    if (read_and_validate(argc, argv, &head) == 0)
    {
        printf("\n[ERROR] Input file validation failed!\n");
        printf("        One or more input files are invalid or unreadable.\n");
        printf("        Please verify filenames and access permissions.\n");
        return FAILURE;
    }

    printf("\n[INFO] Input files validated successfully.\n");
    printf("------------------------------------------------------------\n");
    printf(" Files to be processed:\n");

    Node *temp = head;
    int i = 1;
    while (temp != NULL)
    {
        printf("   [%02d] %s\n", i, temp->file_name);
        i++;
        temp = temp->link;
    }

    printf("------------------------------------------------------------\n");

    hash_t hash[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        hash[i].index = i;
        hash[i].h_link = NULL;
    }

    char buffer[100];
    int option;

    int isCreated = 0;
    int isUpdated = 0;


    while (1)
    {
        printf("\n----------------------- MAIN MENU ---------------------------\n");
        printf("   [1] Create Database\n");
        printf("   [2] Display Database\n");
        printf("   [3] Search Database\n");
        printf("   [4] Update Database\n");
        printf("   [5] Save Database\n");
        printf("   [6] Exit Program\n");
        printf("------------------------------------------------------------\n");
        printf("Enter your choice [1-6]: ");

        if (scanf("%d", &option) != 1)
        {
            printf("\n[ERROR] Invalid input. Please enter a numeric value (1–6).\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option)
        {
            case 1:

                if (isCreated || isUpdated)
                {
                    printf("[ERROR] Database already exists. Cannot create again.\n");
                    break;
                }
                if (!head)
                {
                    printf("[INFO] No new files available for database creation.\n");
                    break;
                }
                if (create_database(hash, &head) == SUCCESS)
                {
                    printf("[SUCCESS] Database created successfully.\n");
                    isCreated = 1;
                }
                else
                    printf("[FAILURE] Database creation failed.\n");
                break;

            case 2:
                if (is_hash_empty(hash) == FAILURE)
                {
                    if (display_database(hash) == SUCCESS)
                        printf("[SUCCESS] Database displayed successfully.\n");
                    else
                        printf("[FAILURE] Error displaying database.\n");
                }
                else
                    printf("[INFO] Database is empty. Nothing to display.\n");
                break;

            case 3:
                if (is_hash_empty(hash) == FAILURE)
                {
                    printf("Enter the word to search: ");
                    scanf("%99s", buffer);
                    if (search_database(hash, buffer) == SUCCESS)
                        printf("[SUCCESS] Word '%s' found in the database.\n", buffer);
                    else
                        printf("[INFO] Word '%s' not found in the database.\n", buffer);
                }
                else
                    printf("[INFO] Database is empty. Please create it first.\n");
                break;


            case 4:
                 if (isCreated == 1)
                {
                   printf("[ERROR] Cannot update after creating database.\n");
                   printf("        Restart program to use update option.\n");
                   break;
                }

                if (is_hash_empty(hash) == FAILURE)
               {
                   printf("[ERROR] Database already exists. Update not possible.\n");
                   break;
                }

                printf("Enter the backup filename to update from: ");
                scanf("%99s", buffer);

                if (update_database(hash, &head, buffer) == SUCCESS)
                {
                    printf("[SUCCESS] Database updated successfully.\n");
                    isUpdated = 1;
                }
                else
                    printf("[FAILURE] Database update failed.\n");
                break;

            case 5:
                if (is_hash_empty(hash) == FAILURE)
                {
                    printf("Enter the filename to store the database : ");
                    scanf("%99s", buffer);
                    if( Save_database(hash , buffer) == SUCCESS)
                    {
                        printf("[SUCCESS] Database saved successfully.\n");
                    }
                    else
                    {
                       printf("[FAILURE] Error in saving database.\n");
                    }
                }
                else
                 printf("[INFO] Database is empty. Nothing to save.\n");
                break;

            case 6:
                printf("\n[EXIT] Exiting program... Goodbye!\n");
                printf("============================================================\n");
                return 0;

            default:
                printf("[ERROR] Invalid choice. Please select option 1–6.\n");
        }
    }

    return 0;
}

 