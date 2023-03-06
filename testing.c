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

// Function responsible for getting new command from user then taking the appropriate action
void new_command(){
	// Buffer to save the command that the user enters
	char curr_command[256] = {'\0'};

	// Prompt
	printf(PRMT);
	fgets(curr_command, sizeof(curr_command), stdin);

	// Process command
	// remove extra \n at the end of the command
	strip(curr_command);
	if(!strcmp(curr_command, EXIT)){
		exit(0);
	}
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
			printf("Error fork failed\n");
		}
		else if(pid == 0){ // child process
			// Run the command in the child process
			int ret_val = execvp(command_words[0], command_words);
			if(ret_val < 0){
				perror("Error\n");
				return;
			}
		}
		else{ // Parent process
			usleep(1);
			kill(pid, SIGTSTP);
			printf("process in the background\n");
		}

	}
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
			printf("Error fork failed\n");
		}
		else if(pid == 0){ // Child process
			// Run the command in the child process
			int ret_val = execvp(command_words[0], command_words);
			if(ret_val < 0){
				perror("Error\n");
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
	}

}


int main(){
	while(1){
		new_command();
	}
	return 0;
}
