#include "mp3_header.h"


//Converts a 4-byte sync-safe integer from the ID3v2 tag header or frame to an int.
int convert_size(unsigned char *bytes) 
{
    return (bytes[0] << 21) | (bytes[1] << 14) | (bytes[2] << 7) | bytes[3];
}



//Edits a specific ID3v2 tag frame in an opened MP3 file.
void edit_id3v2(FILE *fp, char *option, char *value) 
{
    unsigned char header[10];
    // Read ID3v2 header (first 10 bytes)
    fread(header, 1, 10, fp);
    // Check if file has an ID3v2 tag
    if (strncmp((char *)header, "ID3", 3) != 0) 
    {
        printf("Error: No ID3v2 tag found.\n");
        return;
    }
    // Get total tag size from header (sync-safe integer)
    int tag_size = convert_size(header + 6);
    long tag_end = 10 + tag_size;
    // Loop through frames until end of tag
    while (ftell(fp) < tag_end) 
    {
        char frame_id[5] = {0};
        unsigned char size_bytes[4], flags[2];
        // Read frame ID
        if (fread(frame_id, 1, 4, fp) != 4 || frame_id[0] == 0) 
        {
            break;
        }
        // Read frame size
        if (fread(size_bytes, 1, 4, fp) != 4) 
        {
            break;
        }
        // Read frame flags
        if (fread(flags, 1, 2, fp) != 2)
        {
            break;
        }
         // Convert size and store current frame position
        int frame_size = convert_size(size_bytes);
        long frame_pos = ftell(fp);
        // Skip invalid or unusually large frames
        if (frame_size <= 0 || frame_size > 5000) 
        {
            fseek(fp, 1, SEEK_CUR);
            continue;
        }
        // Check if this frame matches the option to edit
        if ((strcmp(option, "-t") == 0 && strcmp(frame_id, "TIT2") == 0) ||
            (strcmp(option, "-a") == 0 && strcmp(frame_id, "TPE1") == 0) ||
            (strcmp(option, "-A") == 0 && strcmp(frame_id, "TALB") == 0) ||
            (strcmp(option, "-y") == 0 && (strcmp(frame_id, "TYER") == 0 || strcmp(frame_id, "TDRC") == 0)) ||
            (strcmp(option, "-g") == 0 && strcmp(frame_id, "TCON") == 0) ||
            (strcmp(option, "-c") == 0 && strcmp(frame_id, "COMM") == 0)) 
            {
            // Move to start of frame data
            fseek(fp, frame_pos, SEEK_SET);
            // Set text encoding to UTF-8
            unsigned char enc = 0x03; 
            fwrite(&enc, 1, 1, fp);
            // remaining bytes after encoding byte
            int write_len = frame_size - 1;
            int val_len = strlen(value) < write_len ? strlen(value) : write_len;
            // Write new value
            fwrite(value, 1, val_len, fp);

           // Pad remaining space with zeros if needed
            for (int i = val_len; i < write_len; i++) 
            {
                unsigned char zero = 0x00;
                fwrite(&zero, 1, 1, fp);
            }
            return;
        }
        // Move to next frame
        fseek(fp, frame_pos + frame_size, SEEK_SET);
    }

    printf("Specified tag frame not found in ID3v2.\n");
}

//Opens an MP3 file and calls edit_id3v2 to modify the specified tag.
void mp3_edit(char *filename, char *option, char *value) 
{
    printf("-----------------------------------Select Edit option---------------------------\n");
    // Display specific tag being edited (for user clarity)
    if (strcmp(option, "-t") == 0)
    {
        printf("------------------------------Select Title change option---------------------------\n");
        printf("Title   : %s\n", value);
        printf("------------------------------Title changed successfully------------------------\n");
    }
    else if (strcmp(option, "-a") == 0)
    {
        printf("------------------------------Select Artist change option------------------------\n");
        printf("Artist  : %s\n", value);
        printf("------------------------------Artist changed successfully-----------------------\n");
    }
    else if (strcmp(option, "-A") == 0)
    {
        printf("------------------------------Select Album change option-------------------------\n");
        printf("Album   : %s\n", value);
        printf("------------------------------Album changed successfully------------------------\n");
    }
    else if (strcmp(option, "-y") == 0)
    {
        printf("------------------------------Select Year change option--------------------------\n");
        printf("year    : %s\n", value);
        printf("------------------------------Year changed successfully-------------------------\n");
    }
    else if (strcmp(option, "-c") == 0)
    {
        printf("------------------------------Select Comment change option-----------------------\n");
        printf("Comment : %s\n", value);
        printf("------------------------------Comment changed successfully----------------------\n");
    }
    else if (strcmp(option, "-g") == 0)
    {
        printf("------------------------------Select Genre change option-------------------------\n");
        printf("Genre   : %s\n", value);
        printf("------------------------------Genre changed successfully------------------------\n");
    }

    // open file in read/write binary mode
    FILE *fp = fopen(filename, "r+b");
    if (!fp) 
    {
        printf("Error: Cannot open file.\n");
        return;
    }

    edit_id3v2(fp, option, value);
    fclose(fp);
}