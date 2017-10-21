#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>

int StringValid(char *string);
char *ToggleString(char *string);

int main(){

	char ParentString[100], ChildString[100];
	int length, i;

	printf("Enter in a string: ");
	scanf("%s", ParentString);
	char p[] = ToggleString(ParentString);
	printf("%s", p);

	int valid = StringValid(ParentString);


 	if(valid != 0){
	
	return -1;
	
	}

	int fd [4];
	pid_t child;
	for(i=0;i<2;i++){

		if(pipe(fd+(i*2))<0){
			
			perror("Pipe wasn't created");
			exit(EXIT_FAILURE);			
			
		}
	}

	if((child = fork())==-1){

		perror("fork");
		return 1;	

	}
	
	if(child == 0){

		close(fd[0]);
		close(fd[3]);
		
		child = getpid();

		length = read(fd[2], &ChildString, length);
		if(length <0){

			perror("Pipe transfer failed");
			exit(EXIT_FAILURE);	

		}
		
		if(length == 0){

			perror("Something went wrong");


		}else{

			printf("\nChild got string\n");
			printf("%s\n",ChildString);
			if(write(fd[1], ToggleString(ChildString), strlen(ChildString))<0){
			
				perror("Child did not write");
				exit(EXIT_FAILURE);	
	
			}
		
		}

		close(fd[2]);
		close(fd[1]);
		return (EXIT_SUCCESS);
		

	}



	close(fd[2]);
	close(fd[1]);

	child = getpid();

	if(write(fd[3], ParentString, (strlen(ParentString))) != strlen(ParentString)){
	
		perror("Parent did not write properly");
		exit(EXIT_FAILURE);	

	}

	length = read(fd[0], &ParentString, strlen(ParentString));

	if(length <0){

			perror("Pipe transfer failed 2");
			exit(EXIT_FAILURE);			

		}
		
		if(length == 0){

			perror("Something went wrong 2");

		}else{

			printf("%s",ParentString);
		
		}

		close(fd[0]);
		close(fd[3]);
		wait(NULL);

		return (EXIT_SUCCESS);

	


	return (EXIT_SUCCESS);

}	

int StringValid(char *string){

	int i = 0;
	while(i<strlen(string)){
		if(!isalpha((int)string[i])){
			printf("\nUse only letters no numbers or special characters\n");
			return 1;
		}
		i++;

	}
	return 0;
}

char *ToggleString(char *str){

	int i = 0;
	while(i<strlen(str)){
		
		if((int)str[i]<90){

			str[i] += 32;

		}else{

			str[i] -= 32;

		}
		
		i++;

	}
	return str;
}