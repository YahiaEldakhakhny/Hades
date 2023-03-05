/*
This code is the work of
Yahia Walid Mohamad Eldakhakhny		19016891
Detailed project description is at:
https://github.com/SajedHassan/Operating-Systems/blob/master/Labs/lab1/README.md

TODO
[x] Support the exit command
[x] Command with no arguments (ls)
[x] Command with arguments (ls -l)
[ ] Command to be executed in the background (firefox &)
[ ] Shell builtin commands (cd, echo,...)
[ ] Expression evaluation (export,... etc)

*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define EXIT "exit"

// Function to count spaces in a string
int count_char(char* str, char delim){
	
	// Count the number of spaces
	int number_of_spaces = 0;
	int i = 0;
	while(str[i] != '\0'){
		if(str[i] == delim){
			number_of_spaces++;
		}
		i++;
	}
	return number_of_spaces;	
}

//Function to separate words by spaces
void split(char* str_to_split, char* words_arr[]){
	char* delim = " ";
	char delim_char = delim[0];
	int i = 0;

	// Get first token in str_to_split
	char* token = strtok(str_to_split, delim);
	// Loop through the words and copy them
	while(token != NULL){
		words_arr[i] = malloc(10* sizeof(char));
		strcpy(words_arr[i], token);
		i++;
		token = strtok(NULL, delim);
	}
	
}

// Function to strip the string from \n
void strip(char str_to_strip[]){
	int i =0;
	char curr_char = str_to_strip[i];
	while(curr_char != '\0'){
		if(curr_char == '\n'){
			str_to_strip[i] = '\0';
			break;
		}
		else{
			i++;
			curr_char = str_to_strip[i];
		}
	}
}

// Function responsible for getting new command from user then taking the appropriate action
void new_command(){
	// Buffer to save the command that the user enters
	char curr_command[256] = {'\0'};

	// Prompt
	printf(">>> ");
	fgets(curr_command, sizeof(curr_command), stdin);

	// Process command
	// remove extra \n at the end of the command
	strip(curr_command);
	if(!strcmp(curr_command, EXIT)){
		exit(0);
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

		}
	}
}

int main(){
	while(1){
		new_command();
	}


	return 0;
}
