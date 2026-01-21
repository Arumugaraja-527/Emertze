#include "inverted.h"

/*
1 . Reads each file in the file-list
2 . Extracts words from the file
3 . Inserts each word into the hash table
4 . Creates a MainNode if the word is new
5 . Creates/updates SubNode if the word exists
6 . Removes file node after processing
*/

int create_database(hash_t *hash, Node **head)
{
    // If no files are present in the linked list, we cannot create database
    if (*head == NULL)
    {
        printf("[ERROR] No files available for processing.\n");
        return FAILURE;
    }

    Node *temp = *head;         // Pointer to traverse the file linked list
    Node *prev_temp = NULL;     // Used for deleting file node after processing
    FILE *fptr;                 // File pointer for opening files
    char word[100];             // To store each word read from file

    // Process each file one by one
    while (temp != NULL)
    {
        // Open the current file in read mode
        fptr = fopen(temp->file_name, "r");
        if (!fptr)  // If file cannot be opened
        {
            printf("[ERROR] Could not open file: %s\n", temp->file_name);
            temp = temp->link;   // Move to the next file
            continue;            // Skip this file
        }

        printf("[INFO] Reading data from file: %s\n", temp->file_name);

        // Read each word from the file
        while (fscanf(fptr, "%99s", word) != EOF)
        {
            int index;
            char ch = word[0];  // First character of the word

            // Calculate hash index based on first character
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
                index = (ch & ~32) - 'A';  // Convert to uppercase and map to A=0, B=1...
            else if (ch >= '0' && ch <= '9')
                index = 26;                // Index for numbers
            else
                index = 27;                // Index for special characters

            // Start searching for the word in the main node list
            main_t *mainTemp = hash[index].h_link;
            main_t *prev = NULL;

            // Traverse main list to check if this word already exists
            while (mainTemp && strcmp(mainTemp->word, word) != 0)
            {
                prev = mainTemp;          // Move previous pointer
                mainTemp = mainTemp->m_link; // Move to next main node
            }

            // If the word DOES NOT exist → create a new main node
            if (mainTemp == NULL)
            {
                main_t *new_main = malloc(sizeof(main_t)); // Allocate main node
                if (!new_main)
                {
                    printf("[ERROR] Memory allocation failed for main node.\n");
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(new_main->word, word); // Store the new word
                new_main->file_count = 1;      // This word occurs in one file
                new_main->m_link = NULL;       // No next main node yet
                new_main->ms_link = NULL;      // No sub list yet

                // Create sub node to store file name and its word count
                sub_t *new_sub = malloc(sizeof(sub_t));
                if (!new_sub)    // if the new_sub not created free before new_main
                {
                    printf("[ERROR] Memory allocation failed for sub node.\n");
                    free(new_main);
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(new_sub->file_name, temp->file_name); // Store which file
                new_sub->word_count = 1;                     // Word occurs once
                new_sub->s_link = NULL;                      // End of sub list

                new_main->ms_link = new_sub; // Attach sub node to main node
                
                // Insert new main node into the hash bucket list
                if (hash[index].h_link == NULL)
                    hash[index].h_link = new_main;  // Bucket was empty: new_main is the first node
                else
                    prev->m_link = new_main;        // Append new_main at the end of the existing main list
            }
            else
            {
                // Word already exists → Check its sub list (files list)
                sub_t *subTemp = mainTemp->ms_link;
                sub_t *subPrev = NULL;

                // Search for this file name in the sub list
                while (subTemp && strcmp(subTemp->file_name, temp->file_name) != 0)
                {
                    subPrev = subTemp;         // Move previous pointer
                    subTemp = subTemp->s_link; // Move to next sub node
                }

                // If file name is NOT found → create a new sub node
                if (subTemp == NULL)
                {
                    sub_t *new_sub = malloc(sizeof(sub_t));
                    if (!new_sub)
                    {
                        printf("[ERROR] Memory allocation failed for sub node.\n");
                        fclose(fptr);
                        return FAILURE;
                    }

                    strcpy(new_sub->file_name, temp->file_name); // Store file name
                    new_sub->word_count = 1;                     // First occurrence
                    new_sub->s_link = NULL;

                    subPrev->s_link = new_sub;   // Attach new sub node
                    mainTemp->file_count++;      // Increase file count
                }
                else
                {
                    // File already exists → increase word count
                    subTemp->word_count++;
                }
            }
        }

        fclose(fptr); // Close the file
        printf("[SUCCESS] Finished processing file: %s\n", temp->file_name);

        // Remove the processed file from the file linked list
        prev_temp = temp;
        temp = temp->link;
        remove_file(head, prev_temp->file_name);
    }

    printf("[INFO] All files processed successfully. Database creation complete.\n");
    return SUCCESS;
}
