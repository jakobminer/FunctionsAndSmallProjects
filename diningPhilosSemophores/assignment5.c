//dining philosopher problem, implemented using semophores

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include "random.h"

#define PHILOSOPHERCOUNT 5
#define TIMEEATING 100

//prepare global semophore
int chopsticks;

int eatThink(int n){
	//define left and right chopsticks
	int left, right;
	srand(n); 

	//allocate each philospoher's left and right
	left = n;
	if (n < (PHILOSOPHERCOUNT - 1)){
		right = n+1;
	}
	else (right = 0);

	//define semops for picking up and putting down chopsticks
	struct sembuf getLeft[1] = {{left, -1, 0}};
	struct sembuf getRight[1] = {{right, -1, 0}};
	struct sembuf putLeft[1] = {{left, 1, SEM_UNDO}};
	struct sembuf putRight[1] = {{right, 1, SEM_UNDO}};

	int timeSpentEating, timeSpentThinking, time;
	timeSpentThinking = 0;
	timeSpentEating = 0;

	//continue until predetermined time spent eating
	while(timeSpentEating < TIMEEATING){

	    time = randomGaussian(11, 7);
	    if (time < 0) time = 0;
	    printf("Philosopher %d is thinking for %d seconds\n", n, time);
	    timeSpentThinking += time;
	    sleep(time);

	    // get left chopstick
	    if(semop(chopsticks, getLeft, 1) == -1){
	    	printf("Philosopher %d encountered error %d: %s\n", n, errno, strerror(errno));
	    	return n;
	    }

	    // get right chopstick
	    if (semop(chopsticks, getRight, 1) == -1){
	    	printf("Philosopher %d encountered error %d: %s\n", n, errno, strerror(errno));
	    	return n;
	    }

	    time = randomGaussian(9, 3);
	    if (time < 0) time = 0;
	    printf("Philosopher %d is eating for %d seconds\n", n, time);
	    timeSpentEating += time;
	    sleep(time);

	    // put left chopstick
	    if (semop(chopsticks, putLeft, 1) == -1){
	    	printf("Philosopher %d encountered error %d: %s\n", n, errno, strerror(errno));
	    	return n;
	    }

	    // put right chopstick
	    if (semop(chopsticks, putRight, 1) == -1){
	    	printf("Philosopher %d encountered error %d: %s\n", n, errno, strerror(errno));
	    	return n;
	    }

  	}

  	printf("Philosopher %d thought for %d seconds and ate for %d seconds.\n", n, timeSpentThinking, timeSpentEating);
  	return n;
}

int main(){
	int status;
	pid_t philosophers[PHILOSOPHERCOUNT];

	//initialize semophores
	chopsticks = semget(IPC_PRIVATE, PHILOSOPHERCOUNT, IPC_CREAT | IPC_EXCL | 0600);
	if (chopsticks == -1){
		printf("Parent encountered error %d: %s\n", errno, strerror(errno));
		return -1;
	}
	for (int i = 0; i < PHILOSOPHERCOUNT; i++){
		if (semctl(chopsticks, i, SETVAL, 1) == -1){
			printf("Parent encountered error %d: %s\n", errno, strerror(errno));
			if (semctl(chopsticks, 0, IPC_RMID, 0) == -1) printf("Parent encountered error %d: %s\n", errno, strerror(errno));
			return -1;
		}
	}

	//create children, each child goes to table
	for (int i = 0; i < PHILOSOPHERCOUNT; i++){
		int id = fork();
		if (id == 0) {
			int  val = eatThink(i);
			exit(val);
		}
		else {
			philosophers[i] = id;
		}
	}

	//parent tracks children
	for(int i = 0; i < PHILOSOPHERCOUNT; i++) {
    	waitpid(philosophers[i], &status, 0);
  	}

  	//clean up semaphore 
  	if (semctl(chopsticks, 0, IPC_RMID, 0) == -1) printf("Parent encountered error %d: %s\n", errno, strerror(errno));
  	return 0;
}
