#ifndef MP3_HEADER_H
#define MP3_HEADER_H

#define RESET       "\033[0m"
#define RED         "\033[0;31m"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void mp3_view(char *filename);
void edit_id3v2(FILE *fp, char *option, char *value);
void mp3_edit(char *filename, char *option, char *value); 

void print_usage();
void print_help();

#endif