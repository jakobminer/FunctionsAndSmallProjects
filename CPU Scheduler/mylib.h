#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

struct PCB {
	int PID, PR;
	int numCPUBurst, numIOBurst; // to create dynamic arrays to store cpu and io burst times
	int *CPUBurst,*IOBurst;
	int cpuindex, ioindex;
	double timeInQ, turnaround;
	struct timespec ts_begin, ts_end, ts_initialized, ts_finished;
	struct PCB *prev, *next;
	time_t timeEnterReadyQ;
};

typedef struct PCB
	PCB;

char filename[1024];
PCB Ready_Q;
sem_t sem_cpu, sem_io;
int file_read_done;
PCB IO_Q; // Doubly linked lists of PCB
int cpu_sch_done, io_sys_done, cpu_busy, io_busy;
extern int quantum;
extern pthread_mutex_t mutex;
extern float avgTurnaround;
extern float avgWait;
extern int numProcs;
extern int numWaits;

void bubbleswap(int* x,int* y);
int sjf(void *args);
int fifo(void *args);
int rr(void *args);
int pr(void *args);
int iosched(void *args);
int fileread(void *args); //char* filename, int file_read_done, PCB* Ready_Q, sem_t* sem_cpu);

