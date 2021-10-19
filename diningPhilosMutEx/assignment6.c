#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define TIMEEATING 100
#define NUMPHILOSOPHERS 5

//declare chopsticks, philosophers
pthread_mutex_t chopsticks[5];

int randomGaussian(int mean, int stddev) {
	double mu = 0.5 + (double) mean;
	double sigma = fabs((double) stddev);
	double f1 = sqrt(-2.0 * log((double) rand() / (double) RAND_MAX));
	double f2 = 2.0 * 3.14159265359 * (double) rand() / (double) RAND_MAX;
	if (rand() & (1 << 5))
		return (int) floor(mu + sigma * cos(f2) * f1);
	else
		return (int) floor(mu + sigma * sin(f2) * f1);
}

static void *eatThink(void *philoID){
	int id = *(int *)philoID;
	int time, timeSpentThinking, timeSpentEating, left, right, lock;

	timeSpentThinking = 0;
	timeSpentEating = 0;

	//assign left and right chopsticks
	left = id;
	right = (id == NUMPHILOSOPHERS - 1 ? 0 : id + 1);

	//loop for each thread until given thread is full
	while (timeSpentEating < TIMEEATING){

		//pick up chopsticks
		lock = 0;
		while (lock == 0){
		    if (pthread_mutex_lock(&chopsticks[left]) != 0){
		    	printf("Philosopher %d encountered error %d: %s\n", id, errno, strerror(errno));
		    }

		    switch (pthread_mutex_trylock(&chopsticks[right])){
    			case 0:
    				lock = 1;
      				break;
    			case EBUSY:
	    			if (pthread_mutex_unlock(&chopsticks[left]) != 0){
			    		printf("Philosopher %d encountered error %d: %s\n", id, errno, strerror(errno));
					}
        			break;
    			default:
    				printf("Philosopher %d encountered error %d: %s\n", id, errno, strerror(errno));
			}
		}

		//random eating time
	    time = randomGaussian(9, 3);
	    if (time < 0) time = 0;
	    timeSpentEating += time;
	    usleep(1);
	    printf("Philosopher %d is eating for %d seconds (%d so far)\n", id, time, timeSpentEating);
	    sleep(time);

	    //put down chopsticks
	    if (pthread_mutex_unlock(&chopsticks[left]) != 0){
		    printf("Philosopher %d encountered error %d: %s\n", id, errno, strerror(errno));
		}

	    if (pthread_mutex_unlock(&chopsticks[right]) != 0){
		    printf("Philosopher %d encountered error %d: %s\n", id, errno, strerror(errno));
		}

		//random thinking time
		time = randomGaussian(11, 7);
		if (time < 0) time = 0;
		timeSpentThinking += time;
		printf("Philosopher %d is thinking for %d seconds\n", id, time);
		sleep(time);
	}

}

int main () {
	pthread_t philosophers[5];
	int i, id[NUMPHILOSOPHERS];
	srand(25);

	//initialize mutexes
	for (i = 0; i < NUMPHILOSOPHERS; i++){
		if (pthread_mutex_init (&chopsticks[i], NULL) != 0){
		    printf("System encountered error %d: %s\n",errno, strerror(errno));
		}
	}

	//assign each philo id away from loop index to avoid race condition
	for (i = 0; i < NUMPHILOSOPHERS; i++){
    	id[i] = i;
	}

	//create philosopher threads
	for (i = 0; i < NUMPHILOSOPHERS; i++){
		if (pthread_create (&philosophers[i], NULL, eatThink, &id[i]) != 0){
		    printf("System encountered error %d: %s\n",errno, strerror(errno));
		}
	}

	//wait for threads to complete
    for (i = 0; i < NUMPHILOSOPHERS; i++){
    	if (pthread_join (philosophers[i], NULL) != 0){
		    printf("System encountered error %d: %s\n",errno, strerror(errno));
		}
    }

    return 0;
}