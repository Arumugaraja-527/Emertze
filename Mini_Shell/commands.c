#include "header.h"

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",

					"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                    
					"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};


char *ext_cmds[153];      // array of pointers
extern char input_string[50];
extern int pid;
extern char input_string[50];
extern char prompt[20];
extern int status;
s_jobs *list = NULL;


// function for getting command from the input, commands are characters upto space
char *get_command(char *input_string)
{
    //finding the length to allocate memory dynamically
    int len = strlen(input_string) + 1;

    char *cmd = malloc(len * sizeof(char));  // malloc(len)
    if(cmd == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    int i;
    for( i = 0; input_string[i] ; i++)
    {    
         //if the character is space or null break the loop that is command
        if(input_string[i] == ' ' || input_string[i] == '\0')
        {
            break;
        }
        // stores the character in the cmd array
        cmd[i] = input_string[i];
    }

    cmd[i] = '\0';
    
    return cmd;
}

// function for checking the type of the command // 
int check_command_type(char *command)
{
	// checking for the Builtin commands //
	for(int i=0; builtins[i]; i++)
	{
		if(strcmp(builtins[i], command) == 0)
		{
			return BUILTIN;
		}
	}
	// checking for the external commands //
	for(int i=0; ext_cmds[i]; i++)
	{
		if(strcmp(ext_cmds[i], command) == 0)
		{
			return EXTERNAL;
		}
	}
	// if it not fall in both external and internal //
	return NO_COMMAND;
}

//functions for extracting commands from the file annd store it in the 2D array
void extract_external_commands(char **ext_cmds)
{
    int fd = open("ext_cmd.txt", O_RDONLY);
    if(fd == -1)
    {
        printf("File opening failes!\n");
        return;
    }

    char ch;
    char str[50];
    int i = 0, j = 0;
     
    // running loop till end of the loop
    while( read(fd, &ch, 1) > 0 )
    {
        //if it is not next line then storing ch into the string
       if(ch != '\n' && ch != '\r')   //carriage return 
          str[i++] = ch;
    
       else
       {
          if(i > 0)     // this block is for newline handling 
          {
            // storing null character at the end
            str[i] = '\0';
          //allocate the columns that is len of each cmd dynamically
          int len = strlen(str) + 1;
          ext_cmds[j] = malloc(len * sizeof(char));

          strcpy(ext_cmds[j], str);
          j++;
          i = 0;
          }
       }   
    }
    if(i>0) // this same block is for EOF handling or space inbetween commands in the file
	{
		// storing the null character at the end //
		str[i] = '\0';
		// finding the length to allocate the memory dynamically //
		int len = strlen(str) + 1;
		// allocating the memory dynamically //
		ext_cmds[j] = malloc(len*sizeof(char));
		// copying the string to that address //
		strcpy(ext_cmds[j],str);
		// updating the values //
		j++;
	}
	// Inserting the NULL address at last //
	ext_cmds[j] = NULL;
}

void execute_internal_commands(char *cmd, char* input_string)
{
	// Checking the input is exit //
	if(strcmp(cmd,"exit") == 0)
	{
		// to exit the process //
		exit(0);
	}

	// Checking the input is pwd //
	else if(strcmp(cmd,"pwd") == 0)
	{
		// string to store pwd //
		char pwd[100];
		// getting the pwd to the string using getwcd system call //
		getcwd(pwd,100);
		// printing the pwd by using the string //
		printf(ANSI_COLOR_CYAN "%s\n" ANSI_COLOR_RESET, pwd);
	}

	// Checking the input is cd //
	else if(strcmp(cmd,"cd") == 0)
	{
		// by using chdir function we are changing the directory //
		if( chdir(input_string + 3) == -1) // returns 0 success and -1 failure chdir(pathname)
		{
			printf( ANSI_COLOR_RED"Invalid Directory\n"ANSI_COLOR_RESET);
		}
	}
    // Checking the input is echo or not //
	else if(strcmp(cmd,"echo") == 0)
	{
		// $$ for printing the PID of the shell //
		if(strstr(input_string + 5, "$$"))
		{
			printf("%d\n",getpid());
		}

		// $? for printing the exit status of the last executed process //
		else if(strstr(input_string + 5, "$?"))
		{
			if (WIFEXITED(status))
            {
               printf("%d\n", WEXITSTATUS(status));
            }
           else if (WIFSIGNALED(status))
            {
              printf("%d\n", 128 + WTERMSIG(status));    // to get the original number as terminal we added 128
            }
            else if (WIFSTOPPED(status))
            { 
              printf("%d\n", 128 + WSTOPSIG(status));
            }
		}

		// $SHELL for printing the Executable path of the shell //
		else if(strstr(input_string + 5, "$SHELL"))
		{
			char env[100];
			strcpy(env, getenv("SHELL"));  // getenv system call
			printf("%s\n",env);
		}
	}
}

//Function for the converting input to 2D array
void execute_external_commands(int *pipe_count, char **execute)
{
    char str[20];
	int i = 0, k = 0;
	// running the loop till NULL character.
	for(int j = 0; input_string[j]; j++)
	{
		// checking for the pipe //
		if(input_string[j] == '|')
			// Incrementing the pipe count //
			(*pipe_count)++;

		// if the character is not space and null chaaracter then storing the character into the string //
		if(input_string[j] != ' ' && input_string[j] != '\0')
		{
			str[i++] = input_string[j];
		}
		else  // if we get space
		{
			if(i>0)
			{
				// if character is space then stroing null character at last //
				str[i++] = '\0';
				// Allocating memory dynamically for each string //
				execute[k] = malloc(strlen(str)+1);
				// copying that string to that DMA address //
				strcpy(execute[k], str);
				// updating the values of i,k //
				k++;
				i=0;
			}
		}
	} 
    // for last word we dont have the space so do it separetly
	if(i>0)
	{
		// if character is space then stroing null character at last //
		str[i++] = '\0';
		// Allocating memory dynamically for last string //
		execute[k] = malloc(strlen(str)+1);
		// copying that string to that DMA address //
		strcpy(execute[k], str);
		// updating the values of i,k //
		k++;
		i=0;
	}
	// At last adding NULL address into 2D array //
	execute[k] = NULL;
}

//n pipe logic in separate function
void n_pipe(int pipe_count, char *argv[])
{
    int count = 0;
    int pipe_ind[pipe_count + 1]; // starting index of each command

    pipe_ind[count++] = 0;

    /* Split argv on '|' */
    for (int i = 0; argv[i]; i++)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            argv[i] = NULL;   // if the pipe we get then the next is the command that is i +1
            pipe_ind[count++] = i + 1;
        }
    }

    /* Validate pipe usage 
    eg :
    | ls
    ls |
    ls || wc
    argv = {"ls","|",NULL}
    pipe_ind = {0, 2} index 0 ,1
    argv[2] == NULL   -> index 2 is NULL then how?
    invalid pipe usage
    */

    for (int i = 0; i < count; i++)
    {
        if (argv[pipe_ind[i]] == NULL)
        {
            printf( ANSI_COLOR_RED "Invalid use of pipe\n" ANSI_COLOR_RESET);
            return;
        }
    }

    int prev_fd = -1;   // previous pipe read end

    for (int i = 0; i < count; i++)
    {
        int pipefd[2];

        if (i < pipe_count)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                return;
            }
        }

        int pid = fork();

        if (pid == 0)   // -------- CHILD --------
        {
            /* stdin from previous pipe */
            if (prev_fd != -1)
            {
                dup2(prev_fd, 0);
                close(prev_fd);
            }

            /* stdout to next pipe */
            if (i < pipe_count)
            {
                dup2(pipefd[1], 1);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            execvp(argv[pipe_ind[i]], argv + pipe_ind[i]);
            perror("execvp");
            exit(1);
        }
        else if (pid > 0)   // -------- PARENT --------
        {
            if (prev_fd != -1)
                close(prev_fd);

            if (i < pipe_count)
            {
                close(pipefd[1]);     // parent never writes
                prev_fd = pipefd[0];  // save read end
            }
        }
        else
        {
            perror("fork");
            return;
        }
    }

    /* Wait for all children */
    for (int i = 0; i < count; i++)
        wait(NULL);

}

