#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stddef.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLORGREEN   "\033[1;32m"
#define ANSI_COLORBLUE    "\033[1;34m"

// ----------------- Structure declaration --------------- //
// single linked list 
typedef struct stoped_jobs
{
    char command[50];
    int pid;
    struct stoped_jobs *link;
}s_jobs;


void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);//

void copy_change(char *prompt, char *input_string);

int check_command_type(char *command);//
void execute_internal_commands(char *cmd, char *input_string);//
void my_handler(int sig_num);
void extract_external_commands(char **external_commands);//

void execute_external_commands(int *pipe_count, char **execute);
void n_pipe(int pipe_count, char *argv[]);


// ----------------- Linked list operations ---------------- //
void insert_jobs_to_list(s_jobs **head);
void print_jobs(s_jobs *head);
void delete_jobs_from_list(s_jobs **head);

#endif