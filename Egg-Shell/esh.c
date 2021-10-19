/***********************************************************************
name: 	
	Jakob Miner
	Spencer Ross
	Sarah Mathes
	
Egg Shell -- esh

Objectives:	

	•	esh should run an infinite loop that provides the user with a 
		prompt. At this prompt, the user should be able to enter a 
		command which your shell should endeavor to execute.

	•	The first task will be reading in the command, and since some 
		commands may contain arguments you will have to read in the 
		entire line. For this, I would recommend looking into [GNU Read-
		line library]. 

	•	After the input is read in, the shell will need to verify that 
		the command entered is indeed a legal command. All commands must 
		be found on the path specified by the environment variable, PATH. 

	•	The PATH consists of a series of paths delimited by the ‘:’ 
		character. To show the value of PATH, type echo $PATH on the linux 
		command line.

	•	To access the directories included on the PATH, you might want to 
		investigate the functions getenv() and strtok(). If esh cannot find 
		the command on the PATH, then it should check the current directory 
		‘.’. If the command is still not found, then it should print an error 
		message and present a new prompt, i.e., esh should not crash.

	•	If the command is found, esh should execute it using fork() and 
		execv(). Commands may have up to five optional arguments which are 
		delimited by white space. 

	•	If the last argument in a command is an & character, it indicates 
		that the command is to be run in the background. In other words, when 
		the shell spawns a child process, it should not wait for the child to 
		terminate before prompting the user for the next command.

	•	If the user types exit then esh should terminate.

	•	You can omit handling the cd command.

***********************************************************************/

/* Includes and definitions */
#include <unistd.h>
// #include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define FALSE 0
#define TRUE 1
#define SIZE 128
/**********************************************************************
./esh <arg1> &

    maybe put some usage here??
**********************************************************************/

int main(int argc, char const *argv[]) {
	char* pathEnviron = getenv("PATH");
	char** paths = (char **)malloc(SIZE*sizeof(char*));
	char* input;
	bool runInTheBackground = false;
	int status, numArgs = 0;
	char *token;
	char** args = NULL;
	struct dirent *de;
	DIR *dr;
	bool found = false;
	int numPaths = 0; // number of directories on PATH

	// Seperate each directory on PATH by ":"
	token = strtok(pathEnviron,":");
	while ( token != NULL ){ 
		//printf("%s\n",token);
		paths[numPaths] = (char *)malloc(strlen(token)*sizeof(char *));
		strcpy(paths[numPaths],token);
		numPaths++;
		token = strtok(NULL,":");
	}
	numPaths--;	
	
	// Print statement to confirm paths in path[]
	int j=0;
    while ( j <= numPaths){
            //printf("FILE PATH:\n %s\n", paths[j]);
            j++;
    }

	while (1){
		runInTheBackground = false;
		args = (char **)realloc(args,0);
		j=0;
		numArgs = 0;
		int i =0;
		input = readline("esh>");
		if (!strcmp(input, "") || !strcmp(input, "&")) continue;
		token = strtok(input, " ");
		while ( token != NULL ){
			args = (char **)realloc(args, sizeof(char **)*(numArgs+1));
			args[numArgs] = (char *)malloc(sizeof(char *)*strlen(token));
			strcpy(args[numArgs],token);
			numArgs = numArgs + 1;
			token = strtok(NULL, " ");
		}

		// Assess if the last character is & and make the last argument NULL for execv
        if ( strcmp(args[numArgs-1],"&") == 0 ){
                //printf("RUN IN THE BACKGROUND\n");
                runInTheBackground = true;
                args[numArgs-1] = NULL;
        } else {
                args[numArgs] = NULL;
        }

		// Demonstrating the argument number and each token. args[0] should be the command.
		//printf("number of arguments: %i\n", numArgs);
		while (i < numArgs ){
			//printf("LINE: %s\n", args[i]);
			i++;
		}

		if(strcmp(args[0], "exit") == 0) {
			for(int k=0; k<numArgs; k++) free(args[k]);
			for(int k=0; k<numPaths; k++) free(paths[k]);
			free(args);
			free(paths);
			exit(0);
		}

		// Open each directory and see if args[0] is in them.
		char argumentOne[1];
		strcpy(argumentOne,"");
		// Open each directory and see if args[0] is in them.
		j=0;
		int h=0;
		bool found = false;
		int status;
		while( j<= numPaths ){
			//printf("Checking Path: %s\n", paths[j]);
			dr=opendir(paths[j]);
			if(dr == NULL){
				//printf("Couldn't open directory.\n");
				// return 0;
				continue;
			}
			while( (de = readdir(dr)) != NULL){
				if(strcmp(de->d_name,args[0]) == 0){
                        //printf("It's in here: %s\n", paths[j]);
                        strcat(argumentOne,paths[j]);
                        strcat(argumentOne,"/");
                        strcat(argumentOne,args[0]);
                        //printf("Final first argument for execv: %s %s\n", argumentOne,args[0]);
                        h=j;
                        j=numPaths + 1;
                        if(!fork()) {
                                execv(argumentOne,args);
                                perror("Can't exec\n");
                        }
                        if (!runInTheBackground) wait(&status);
                        found = true;
                        break;
                }

			}
			if(found) break; // if command found exececuted stop looking for it
			rewinddir(dr);
			closedir(dr);
			j++;
		}
		if(found == false) {
		    char* curr_path = ".";
		    if((dr = opendir(curr_path)) == NULL) {
		        //printf("Couldn't open directory.\n");
		        continue;
		    }
		    while((de = readdir(dr)) != NULL) {
		        if(strcmp(de->d_name,args[0]) == 0) {
		            //printf("It's in here: %s\n", curr_path);
		            strcat(argumentOne,curr_path);
		            strcat(argumentOne,"/");
		            strcat(argumentOne,args[0]);
		            //printf("Final first argument for execv: %s %s\n", argumentOne,args[0]);
		            h=j;
		            j=numPaths + 1;
		            if(!fork()) {
		                    execv(argumentOne,args);
		                    perror("Can't exec\n");
		            }
		            if (!runInTheBackground) wait(&status);
		            found = true;
		            break;
		        } 
		    }
			if(found==false) printf("ERROR: %s not found!\n", args[0]);
		}
	}
	
	
	return 0;
}


