// Function to count spaces in a string
int count_char(char* str, char delim);


//Function to separate words by spaces
void split(char* str_to_split, char* words_arr[]);


// Function to strip the string from \n
void strip(char str_to_strip[]);


/* Function to find a specific char in a string
if the char is not found the function returns -1
*/
int find_char(char* str, char target);


/*	Function to iterate over the words in a command
	and if a call for an env variable is made
	the function will replace the var with its value
*/
void format_env_var(char* cmd);

