#include "mp3_header.h"

#define VERSION "ID3V2"

int main(int argc, char *argv[])
{
    // Check if user has provided at least one argument
    if (argc < 2)
    {
        printf("----------------------------------------------------------------------------------\n");
        printf(RED"Error : ./mp3reader : Invalid argument\n"RESET);
        print_usage();
        // Print usage instructions if no argument given
        return 0;
    }
    //VIEW TAGS 
    if (strcmp(argv[1], "-v") == 0)
    {
        if (argc < 3)
        {
            printf("Error: Missing arguments for view.\n");
            printf("Usage: ./mp3reader -v <filename> ");
            return 0;
        }

        if (argc > 3)
        {
            printf("Error: Too many arguments\n");
            printf("Usage: ./mp3reader -v <filename> ");
            return 0;
        }


        // Call function to display ID3v2 tag details
        mp3_view(argv[2]);
    }
    //EDIT TAGS 
    else if (strcmp(argv[1], "-e") == 0)
    {
             // Check for minimum arguments: ./mp3reader -e -t/-a/-A/-y/-c/-g "value" file.mp3
        if (argc < 5)
        {
            printf("Error: Missing arguments for edit.\n");
            printf("Usage: ./mp3reader -e -t/-a/-A/-y/-c/-g \"value\" <file>\n");
            return 0;
        }
        if(argc > 5)
        {
            printf("Error: Too many arguments\n");
            printf("Usage: ./mp3reader -e -t/-a/-A/-y/-c/-g \"value\" <file>\n");
            return 0;
        }

        // Tag option to edit (e.g., -t, -a, etc.)
        char *option = argv[2];
        // New value to update
        char *value = argv[3];
        // MP3 file name
        char *filename = argv[4];

        // Validate option
        if (strcmp(option, "-t") != 0 && strcmp(option, "-a") != 0 &&
            strcmp(option, "-A") != 0 && strcmp(option, "-y") != 0 &&
            strcmp(option, "-c") != 0 && strcmp(option, "-g") != 0)
        {
            printf("Error: Invalid tag option '%s'\n", option);
            print_usage();
            return 0;
        }
         // Call function to edit the tag
        mp3_edit(filename, option, value); 
    }
    //help option
    else if (strcmp(argv[1], "-help") == 0)
    {
        print_help();
    }
    //version info
    else if (strcmp(argv[1], "-version") == 0)
    {
        printf("MP3 Tag Reader Version %s\n", VERSION);
    }
    //invalid argument
    else
    {
        printf("----------------------------------------------------------------------------------\n");
        //printf("Error: Invalid argument '%s'\n", argv[1]);
        printf("Error : %s :  Invalid argument\n", argv[1]);
        print_usage();
    }

    return 0;
}
//Function to display correct usage of the program *
void print_usage()
{
    printf("Usage : \n");
    printf("./mp3reader -v <filename.mp3>                      : View ID3v2 tags\n");
    printf("./mp3reader -e -t/-a/-A/-y/-c/-g \"value\" <filename.mp3> : Edit specific tag\n");
    printf("----------------------------------------------------------------------------------\n");
}
//Function to display detailed help message
void print_help()
{
    printf("----------------------------------------------------------------------------------\n");
    printf("                MP3 TAG READER HELP MENU\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("  -v <file>                View ID3v2 tags in MP3 file\n");
    printf("  -e <options> <file>      Edit specific tag\n");
    printf("     Options:\n");
    printf("       -t   Edit Title\n");
    printf("       -a   Edit Artist\n");
    printf("       -A   Edit Album\n");
    printf("       -y   Edit Year\n");
    printf("       -c   Edit Comment\n");
    printf("       -g   Edit Genre\n");
    printf("Format-Hex <filename.mp3> | Select-Object -First 10  : Show hexadecimal view of ID3v2 tag\n");
    printf("  -help                   Show this help message\n");
    printf("  -version                Show version information\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("Example:\n");
    printf("  ./mp3reader -v song.mp3\n");
    printf("  ./mp3reader -e -t \"New Title\" song.mp3\n");
    printf("  ./mp3reader -e -c \"New Comment\" song.mp3\n");
    printf("  ./mp3reader -e -g \"Pop\" song.mp3\n");
    printf("----------------------------------------------------------------------------------\n");
}
