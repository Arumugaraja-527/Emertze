#include "inverted.h"

int Save_database(hash_t *hash,char *f)
{
    if(!valid_filename(f)) // validate filename
    {
        return FAILURE;
    }

    FILE *fptr = fopen(f,"w");
    if (!fptr)
    {
        printf("File is not opened\n");
        return FAILURE;
    }
    for(int i=0; i<SIZE; i++)
    {
        if(hash[i].h_link == NULL)
        {
            continue;
        }
        main_t *maintemp = hash[i].h_link;
        while(maintemp)
        {
            fprintf(fptr,"#%d;%s;%d", i, maintemp->word, maintemp->file_count);  

            sub_t *subtemp = maintemp->ms_link;
            while(subtemp)
            {
                fprintf(fptr,";%s;%d;", subtemp->file_name, subtemp->word_count);
                subtemp = subtemp->s_link;
            }
            fprintf(fptr,"#\n");
            maintemp = maintemp->m_link;
        }
    }

    fseek(fptr,0,SEEK_END);
    long size = ftell(fptr);
    rewind(fptr);
    fclose(fptr);

    if(size)
    {
        return SUCCESS;
    }
    return FAILURE;
    
}