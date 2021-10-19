//implementation of Pipe, using colon to split args

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	char **in1 = malloc (sizeof(char*)*argc);
	char **in2 = malloc (sizeof(char*)*argc);
	int i, j, colonFound = 0;

	//breaking argv into 2 arrays, in1 for first arg and in2 for 2nd arg
	//splitting at the colon if found
	for (i = 0; i < argc-1; i++){
		if(!strcmp(argv[i+1], ":")){
			in1[i] = NULL;
			j = i + 1;
			colonFound = 1;
			break;
		}
		else in1[i] = argv[i+1];
	}

	for (i = 0; i < argc - j; i++){
		in2[i] = argv[++j];
	}
	in2[i] = NULL;

	//check to see if no 2nd arg. if not, only execute 1st arg
	if(!strcmp(argv[argc-1], ":")){
		if (execvp(in1[0], in1) == -1)printf("%s\n", strerror(errno));
		return 0;
	}

	//check if no colon present. if not, run as single arg
	if(colonFound == 0){
		if (execvp(in1[0], in1) == -1)printf("%s\n", strerror(errno));
		return 0;
	}

	//if no special circumstance, run as normal
	int fd[2];
	if (pipe(fd) == -1)printf("%s\n", strerror(errno));

	//first child executes first arg, rerountes to 2nd child.
	switch (fork()) {
		case -1:
			printf("%s\n", strerror(errno));
		case 0:
			if (close(fd[0]) == -1)printf("%s\n", strerror(errno));
			if (fd[1] != STDOUT_FILENO) {
				if (dup2(fd[1], STDOUT_FILENO) == -1)printf("%s\n", strerror(errno));
				if (close(fd[1]) == -1)printf("%s\n", strerror(errno));
			}
			if (execvp(in1[0], in1) == -1)printf("%s\n", strerror(errno));
		default:
			break;
	}

	//2nd child reads from first child, feeds into 2nd arg, rerouts to stdout
	switch (fork()) {
		case -1:
			printf("%s\n", strerror(errno));
		case 0:
			if (close(fd[1]) == -1)printf("%s\n", strerror(errno));
			if (fd[0] != STDIN_FILENO) {
				if (dup2(fd[0], STDIN_FILENO) == -1)printf("%s\n", strerror(errno));
				if (close(fd[0]) == -1)printf("%s\n", strerror(errno));
			}
			if (execvp(in2[0], in2) == -1)printf("%s\n", strerror(errno));
		default:
			break;
	}

	//parent awaits children to finish
	if (close(fd[0]) == -1)printf("%s\n", strerror(errno));
	if (close(fd[1]) == -1)printf("%s\n", strerror(errno));
	if (wait(NULL) == -1)printf("%s\n", strerror(errno));
	if (wait(NULL) == -1)printf("%s\n", strerror(errno));

	exit(EXIT_SUCCESS);
}
