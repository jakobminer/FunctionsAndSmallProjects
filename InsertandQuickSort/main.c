#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "assignment7.h"

int main (int argc, char **argv) {
    FILE *fd = fopen("webster_random", "r");
    char line[17];
    char **dictionary = malloc(sizeof(char*)*20422);
    int i = 0;
    while (fgets(line, 17, fd) != NULL) {
        dictionary[i] = malloc(sizeof(char)*17);
        strcpy(dictionary[i++], line);
    }
    fclose(fd);
    clock_t start = clock();
    setSortThreads(2);
    sortThreaded(dictionary, 20422);
    clock_t end = clock();
    printf("This time = %lf seconds\n", ((double)(end-start))/CLOCKS_PER_SEC);

    i = 0;
    fd = fopen("webster_random_sorted", "w");
    while (i < 20422) {
        fputs(dictionary[i++], fd);
    }
    fclose(fd);

    return 0;
}
