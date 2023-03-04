
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define EXIT "exit"


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
	while(1){
		new_command();
	}


	return 0;
}
