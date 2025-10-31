#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"

// Function to decode one byte of data from LSB of 8 bytes in image buffer
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    int i, n = 7;
    char result = 0;

    for(i = 0; i <= 7; i++)
    {
        int bit = image_buffer[i] & 1;
        bit = (bit << n);
        result = result | bit;
        n--;
    }
    *data = result;
    return d_success;
}

// Function to decode 32-bit integer (size) from LSB of 32 bytes in image buffer
Status decode_size_from_lsb(int *data, char *image_buffer)
{

    int i, n = 32;
    int result = 0;

    for(i = 0; i < 32; i++)
    {
        int bit = image_buffer[i] & 1;
        bit = (bit << n);
        result = result | bit;
        n--;
    }

    *data = result;
    return d_success;
}

// Function to open stego image file and skip BMP header (54 bytes)
Status decode_open_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if(decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return d_failure;
    }
    printf("Position of offset in stego file is : %ld\n", ftell(decInfo->fptr_stego_image));
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    printf("Position of offset in stego file is : %ld\n", ftell(decInfo->fptr_stego_image));
    return d_success;
}

// Function to decode magic string length from LSB of 32 bytes
Status decode_magic_string_len(int *len, DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer, 1, 32, decInfo->fptr_stego_image);
    decode_size_from_lsb(len, buffer);
    decInfo->magic_str_len = *len;
    printf("Magic string length is : %d\n", decInfo->magic_str_len);
    return d_success;
}

// Function to decode magic string character by character from LSB
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    for(int i = 0; i < (decInfo->magic_str_len); i++)
    {
        char buffer[8];
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&decInfo->decoded_magic[i], buffer);
    }
    decInfo->decoded_magic[decInfo->magic_str_len] = '\0';  
    //printf("Magic_string is : %s\n", decInfo->decoded_magic);
    return d_success;
}

// Function to validate decoded magic string against predefined MAGIC_STRING
Status validate_magic_string(DecodeInfo *decInfo)
{
    if(strcmp(MAGIC_STRING, decInfo->decoded_magic) != 0)
    {
        printf("Magic string mismatch! Not a valid stego file.\n");
        return d_failure;
    }
    printf("Magic string validated successfully!\n");
    return d_success;
}

// Function to decode secret file extension length from LSB of 32 bytes
Status decode_secret_file_extn_len(int *len, DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer, 1, 32, decInfo->fptr_stego_image);
    decode_size_from_lsb(len, buffer);
    decInfo->de_ext_size = *len;
    printf("Extension length is : %d\n", decInfo->de_ext_size);
    return d_success;
}

// Function to decode secret file extension character by character from LSB
Status decode_secret_file_extn(const char *file_extn, DecodeInfo *decInfo)
{
    char output[50];
    for(int i = 0; i < (decInfo->de_ext_size); i++)
    {
        char buffer[8];
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&output[i], buffer);
    }
    output[decInfo->de_ext_size] = '\0'; 
    decInfo->secret_fname = output;
    printf("Secret file Extenxion is : %s\n", decInfo->secret_fname);
    strcat(decInfo->output_fname, decInfo->secret_fname); 
    decInfo->fptr_output_file = fopen(decInfo->output_fname, "w");
    if(decInfo->fptr_output_file == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);
        return d_failure;
    }
    printf("Output file created : %s\n", decInfo->output_fname);
    return d_success;
}

// Function to decode secret file size from LSB of 32 bytes
Status decode_secret_file_size(int file_size, DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer, 1, 32, decInfo->fptr_stego_image);
    decode_size_from_lsb(&file_size, buffer);
    decInfo->size_secret_file = file_size;
    printf("Secret file size is : %d\n", decInfo->size_secret_file);
    return d_success;
} 

// Function to decode secret file data byte by byte from LSB
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char ch;
    for(int i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&ch, buffer);
        fputc(ch, decInfo->fptr_output_file);
    }
   return d_success;
}

// Main decoding function that orchestrates the entire decoding process
Status do_decoding(DecodeInfo *decInfo)
{
    if (decode_open_files(decInfo) == d_failure)
    {
        printf("File opening failed!\n");
        return d_failure;
    }

    if (decode_magic_string_len(&decInfo->magic_str_len, decInfo) == d_failure)
    {
        printf("Magic string length is not decoded\n");
        return d_failure;
    }

    if (decode_magic_string(decInfo->decoded_magic, decInfo) == d_failure)
    {
        printf("Magic string is not decoded\n");
        return d_failure;   
    }
    if(validate_magic_string(decInfo) == d_failure)
    {
        printf("Authentication failed.Try again?\n");
        return d_failure;
    }

    if (decode_secret_file_extn_len(&decInfo->de_ext_size, decInfo) == d_failure)
    {
        printf("Extension length is not decoded\n");
        return d_failure;   
    }

    if (decode_secret_file_extn(decInfo->secret_fname, decInfo) == d_failure)
    {
        printf("Extension is not decoded\n");
        return d_failure;   
    }

    if (decode_secret_file_size(decInfo->size_secret_file, decInfo) == d_failure)
    {
        printf("Secret file size is not decoded\n");
        return d_failure;   
    }

    if (decode_secret_file_data(decInfo) == d_failure)
    {
        printf("Secret file data is not decoded\n");
        return d_failure;   
    }
    printf("Secret file data is decoded successfully\n");

    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output_file);

    return d_success; 
}

// Function to read and validate command line arguments for decoding
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char temp[50];
    const char default_name[] = "decoded_output";

    printf("[DEBUG] Entering read_and_validate_decode_args()\n");

    // 1️. Validate stego image argument
    if (argv[2] && strstr(argv[2], ".bmp") != NULL)
    {
        printf(".bmp file is present\n");
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("[DEBUG] Either argv[2] is NULL or doesn't contain .bmp\n");
        return d_failure;
    }

    // 2️. Handle optional output file name
    if (argv[3] == NULL)
    {
        strcpy(temp, default_name);
        printf("No output file name provided. Using default: %s\n", temp);
    }
    else
    {
        strcpy(temp, argv[3]);
        printf("[DEBUG] Output arg provided: %s\n", temp);
    }

    // 3️. Remove any extension if present
    for (int i = 0; temp[i] != '\0'; i++)
    {
        if (temp[i] == '.')
        {
            temp[i] = '\0';
            break;
        }
    }

    strcpy(decInfo->output_fname, temp);
    printf("File name is : %s\n", decInfo->output_fname);

    // 4️. Try opening the stego image
    printf("[DEBUG] Attempting to open stego image: %s\n", decInfo->stego_image_fname);
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return d_failure;
    }

    printf("[DEBUG] File opened successfully!\n");
    return d_success;
}