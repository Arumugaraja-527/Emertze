#include "inverted.h"

int display_database(hash_t *hash)
{
    printf("\n========== Inverted Index Database ==========\n");
    for (int i = 0; i < SIZE; i++)
    {
        if (hash[i].h_link == NULL)
            continue;

        printf("Index [%d]:\n", hash[i].index);
        main_t *maintemp = hash[i].h_link;
        while (maintemp != NULL)
        {
            printf("  Word  : %-20s | File Count : %d\t", maintemp->word, maintemp->file_count);
            sub_t *subtemp = maintemp->ms_link;
            while (subtemp != NULL)
            {
                printf(" File   : %-15s | Word Count : %d\t", subtemp->file_name, subtemp->word_count);
                subtemp = subtemp->s_link;
            }
            printf("\n");
            maintemp = maintemp->m_link;
        }
        printf("\n");
    }
    return SUCCESS;
}