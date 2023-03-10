/*
This code is the work of
Yahia Walid Mohamad Eldakhakhny		19016891
Detailed project description is at:
https://github.com/SajedHassan/Operating-Systems/blob/master/Labs/lab1/README.md

TODO
[x] Support the exit command
[x] Command with no arguments (ls)
[x] Command with arguments (ls -l)
[x] Command to be executed in the background (firefox &)
[ ] Shell builtin commands (cd, echo,...)
[ ] Expression evaluation (export,... etc)

*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include "funcs.h"

#define EXIT "exit"
#define BG_CHAR '&'
#define PRMT ">>> "
#define CD "cd "
#define ERR "Error\n"
#define EXPRT "export "
#define ECHO "echo "
#define WORD_SIZE 20 // ASSUME NO WORD IS LARGER THAN 20 CHAR
#define CMD_SIZE 256 // ASSUNE NO COMMAND IS LARGER THAN 256 CHAR


// Function responsible for getting new command from user then taking the appropriate action
void new_command(){
	// Buffer to save the command that the user enters
	char curr_command[CMD_SIZE] = {'\0'};

	// Prompt
	printf(PRMT);
	fgets(curr_command, sizeof(curr_command), stdin);

	// remove extra \n at the end of the command
	strip(curr_command);

	// Make sure command is not empty
	if(strlen(curr_command) <= 0){
		return;
	}

	// Account for environment variables
	format_env_var(curr_command);
	
	// Process command
	if(!strcmp(curr_command, EXIT)){
		exit(0);
	}// END OF EXIT
	else if(strstr(curr_command, ECHO) != NULL){ // if the comand is echo
		int end_of_echo = find_char(curr_command, ' ');
		printf("%s\n", &curr_command[end_of_echo +1]);
		
	}// END OF ECHO
	else if(strstr(curr_command, EXPRT) != NULL){ // if the command is export
		int name_pos = find_char(curr_command, ' ') + 1;
		int equ_pos = find_char(curr_command, '=');
		int c = 0; // just a counter for loops
		char var_name[WORD_SIZE] = {'\0'};
		char var_val[WORD_SIZE] = {'\0'};
		
		// Get var_name from current_command
		for(int i = name_pos;; i++){
			if(curr_command[i] == '='){
				break;
			}
			else{
				var_name[c] = curr_command[i];
				c++;
			}
		}
		
		c = 0;
		// Get var_val from curr_command
		for(int i = equ_pos +1; curr_command[i] != '\0'; i++){
			if(curr_command[i] == '\"'){
				continue;
			}
			else{
				var_val[c] = curr_command[i];
				c++;
			}
		}

		// generate string needed to initialize env var
		// env_var contains 2 words (var_name and var_value)
		char env_var[2* WORD_SIZE] = {'\0'};
		strcat(env_var, var_name);
		strcat(env_var, "=");
		strcat(env_var, var_val);
		// printf("%s\n", env_var);

		// Create environment variable
		if(putenv(env_var) < 0){
			printf(ERR);
		}
	}// END OF EXPORT
	else if(strstr(curr_command, CD) != NULL){ // if the command is cd
		// Create array to hold words of the command
		int words_count = count_char(curr_command, ' ') + 1;
		// words_cound +1 because the last element is NULL
		char* command_words[words_count +1];
		// split the curr_command into its words
		split(curr_command, command_words);
		// Change the directory
		int cd_res = chdir(command_words[1]);
		// Check if the operation was successful
		if(cd_res < 0){
			printf(ERR);
		}
				
	}// END OF CD
	else if(find_char(curr_command, BG_CHAR) != -1){ // if user wants the command to execute in the background
		// End the command before &
		int amp_pos = find_char(curr_command, BG_CHAR);
		curr_command[amp_pos -1] = '\0';

		// Create array to hold words of the command
		int words_count = count_char(curr_command, ' ') + 1;
		// words_cound +1 because the last element is NULL
		char* command_words[words_count +1];
		// split the curr_command into its words
		split(curr_command, command_words);
		// Make the last element NULL
		char* temp_null = NULL;
		command_words[words_count] = temp_null;

		// Execute command
		pid_t pid = fork();

		if(pid < 0){
			printf(ERR);
		}
		else if(pid == 0){ // child process
			// Run the command in the child process
			int ret_val = execvp(command_words[0], command_words);
			if(ret_val < 0){
				perror(ERR);
				exit(0);
			}
		}
		else{ // Parent process
			usleep(2);
			kill(pid, SIGTSTP);
			// TODO: write to log file
			printf("process in the background\n");
		}

	}// END OF BACKGROUND PROCESS
	else{
		// Create array to hold words of the command
		int words_count = count_char(curr_command, ' ') + 1;
		// words_cound +1 because the last element is NULL
		char* command_words[words_count +1];
		// split the curr_command into its words
		split(curr_command, command_words);
		// Make the last element NULL
		char* temp_null = NULL;
		command_words[words_count] = temp_null;
		
		// Execute command
		pid_t pid = fork();
		
		if(pid < 0){ // Fork failed
			printf(ERR);
		}
		else if(pid == 0){ // Child process
			// Run the command in the child process
			int ret_val = execvp(command_words[0], command_words);
			if(ret_val < 0){
				perror(ERR);
				exit(0);
			}
		}
		else{ // Parent process
			wait(NULL);
			// when the child ends execution we must free allocated memory
			for(int k = 0; command_words[k] != NULL; k++){
				free(command_words[k]);
			}
		}

	}// END OF GENERAL COMMAND
}

int main(){
	while(1){
		new_command();
	}


	return 0;
}
