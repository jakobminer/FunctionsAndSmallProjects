#include "mylib.h"
int quantum;
int numProcs = 0;
int numWaits = 0;
float avgTurnaround = 0;
float avgWait = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void main(int argc, char* argv[]){
	//char *filename;i
	char temp[250];
	char alg[10];
	pthread_t filereadThread;
    pthread_t schedulerThread;
    pthread_t ioThread;
	int filereadThreadRet, schedulerThreadRet, ioThreadRet;
	cpu_sch_done=0; io_sys_done=0; cpu_busy=0; io_busy=0; file_read_done=1;
	// Process Command Line Args & Get Simulation Parameters
	//printf("Number of arguments: %i\n", argc);
	for( int i=0 ; i<argc ; i++ ){
		if( strcmp(argv[i],"-alg") == 0 ){
			printf("Algorithm is %s\n", argv[i+1]);
			strcpy(alg, argv[i+1]);
		} 
		if ( strcmp(argv[i],"-quantum") == 0 ){
			printf("Quantum time is %s\n", argv[i+1]);
			quantum = strtol(argv[i+1],NULL,10);
		}
		if( strcmp(argv[i],"-input") == 0 ){
			strcpy(filename,argv[i+1]);
			printf("Input file is %s\n", filename);
		}
	}

	// Create/Initialize necessary data structures (Ready_Q and IO_Q)
	sem_init(&sem_cpu, 1, 0);
	sem_init(&sem_io, 1, 0);
	// Start the three threads with appropriate parameters
	filereadThreadRet = pthread_create(&filereadThread, NULL, (void *)fileread,NULL);
	pthread_join(filereadThread,NULL);

	PCB *navigator;
    navigator = &Ready_Q;
    //printf("\nPost fileread contents of Ready_Q:\n");
    int i=1;
    while( navigator != NULL ){
            //printf("item %i - CPUBurst: %i IOBurst: %i\n", i, navigator->numCPUBurst, navigator->numIOBurst);
            navigator=navigator->next;
            i++;
    }
    int value;
    sem_getvalue(&sem_cpu, &value);
    //printf("SEMAPHORE VALUE: %i\n", value);

	if (!strcmp(alg, "fifo")) schedulerThreadRet = pthread_create(&schedulerThread, NULL, (void *)fifo,NULL);
	if (!strcmp(alg, "sjf")) schedulerThreadRet = pthread_create(&schedulerThread, NULL, (void *)sjf,NULL);
	if (!strcmp(alg, "rr")) schedulerThreadRet = pthread_create(&schedulerThread, NULL, (void *)rr,NULL);
	if (!strcmp(alg, "pr")) schedulerThreadRet = pthread_create(&schedulerThread, NULL, (void *)pr,NULL);

	ioThreadRet = pthread_create(&ioThread, NULL, (void *)iosched,NULL);
	pthread_join(schedulerThread,NULL);
	pthread_join(ioThread,NULL);

	printf("Avg Turnaround = %0.1fms\n", avgTurnaround/numProcs);
	printf("Avg Wait = %0.1fms\n", avgWait/numWaits*0.000001);

	return; 
}
