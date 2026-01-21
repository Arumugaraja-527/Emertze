#include "inverted.h"

int update_database(hash_t *hash, Node **head, char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("[ERROR] Failed to open backup file: %s\n", filename);
        return FAILURE;
    }

    printf("\n============================================================\n");
    printf("                 UPDATING DATABASE FROM BACKUP               \n");
    printf("============================================================\n");
    printf("[INFO] Reading data from backup file: %s\n", filename);

    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = '\0';   // removes newline
        
        // the backup file should start and end with # in every line
        if (line[0] != '#' || line[strlen(line) - 1] != '#')
        {
            printf("[ERROR] Invalid format in backup file. Each line must start and end with '#'.\n");
            fclose(fp);
            return FAILURE;
        }

        line[strlen(line) - 1] = '\0';
        char *data = line + 1;

        char *token = strtok(data, ";");
        if (!token)
        {
            continue;
        }
        int index = atoi(token);

        char *word = strtok(NULL, ";");
        if (!word)
        {
            continue;
        }

        char *file_count_str = strtok(NULL, ";");
        if (!file_count_str)
        {
            continue;
        }
        int file_count = atoi(file_count_str);

        printf("\n[ENTRY] Word: '%s' | Index: %d | File Count: %d\n", word, index, file_count);

        for (int i = 0; i < file_count; i++)
        {
            char *file_name = strtok(NULL, ";");
            char *count_str = strtok(NULL, ";");
            if (!file_name || !count_str)
            {
                break;
            }

            int count = atoi(count_str);
            
            //insert the data into the database using this function

            insert_to_database(hash, index, word, file_name, count);

            //printf("   ↳ File: %-15s | Word Count: %d\n", file_name, count);
        }
    }

    fclose(fp);


    return SUCCESS;
}
