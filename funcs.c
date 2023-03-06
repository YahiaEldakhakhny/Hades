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

/* Function to find a specific char in a string
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