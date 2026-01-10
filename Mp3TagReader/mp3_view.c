#include "mp3_header.h"

//Converts the 4-byte syncsafe integer in ID3v2 header/frame size to integer.
static int convert_size(unsigned char *buff)
{
    int size = 0;
    size = (buff[0] << 21) | (buff[1] << 14) | (buff[2] << 7) | buff[3];
    return size;
}

//Opens the MP3 file and reads its ID3v2 tag to display metadata.
void mp3_view(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Error: Cannot open file '%s'\n", filename);
        return;
    }

    unsigned char header[10];
    if (fread(header, 1, 10, fp) != 10)
    {
        printf("Error: File too small or invalid.\n");
        fclose(fp);
        return;
    }
    // Check if file has ID3v2 tag
    if (strncmp((char *)header, "ID3", 3) != 0)
    {
        printf("No ID3v2 tag found in '%s'\n", filename);
        fclose(fp);
        return;
    }

    int tag_size = convert_size(header + 6);
    long tag_end = tag_size + 10;
    // Calculate end position of ID3v2 tag
    printf("----------------------------------------------------------------------------------\n");
    printf("MP3 Tag reader and Editor for ID3v2\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("MP3 File: %s\n", filename);
    printf("---- ID3v2 TAG DETAILS ----\n");
    printf("----------------------------------------------------------------------------------\n");
    // Loop through all frames until the end of tag    [ Frame ID  = 4bytes][ Frame Size = 4 bytes ][ Flags = 2 ][ Data ]

    while (ftell(fp) < tag_end)
    {
        char frame_id[5];
        unsigned char size_bytes[4];
        unsigned char flags[2];

       // Read frame ID (4 bytes)
        if (fread(frame_id, 1, 4, fp) != 4)
        {
            break;
        }
        frame_id[4] = '\0';   

        if (frame_id[0] == 0)     // checks the end of the frames
        {
            break;
        } 
        // Read frame size (4 bytes) and flags (2 bytes)
        if (fread(size_bytes, 1, 4, fp) != 4)
        {
            break;
        }

        fread(flags, 1, 2, fp);   //  Read flag 

        int frame_size = convert_size(size_bytes);
        
        if (frame_size <= 0 || frame_size > 5000)
        {
            fseek(fp, 1, SEEK_CUR);
            // Skip invalid frame
            continue;
        }

        unsigned char *data = malloc(frame_size + 1);
        // Allocate memory to store frame data
        if (!data)
        {
            printf("Memory allocation failed.\n");
            break;
        }

        size_t read_bytes = fread(data, 1, frame_size, fp);
        data[read_bytes] = '\0';

        char *text = (char *)(data + 1);    //skips the first byte it tells the encoding type
        // Match frame ID and print its content
        if (strcmp(frame_id, "TIT2") == 0)
        {
            printf("Title   : %s\n", text);
        }
        else if (strcmp(frame_id, "TPE1") == 0)
        {
            printf("Artist  : %s\n", text);
        }
        else if (strcmp(frame_id, "TALB") == 0)
        {
            printf("Album   : %s\n", text);
        }
        else if (strcmp(frame_id, "TYER") == 0)
        {    
            printf("Year    : %s\n", text);
        }
        else if (strcmp(frame_id, "COMM") == 0)       // TCOM OR COMM
        {
            printf("Comment : %s\n", text);
        }
        else if (strcmp(frame_id, "TCON") == 0)
        {
            printf("Genre   : %s\n", text);
        }

        free(data);
    }
    printf("----------------------------------------------------------------------------------\n");

    fclose(fp);
}