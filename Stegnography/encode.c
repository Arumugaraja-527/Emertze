#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int) , 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    // Find the size of secret file data
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    char *ext;

    /* ---------- Step 1: Validate Source Image File ---------- */
    ext = strrchr(argv[2], '.');                            // Find last '.'

    if (ext && strcmp(ext, ".bmp") == 0 && ext != argv[2])  // Ensure at least 1 char before '.'
    {
        printf("Source image file validated: %s\n", argv[2]);
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        printf("Error: Source image must be a .bmp file with valid name\n");
        return e_failure;
    }

    /* ---------- Step 2: Validate Secret File ---------- */
    if (argv[3] != NULL)
    {
        ext = strrchr(argv[3], '.');
        if (ext && ext != argv[3] && (strcmp(ext, ".txt") == 0 || strcmp(ext, ".c") == 0 || strcmp(ext, ".h") == 0 || strcmp(ext, ".sh") == 0))
        {
            printf("Secret file validated: %s\n", argv[3]);
            encInfo->secret_fname = argv[3];
            strcpy(encInfo->extn_secret_file, ext); // store file extension (like ".txt")
        }
        else
        {
            printf("Error: Secret file must be .txt, .c, .h or .sh\n");
            return e_failure;
        }
    }
    else
    {
        printf("Error: Please pass a secret file\n");
        return e_failure;
    }

    /* ---------- Step 3: Validate or Assign Stego Image File ---------- */
    if (argv[4] != NULL)
    {
        ext = strrchr(argv[4], '.');
        if (ext && strcmp(ext, ".bmp") == 0 && ext != argv[4])
        {
            printf("Output stego image file validated: %s\n", argv[4]);
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            printf("Error: Output file must have .bmp extension\n");
            return e_failure;
        }
    }
    else
    {
        // Default output filename
        encInfo->stego_image_fname = "stego_img.bmp";
        printf("No output file specified, defaulting to: stego_img.bmp\n");
    }

    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

//**************************************************************** */
// 1. Ask user to enter a magic string (like a password marker).
// 2️. Find its length and store it for encoding.
// 3️. Extract the extension (like .txt) from the secret file name.
// 4️.  Find the length of that extension.
// 5️. Get the total size of the secret file in bytes.
// 6️. Get total size of the source BMP image file.
// 7️. Calculate total bytes to encode: 
//     (magic string + its length + secret extension + its length + secret data + their sizes)
// 8️. Multiply by 8 because each byte of secret data takes 8 image bytes (1 bit per byte).
// 9️. Subtract 54 bytes (BMP header) since header can’t be used for hiding data.
// 10.Compare if (required bits) ≤ (available image bytes - 54).
//       If yes → return e_success (enough capacity to hide data)
//       If no  → return e_failure (not enough image space)
//********************************************************************/


Status check_capacity(EncodeInfo *encInfo)
{
    strcpy(encInfo->magic, MAGIC_STRING);
    encInfo->magic_str_len = strlen(MAGIC_STRING);

    printf("Length is : %d\n", encInfo->magic_str_len);
    int sizeof_magic_len = sizeof(encInfo->magic_str_len);
    printf("sizeof Magic string is : %d\n", sizeof_magic_len );
    
    char *secret_ext = strstr(encInfo->secret_fname, ".");
    for(int i = 0; i < MAX_FILE_SUFFIX; i++)
    {
        encInfo->extn_secret_file[i] = secret_ext[i];
    }
    printf("Extension of Secret file: %s\n", encInfo->extn_secret_file);
    encInfo->ext_size = strlen(secret_ext);
    printf("Length of secret extension is: %d\n", encInfo->ext_size);
    
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    printf("Size of Secret data : %ld\n", encInfo->size_secret_file);
    printf("Size of Secret file : %ld\n", sizeof(encInfo->size_secret_file));

    int file_size = get_image_size_for_bmp(encInfo->fptr_src_image);
    printf("Size Of Source image : %d\n", file_size);

    int bytes_encode;
    bytes_encode= sizeof_magic_len + encInfo->magic_str_len + encInfo->ext_size +
        encInfo->size_secret_file  + sizeof(encInfo->size_secret_file) + sizeof(encInfo->ext_size);
    printf("Capacity checking : %d\n", bytes_encode * 8);

    if((bytes_encode*8) <= file_size - 54)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    } 

}

/*******************************************************
 * Function: copy_bmp_header
 * Purpose : Copies the 54-byte BMP header from source image
 *            to destination (stego) image without modification.
 * Explanation:
 *   - Every BMP file starts with a fixed 54-byte header.
 *   - This header contains metadata (width, height, color depth, etc.)
 *   - It must be copied unchanged before we modify pixel data.
 *******************************************************/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);              // Move file pointer to start (position 0)
    
    char buffer[54];                     // BMP header = 54 bytes
    fread(buffer, 54, 1, fptr_src_image);  // Read 54 bytes from source image
    fwrite(buffer, 54, 1, fptr_dest_image); // Write same 54 bytes to stego image

    int res = ftell(fptr_dest_image);    // Verify position now at byte 54
    if (res == 54)
        return e_success;
    else
        return e_failure;
}


