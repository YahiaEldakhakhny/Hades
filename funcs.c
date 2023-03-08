#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define EXIT "exit"
#define BG_CHAR '&'
#define PRMT ">>> "
#define CD "cd "
#define ERR "Error\n"
#define EXPRT "export "

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

/* Function to find a specific the first instance of 
a char in a string
if the char is not found the function returns -1
*/
int find_char(char* str, char target){
	for(int i = 0; str[i] != '\0'; i++){
		if(str[i] == target){
			return i;
		}
	}
	return -1;
}

/*	Function to iterate over the words in a command
	and if a call for an env variable is made
	the function will replace the var with its value
*/
void format_env_var(char* cmd){
	int dol_pos = find_char(cmd, '$');
	// printf("$ at position: %d\n", dol_pos);
	if(dol_pos >= 0){
		int pos_nxt_space = find_char(&cmd[dol_pos], ' ');
		int end_of_cmd = (int) strlen(cmd);
		int var_end = ((pos_nxt_space >= 0) && (pos_nxt_space < end_of_cmd)) ? (pos_nxt_space -1) : (end_of_cmd -1);

		// printf("End of var at: %d\n", var_end);
		// Get var name
		char var_name[10] = {'\0'};
		int c = 0; //just a counter for the loop
		for(int i = dol_pos +1; i <= var_end; i++){
			var_name[c] = cmd[i];
			c++;
		}
		// printf("var name is %s\n", var_name);

		// Get variable value
		char* var_val = getenv(var_name);
		// printf("var val is %s\n", var_val);
		
		// make a temp var to assemble
		char temp_cmd[256];
		for(int j = 0; j < dol_pos; j++){
			temp_cmd[j] = cmd[j];
		}

		// add the value of the var
		strcat(temp_cmd, var_val);
		// add the rest of the command
		strcat(temp_cmd, &cmd[var_end + 1]);
		// Modify cmd
		strcpy(cmd, temp_cmd);
		// printf("modified command: %s\n", temp_cmd);
	}
}