// Function for signal handler //
void my_handler(int signum)
{
	// task when signal is SIGINT //
	if(signum == SIGINT)
	{
		if(pid == 0)
		{
		   char cwd[500];
           getcwd(cwd, sizeof(cwd));

           if (strcmp(prompt, "Minishell$:~") == 0)
           {
               // default prompt → show path
                printf(ANSI_COLOR_GREEN"\n%s"ANSI_COLOR_RESET, prompt );
                printf(ANSI_COLOR_BLUE"%s$ "ANSI_COLOR_RESET, cwd);
            }
           else
           {
               // PS1 changed → show only PS1
               printf(ANSI_COLOR_GREEN"%s "ANSI_COLOR_RESET, prompt);
            }	
		}
		//printf(ANSI_COLOR_GREEN"\n%s "ANSI_COLOR_RESET, prompt);
		fflush(stdout);
	}
	// task when the signal is SIGTSTP //
	else if (signum == SIGTSTP)
	{
		// when task is not executing //
		if(pid == 0)
		{
			char cwd[500];
           getcwd(cwd, sizeof(cwd));

           if (strcmp(prompt, "Minishell$:~") == 0)
           {
               // default prompt → show path
                printf(ANSI_COLOR_GREEN"\n%s"ANSI_COLOR_RESET, prompt );
                printf(ANSI_COLOR_BLUE"%s$ "ANSI_COLOR_RESET, cwd);
				fflush(stdout);
            }
           else
           {
               // PS1 changed → show only PS1
               printf(ANSI_COLOR_GREEN"%s "ANSI_COLOR_RESET, prompt);
			   fflush(stdout);
            }	
		}
	
		// when task is executing //
		else if(pid > 0)
		{
			insert_jobs_to_list(&list);
		}
	}
    //task when signal is SIGCHILD
    else if(signum == SIGCHLD)
    {
        waitpid(-1 , &status, WNOHANG); // non blocking checks for any child to retrun
    }
 
}
// Function for inserting input and command to list when signal is SIGTSTP //
void insert_jobs_to_list(s_jobs **head) // insert at first
{
	// Creating NODE.
	s_jobs *new = malloc(sizeof(s_jobs));
	if(new == NULL)
	return ;
	// Updating data to node //
	new -> pid = pid;
	strcpy(new -> command, input_string);
	new -> link = NULL;
	// Updating the link part //
	if(*head == NULL)
	{
		*head = new;
	}
	else
	{
		new -> link = *head;
		*head = new;
	}
	return;
}

// Function for Printing the jobs //
void print_jobs(s_jobs *head)
{
	s_jobs *temp = head;
	int i=1;
	// Running loop till end //
	while(temp != NULL)
	{
		printf("[%d]      %d       Stopped   %s\n",i,temp->pid,temp->command);
		temp = temp ->link;
		i++;
	}
}

// Function for removing completed jobs from the list //
void delete_jobs_from_list(s_jobs **head) // delete at first
{
	// check for no Jobs // 
	if(*head == NULL)
	{
		return;
	}
	s_jobs *temp = *head;
	*head = temp -> link;
	free(temp);
	return;
}

