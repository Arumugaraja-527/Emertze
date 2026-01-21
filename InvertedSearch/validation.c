#include "inverted.h"

 //   Checks whether the file name ends with ".txt"
int valid_filename(char *s)
{
    char *pos = strstr(s, ".txt");  // not found return 0 

    // must contain ".txt" at end of filename
    if (!pos || pos == s || *(pos + 4) != '\0')    // if any of the condition is true then return failure
        return FAILURE;

    return SUCCESS;
}

//    Checks if a file is empty
int is_file_empty(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);   // move to end of file
    long size = ftell(fptr);    // get file size
    rewind(fptr);               // go back to beginning  for future purpose
    return (size == 0);         // true if size = 0
}


//    Checks if filename already exists in linked list

int is_duplicate(Node *head, char *filename)
{
    Node *temp = head;   // temp node to traverse the single linked list

    while (temp)
    {
        if (strcmp(temp->file_name, filename) == 0)
            return SUCCESS;     // duplicate found
        temp = temp->link;
    }

    return FAILURE;             // no duplicate
}


//    Inserts a new filename into the linked list
void insert_file(Node **head, char *filename)
{
    Node *new = malloc(sizeof(Node));     // create a new node of type Node

    strcpy(new->file_name, filename);
    new->link = NULL;

    // first file
    if (*head == NULL)
    {
        *head = new;
    }
    else
    {
        // insert at last
        Node *temp = *head;
        while (temp->link != NULL)
            temp = temp->link;
        temp->link = new;
    }
}


int read_and_validate(int argc, char *argv[], Node **head)
{
    printf("\n========== VALIDATING INPUT FILES ==========\n");

    for (int i = 1; i < argc; i++)
    {
        // check .txt
        if (!valid_filename(argv[i]))
        {
            printf("[ERROR] Invalid file name: '%s' (must end with .txt)\n", argv[i]);
            continue;
        }

        // check duplicate
        if (is_duplicate(*head, argv[i]))
        {
            printf("[WARNING] Duplicate file skipped: '%s'\n", argv[i]);
            continue;
        }

        // check opening
        FILE *fptr = fopen(argv[i], "r");
        if (!fptr)
        {
            printf("[ERROR] Cannot open file: '%s'\n", argv[i]);
            continue;
        }

        // check empty
        if (is_file_empty(fptr))
        {
            printf("[INFO] File is empty: '%s'\n", argv[i]);
            fclose(fptr);
            continue;
        }

        fclose(fptr);

        // insert valid file
        insert_file(head, argv[i]);
        printf("[SUCCESS] File added: '%s'\n", argv[i]);
    }

    // if no file is valid
    if (*head == NULL)
    {
        printf("\nNo valid input files found.\n");
        return FAILURE;
    }

    printf("\nAll valid files processed successfully.\n");
    return SUCCESS;
}

int is_hash_empty(hash_t *hash)
{
    int flag = 0;
    for (int i = 0; i < SIZE; i++)
    {
        if (hash[i].h_link != NULL)
            flag = 1;
    }

    if (flag)
        return FAILURE;
    else
        return SUCCESS;
}

void insert_to_database(hash_t *hash, int index, char *word, char *file_name, int word_count)
{
    // Get the first main node in the hash table bucket
    main_t *mainTemp = hash[index].h_link;

    // To keep track of previous main node (important when inserting new node)
    main_t *prev = NULL;

    // Search through main linked list to find if word already exists
    while (mainTemp && strcmp(mainTemp->word, word) != 0)
    {
        prev = mainTemp;           // Move prev forward
        mainTemp = mainTemp->m_link; // Move mainTemp to next node
    }

    // If word NOT found in main list → create a new main node
    if (mainTemp == NULL)
    {
        main_t *new_main = malloc(sizeof(main_t)); // Create main node
        if (!new_main)                             // Check memory
        {
            printf("[ERROR] Memory allocation failed for main node.\n");
            return;
        }

        strcpy(new_main->word, word); // Store word in main node
        new_main->file_count = 1;     // This word appears in 1 file for now
        new_main->m_link = NULL;      // No next main node yet
        new_main->ms_link = NULL;     // No sub list yet

        // Create sub node for the file name and count
        sub_t *new_sub = malloc(sizeof(sub_t));
        if (!new_sub)
        {
            printf("[ERROR] Memory allocation failed for sub node.\n");
            free(new_main); // Free main node since sub failed
            return;
        }

        strcpy(new_sub->file_name, file_name); // Store file name
        new_sub->word_count = word_count;      // Store count
        new_sub->s_link = NULL;                // No next sub yet

        new_main->ms_link = new_sub; // Attach this sub node to main node

        // Insert main node into hash table
        if (hash[index].h_link == NULL)
            hash[index].h_link = new_main; // First node in the bucket
        else
            prev->m_link = new_main; // Attach at end of main list
    }
    else
    {
        // Word exists → check sub list (files list)
        sub_t *subTemp = mainTemp->ms_link;
        sub_t *subPrev = NULL;

        // Search for the file name in sublist
        while (subTemp && strcmp(subTemp->file_name, file_name) != 0)
        {
            subPrev = subTemp;         // Move prev forward
            subTemp = subTemp->s_link; // Move to next sub node
        }

        // If file NOT found → create new sub node
        if (subTemp == NULL)
        {
            sub_t *new_sub = malloc(sizeof(sub_t)); // Create sub node
            if (!new_sub)
            {
                printf("[ERROR] Memory allocation failed for sub node.\n");
                return;
            }

            strcpy(new_sub->file_name, file_name); // Store file name
            new_sub->word_count = word_count;      // Store count
            new_sub->s_link = NULL;                // End of list

            subPrev->s_link = new_sub; // Attach new sub to the list
            mainTemp->file_count++;    // Increase file count for this word
        }
        else
        {
            // File exists → just increase the word count
            subTemp->word_count++;
        }
    }
}


int remove_file(Node **head, char *filename)
{
    if (*head == NULL)
    {
        printf("[ERROR] File list is empty.\n");
        return FAILURE;
    }

    Node *temp = *head;
    Node *prev = NULL;

    while (temp)
    {
        if (strcmp(temp->file_name, filename) == 0)
        {
            if (temp == *head)
            {
                *head = temp->link;
                free(temp);
                return SUCCESS;
            }
            else
            {
                prev->link = temp->link;
                free(temp);
                return SUCCESS;
            }
        }
        prev = temp;
        temp = temp->link;
    }
    return SUCCESS;
}
