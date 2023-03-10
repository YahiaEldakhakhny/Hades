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



// Function responsible for getting new command from user then taking the appropriate action
void new_command(){
	// Buffer to save the command that the user enters
	char curr_command[256] = {'\0'};

	// Prompt
	printf(PRMT);
	fgets(curr_command, sizeof(curr_command), stdin);

	// remove extra \n at the end of the command
	strip(curr_command);
	
	// Process command
	if(!strcmp(curr_command, EXIT)){
		exit(0);
	}// END OF EXIT
	else if(strstr(curr_command, EXPRT) != NULL){ // if the command is export
		int name_pos = find_char(curr_command, ' ') + 1;
		int equ_pos = find_char(curr_command, '=');
		int c = 0; // just a counter for loops
		char var_name[10] = {'\0'};
		char var_val[10] = {'\0'};
		
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
		char env_var[40] = {'\0'};// 40 is just an arbitrary number
		strcat(env_var, var_name);
		strcat(env_var, "=");
		strcat(env_var, var_val);
		printf("env_var is %s\n", env_var);

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
				return;
			}
		}
		else{ // Parent process
			usleep(1);
			kill(pid, SIGTSTP);
			printf("process in the background\n");
		}

	}// END OF BACKGROUND PROCESS
	else{
		// Account for environment variables
		format_env_var(curr_command);
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
				return;
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
	putenv("x=-l -a -h");
	printf("%s\n", getenv("x"));
	return 0;

}
