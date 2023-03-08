#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int check_command_type (char *cmd);
void execute_shell_built_in (char *argv[3], char *cmd);
void execute_command (char *argv[3], char *cmd);
void child_handler();

char in_background = 0;
FILE* log_file;

int main()
{
    /* A signal is a software generated interrupt that is sent to a process by the OS
    SIGCHLD is number of the signal sent to the parent process when child terminates
    when SIGCHLD signal is received, the child_handler function is executed*/
    signal(SIGCHLD, child_handler);

    size_t size = 10;
    char *input, *cmd, *parameter;
    int n = 0;
    char *argv[3];

    // Clear the previous data
    remove("Process_Log.txt");
    log_file = fopen ("Process_Log.txt","a");
    fclose(log_file);

    while(1){
        in_background=0;
        n = 0;
        printf ("$>> ");

        // Take user input
        input = (char *) malloc(size);
        getline (&input, &size, stdin);

        // Split input line into seprate strings (arguments list)
        parameter = strtok(input, " ");
        while (parameter != NULL) {
            if (n == 0){
                cmd = parameter;
            }
            if(strcmp(parameter,"&\n") !=0 ){
                argv[n] = parameter;
                parameter = strtok(NULL, " ");
                n++;
            }
            else{
                in_background = 1;
                break;
            }
        }
        // Add null terminating string and remove \n from last argument
        argv[n] = NULL;
        argv[n-1] = strtok(argv[n-1], "\n");
        if(n == 1)
            cmd = strtok(argv[n-1], "\n");

        /**
        * check if required process is built-in or not
        * 1: means built-in
        * 0: means required to be executed
        */
        int input_type = check_command_type(cmd);
        switch(input_type){
            case 1:
                execute_shell_built_in(argv, cmd);
                break;
            case 0:
                execute_command(argv, cmd);
                break;
        }
    }
    return 0;
}

int check_command_type(char *cmd){
    if(strcmp(cmd, "exit") == 0 || strcmp(cmd, "cd") == 0 || strcmp(cmd, "pwd") ==0 || strcmp(cmd, "export") == 0
        || strcmp(cmd, "echo") == 0)
        return 1;
    else
        return 0;
}

void execute_shell_built_in(char *argv[3], char *cmd)
{
    // exit command
    if (strcmp(cmd, "exit") == 0){
        exit(EXIT_SUCCESS);
    }

    // cd command
    else if (strcmp(cmd, "cd") == 0){
        // chdir returns 0 in case of success and returns -1 in case of error
        if (chdir(argv[1]) != 0){
            printf("Error changing directory\n");
        }
    }

    // pwd command
    else if (strcmp(cmd, "pwd") == 0){
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("Dir: %s\n", cwd);
    }

    // echo command
    else if (strcmp(cmd, "echo") == 0){
        char x[] = " ";
        strcat (x,argv[1]);
        strcat(cmd, x);
        system(cmd);
    }

    // export command
    else if(strcmp(cmd, "export") == 0){
        char deli[2] = "\"";
        char *ret;
        ret = strtok(argv[1],deli);
        char *value;
        value = strtok(0,deli);
        strcat(ret,value);
        putenv(ret);
    }
}

void execute_command(char *argv[3], char *cmd)
{
        pid_t child_pid;
        int status;
        child_pid = fork();

        /* fork() return value can be:
        Zero: Returned to the newly created child process.
        Negative Value: creation of a child process was unsuccessful.
        Positive value: Returned to parent */

        if(child_pid == 0){
            // execvp returns -1 in case of failed execution
            if(execvp(cmd,argv) == -1)
                printf("Wrong Command\n");
        }

        else if (child_pid == -1){
            printf("Error occured (can't fork)\n");
            exit(EXIT_FAILURE);
        }

        else if (child_pid > 0){
            if(in_background == 1){
                // return immediately (don't wait for the child process to terminate)
                waitpid(child_pid,&status,WNOHANG);
            }

            else{
                waitpid(child_pid,&status,0);   // wait till the child process terminates
                log_file = fopen ("Process_Log.txt","a");
                fprintf (log_file, "child process was terminated pid = %d \n",child_pid);
                fclose(log_file);
            }
        }
}

// Handler function executed when a child process in the background is terminated
void child_handler(int sig)
{
    int status;
    pid_t pid;

   while((pid = waitpid(-1 , &status , WUNTRACED | WNOHANG| WCONTINUED)) > 0)
    {
        if(WIFEXITED(status))
        {
            log_file = fopen ("Process_Log.txt","a");
            fprintf (log_file, "child process was terminated pid = %d \n",pid);
            fclose(log_file);
        }
    }
}
