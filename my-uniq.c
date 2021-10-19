/*
Jakob Minermy-uniq
8 Feb 2021

A simplified implementation of the unix command uniq. it prints the contents of one or more
	files to stdout, omitting all instances of lines appearing 2 or more times sequentially.

	If no input file is given, it will instead read from stdin.

	to run, enter the command:
	$ ./my-uniq <file1> <file2> <file...etc>
	
	line buffers are dynamically sized, 
	so lines are read without wasting memory or under-allocating
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char const *argv[]) {
	char *line;
	char *lastLine;
	size_t size = 0;
	int bytesRead;
	FILE *fp;

	if (argc == 1){
		lastLine = malloc(0);
		line = (char *) malloc (size);
		bytesRead = getline(&line, &size, stdin);
		while(bytesRead > 0){
			if(strcmp(line, lastLine))
				printf("%s", line);
			free(lastLine);
			lastLine = malloc(bytesRead);
			strcpy(lastLine, line);
			bytesRead = getline(&line, &size, stdin);
		}
		free(line);
		free(lastLine);
		return 0;
	}

	for (int i = 1; i < argc; i++){
		fp = fopen(argv[i], "r");
		if (fp == NULL) {
			printf("cannot open file\n");
			exit(1);
		}

		lastLine = malloc(0);
		line = (char *) malloc (size);
		bytesRead = getline(&line, &size, fp);
		while (bytesRead > 0){
			if(strcmp(line, lastLine))
				printf("%s", line);
			free(lastLine);
			lastLine = malloc(bytesRead);
			strcpy(lastLine, line);
			bytesRead = getline(&line, &size, fp);
		}

		fclose(fp);
	}
	return 0;
}
