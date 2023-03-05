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

// Function to split a string into sub-strings
void split(char* str_to_split,char** words_arr){
	char* delim = " ";
	char split_char = delim[0];


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

void new_command(){
	// Buffer to save the command that the user enters
	char curr_command[256] = {'\0'};

	// Prompt
	printf(">>> ");
	fgets(curr_command, sizeof(curr_command), stdin);

	// strip the command from any \n
	strip(curr_command);
	
	// Process command
	// printf("The result of comparison is %d\n", strcmp(curr_command, EXIT));
	int k =0;
	char c = curr_command[k];
	while(c != '\0'){
		printf("%c\n", c);
		k++;
		c = curr_command[k];		
	}
}


int main(){
	printf("%p\n", NULL);
	return 0;
}
