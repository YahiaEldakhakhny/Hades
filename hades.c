#include <stdio.h>
#include <string.h>
#include <unistd.h>

void new_command(){
	char curr_command[256] = {'\0'};
	pid_t my_pid;
	printf(">>> ");
	fgets(curr_command, sizeof(curr_command), stdin);
	my_pid = getpid();
        printf("you entered %s", curr_command);
	printf("the pid is %u \n", my_pid);
}

int main(){
	while(1){
		new_command();
	}


	return 0;
}
