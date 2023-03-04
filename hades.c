/*
This code is the work of
Yahia Walid Mohamad Eldakhakhny		19016891
Detailed project description is at:
https://github.com/SajedHassan/Operating-Systems/blob/master/Labs/lab1/README.md

TODO
[x] Support the exit command
[ ] Command with no arguments
[ ] Command with no arguments
[ ] Command with arguments
[ ] Command to be executed in the background (firefox &)
[ ] Shell builtin commands (cd, echo,...)
[ ] Expression evaluation (export,... etc)

*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define EXIT "exit\n"

// Function to strip the string from \n
void strip(char* str_to_strip){
	int i =0;
	char curr_char = str_to_strip[0];
	while(curr_char ~= '\0'){
		if(curr_char == '\n'){
			curr_char = '\0';
			break;
		}
		else{
			i++;
			curr_char = str_to_strip(i);
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
	if(!strcmp(curr_command, EXIT)){
		exit(0);
	}
	else{
		printf("Mesh sam3ak mel DJ\n");
	}
}

int main(){
	while(1){
		new_command();
	}


	return 0;
}
