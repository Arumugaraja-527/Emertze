#include "header.h"

//Global variables
int pid = 0;
extern char *ext_cmds[153]; 
extern char input_string[50];
extern char prompt[20];
int status;
extern s_jobs *list;

void scan_input(char *prompt, char *input_string)
{
  
    //calling the function to extract commands from the file
    extract_external_commands(ext_cmds);
    
    // Registering the Signals with own handler // 
	signal(SIGINT, my_handler);
	signal(SIGTSTP, my_handler);
    signal(SIGCHLD, my_handler);
	
    while (1)
    {
       // printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, prompt);
      char cwd[500];
      getcwd(cwd, sizeof(cwd));

     if (strcmp(prompt, "Minishell$:~") == 0)
     {
        // default prompt → show path
       printf(ANSI_COLOR_GREEN"%s"ANSI_COLOR_RESET, prompt );
       printf(ANSI_COLOR_BLUE"%s$ "ANSI_COLOR_RESET, cwd);
     }
     else
     {
      // PS1 changed → show only PS1
      printf(ANSI_COLOR_GREEN"%s "ANSI_COLOR_RESET, prompt);
     }
     fflush(stdout);
        
        scanf("%[^\n]", input_string);
        getchar();  

        // Check PS1=
        if (strncmp(input_string, "PS1=", 4) == 0)
        {
            // Check empty prompt
            if (input_string[4] == '\0')
            {
                printf("ERROR: Invalid Prompt\n");
                continue;
            }

           int space_found = 0;

            // Check for space from index 4 to '\0'
            for (int i = 4; input_string[i] != '\0'; i++)
            {
                if (input_string[i] == ' ')
                {
                    space_found = 1;
                    break;
                }
            }

            if (space_found)
            {
                printf(ANSI_COLOR_RED "ERROR: Invalid Prompt\n"  ANSI_COLOR_RESET);
            }
            else
            {
                strcpy(prompt, input_string + 4);
            }
        }
        else
        {
            //getting the command from the input string
          char *cmd = get_command(input_string);
           //check the commands type

          if(check_command_type(cmd) == BUILTIN)
          {
            execute_internal_commands(cmd, input_string);
          }

          else if(check_command_type(cmd) == EXTERNAL)
          {

             char *execute[sizeof(input_string) / 2];    // inputstring/2 just simply only 100/2 = 50 but commands are only 3 to 4 char
             int pipe_count = 0;

             //converting input string to the 2d array and checking the pipe count
             //why converting to 2D array becoz execvp()
             execute_external_commands(&pipe_count, execute);

             // create child process
             pid = fork();
             if(pid > 0)
             {
                waitpid(pid, &status, WUNTRACED);  // waits for the child to execute 
                pid = 0;
             }
             else if(pid == 0)
             {
                // In child we are setting signals to default
                signal(SIGINT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                //if there is no pipe
                if(pipe_count == 0)
                {
                    execvp(execute[0], execute);
                    exit(0);
                }
                else
                {
                    // calling the n pipe logic
                    n_pipe(pipe_count, execute);
                    exit(0);
                }
             }
             else
             {
                // error message if child process not created
                perror("fork");
             }
          }
          else // if the command notbelongs to internal and external
          {
             //checking the input is jobs or not
            if(strcmp(cmd, "jobs") == 0)
            {
                print_jobs(list); // printing the stopped jobs
            }
            // fore ground
          else if (strcmp(cmd, "fg") == 0)
          {
            if (list != NULL)
            {
              printf("%s\n", list->command);   // show command 
              //continueing the stopped job
              kill(list->pid, SIGCONT);

              pid = list->pid;
              // waiting till the child termination or child stopped
              waitpid(pid, &status, WUNTRACED);
              // reseting the pid to zero
              pid = 0;
              // delete the job from the list when the job is completed fully
             if (WIFEXITED(status) || WIFSIGNALED(status))
             {
               delete_jobs_from_list(&list);
             }

            }
          }

            // checking the input is bg --> background
			else if (strcmp(cmd,"bg") == 0)
			{
				if(list != NULL)
				{
					// Continueing the stopped job //
					kill(list->pid, SIGCONT);
					// Deleting the job from the list after continueing //
                    printf("[%d]+ %s &\n", 1, list->command);
                    delete_jobs_from_list(&list);

				}
			}

            else
            {
                printf(ANSI_COLOR_RED "Command not found!\n" ANSI_COLOR_RESET);
            }
          }
          free(cmd);
        }
    }
}