/*******************************************************
 * Function: encode_magic_string_len
 * Purpose : Encodes the length of the magic string (# of chars)
 * Explanation:
 *   - Magic string length is an integer (4 bytes = 32 bits)
 *   - Since we hide 1 bit in 1 byte of image data (LSB method),
 *     we need 32 image bytes to store 32 bits.
 *   - These 32 bytes are read from the source image,
 *     modified, and written to the stego image.
 *******************************************************/
Status encode_magic_string_len(int len, EncodeInfo *encInfo)
{
    char buffer[32];  // 32 image bytes used to hide 32 bits of 'len'
    fread(buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(len, buffer);      // Store integer bit-by-bit in LSBs
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}


/*******************************************************
 * Function: encode_magic_string
 * Purpose : Encodes the actual magic string (like "#*")
 * Explanation:
 *   - Each character = 8 bits
 *   - Each bit hidden in 1 image byte (LSB)
 *   - Therefore, 8 bytes of image data are used per character.
 *   - If magic string length = 2 (#*), total used = 2 × 8 = 16 bytes.
 *******************************************************/
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    for (int i = 0; i < encInfo->magic_str_len; i++)
    {
        char buffer[8];  // 8 image bytes for 1 character
        fread(buffer, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i], buffer);
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}


/*******************************************************
 * Function: encode_secret_file_extn_size
 * Purpose : Encodes the size (length) of the file extension (.txt)
 * Explanation:
 *   - The extension size is an integer (e.g., 4 for ".txt")
 *   - Integer = 32 bits → needs 32 image bytes.
 *   - 32 bytes from the image are read and modified.
 *******************************************************/
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char buffer[32];  
    fread(buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(size, buffer);      // Store integer bitwise
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}


/*******************************************************
 * Function: encode_secret_file_extn
 * Purpose : Encodes the actual file extension characters (.txt)
 * Explanation:
 *   - Each extension character = 8 bits
 *   - Each bit stored in 1 image byte
 *   - Therefore, for ".txt" (4 chars) → 4 × 8 = 32 image bytes used.
 *******************************************************/
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    for (int i = 0; i < encInfo->ext_size; i++)
    {
        char buffer[8];  // 8 bytes of image per character
        fread(buffer, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(file_extn[i], buffer);
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}


/*******************************************************
 * Function: encode_secret_file_size
 * Purpose : Encodes the size (in bytes) of the secret file
 * Explanation:
 *   - File size is stored as a long integer (4 bytes = 32 bits)
 *   - Need 32 image bytes to hide these 32 bits.
 *   - These 32 bytes from the image are modified and written.
 *******************************************************/
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];  // 32 bytes of image → 32 bits for integer
    fread(buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(file_size, buffer);
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch;
    while(fread(&ch, 1, 1, encInfo->fptr_secret) == 1)
    {
        char buffer[8];
        if(fread(buffer, 1, 8, encInfo->fptr_src_image) < 8)
        {
            return e_failure;
        }
        encode_byte_to_lsb(ch, buffer);
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    int ch;
    while ((ch = fgetc(fptr_src)) != EOF)
    {
        fputc(ch, fptr_dest);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
   int i, n = 7;

   for (i = 0; i <= 7; i++)
   {
        int mask = 1 << n;
        int get = data & mask;
        image_buffer[i] = image_buffer[i] & ~1;
        get = (get >> n);
        image_buffer[i] = image_buffer[i] | get;
        n--;
   }
   return e_success;
}

Status encode_size_to_lsb(int size, char *imageBuffer)
{
   int i, n = 32;
   
   for(i = 0; i < 32; i++)
   {
        int mask = 1 << n;
        int get = size & mask;
        imageBuffer[i] = imageBuffer[i] & ~1;
        get = (get >> n);
        imageBuffer[i] = imageBuffer[i] | get;
        n--;
   }
   return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_failure)
    {
        printf("File opening failed!\n");
        return e_failure;
    }

    if (check_capacity(encInfo) == e_failure)
    {
        printf("Check capacity is failed!\n");
        return e_failure;
    }
    printf("Check capacity is success!\n");

    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("BMP header not copied successfully\n");
        return e_failure;
    }
    printf("BMP header is copied successfully\n");

    if (encode_magic_string_len(encInfo->magic_str_len, encInfo) == e_failure)
    {
        printf("Magic string length is not encoded\n");
        return e_failure;
    }
    printf("Magic string length is copied successfully\n");

    if (encode_magic_string(encInfo->magic, encInfo) == e_failure)
    {
        printf("Magic string is not encoded\n");
        return e_failure;
    }
    printf("Magic string data is copied successfully\n");

    if (encode_secret_file_extn_size(encInfo->ext_size, encInfo) == e_failure)
    {
        printf("Secret file Extension length is not encoded\n");
        return e_failure;
    }
    printf("Secret file extension length is copied successfully\n");

    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        printf("Secret file Extension is not encoded\n");
        return e_failure;
    }
    printf("Secret file extension is copied successfully\n");

    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        printf("Secret file size is not encoded\n");
        return e_failure;
    }
    printf("Secret file size is copied successfully\n");

    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("Secret file data is not encoded\n");
        return e_failure;
    }
    printf("Secret file data is copied successfully\n");

    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("Remaining image data is not encoded\n");
        return e_failure;
    }
    printf("Remaining image data is copied successfully\n");

    return e_success;
}
