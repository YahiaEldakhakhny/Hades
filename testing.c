
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define EXIT "hi\n"


void new_command(){
	// Buffer to save the command that the user enters
	char curr_command[256] = {'\0'};

	// Prompt
	printf(">>> ");
	fgets(curr_command, sizeof(curr_command), stdin);

	// Process command
	printf("The result of comparison is %d\n", strcmp(curr_command, EXIT));
}


int main(){
	while(1){
		new_command();
	}


	return 0;
}
