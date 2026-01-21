
#include "inverted.h"

int search_database(hash_t *hash, char *s)
{
    if (!hash || !s || !s[0]) 
    {
        printf("[ERROR] Invalid arguments to search.\n");
        return FAILURE;
    }

    int index;
    char ch = s[0];

    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        index = (ch & ~32) - 'A'; 
    else if (ch >= '0' && ch <= '9')
        index = 26;
    else
        index = 27;

    if (hash[index].h_link == NULL) 
    {
        printf("[INFO] No entries found in this index for '%s'.\n", s);
        return FAILURE;
    }

    main_t *maintemp = hash[index].h_link;

    while (maintemp != NULL) 
    {
        if (strcmp(maintemp->word, s) == 0) 
        {
            printf("\n============================================================\n");
            printf("                    SEARCH RESULT                            \n");
            printf("============================================================\n");

            printf(" Word        : " "%-20s ", maintemp->word);
            printf("| File Count : %d\n", maintemp->file_count);

            sub_t *subtemp = maintemp->ms_link;
            while (subtemp != NULL) 
            {
                printf("     ↳ ""File: ""%s  ", subtemp->file_name);
                printf("| Word Count: %d\n", subtemp->word_count);
                subtemp = subtemp->s_link;
            }

            printf("------------------------------------------------------------\n");
            printf("[SUCCESS] Word '%s' found in the database.\n", s);
            printf("============================================================\n");

            return SUCCESS;
        }
        maintemp = maintemp->m_link;
    }

   // printf("[INFO] Word '%s' not found in the database.\n", s);
    return FAILURE;
}
