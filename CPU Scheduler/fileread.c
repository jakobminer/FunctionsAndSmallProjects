#include "mylib.h"

int fileread(void *args){ //char* filename, int file_read_done, PCB* Ready_Q, sem_t* sem_cpu){
	int currPID=0;
	char tokens[100][100]; int tokensCounter=0;
	FILE *fp;
	char* token;
	char buffer[1024], buffer2[1024];
	PCB *last,*current;
	int numCPU=0, numIO=0;
	int numProcesses = 0;
	int count=0; char *c; 
	int tempCPUburst[30], tempIOburst[30];
	int numCPUburst=0, numIOburst=0;
	current=&Ready_Q;

	fp=fopen(filename,"r");
	if(fp==NULL){
		printf("Error opening input file.\n");
		return -1;
	}
	
	while( fgets(buffer,1024,fp) != NULL ){
		count=0; tokensCounter=0;
		strcpy(buffer2,buffer);
		c=strtok(buffer, " ");;
		while( c != NULL ){
			count++; 
			c=strtok(NULL, " ");
		} //printf("Number of tokens: %i\n", count);
		token = strtok(buffer2, " ");	
		if ( strcmp(token, "proc") == 0 ){
			//printf("\nPROC\n");
			while( token != NULL ){	
				strcpy(tokens[tokensCounter],token);
				tokensCounter++;
				token=strtok(NULL, " ");
			} // for ( int j=0;j<count;j++) { printf("%s ", tokens[j]); }
			numProcesses=atoi(tokens[2]);
			// Create a PCB structure with PID=++currPID
			current->next=(PCB *)malloc(sizeof(PCB));
			// Insert PCB into the beginning of Ready_Q
			last=current;
			current=current->next;
			current->prev=last;
			current->next=NULL;
			//initialize PCB parameters
			current->PID=0;
			current->PR=atoi(tokens[1]);
			current->numCPUBurst=numProcesses/2 + 1;
			current->numIOBurst=numProcesses/2;
			current->cpuindex=0; current->ioindex=0;
			//printf("Number of CPU bursts: %i, num of IO bursts: %i\n",current->numCPUBurst,current->numIOBurst);
			current->CPUBurst=(int*)malloc(current->numCPUBurst*sizeof(int));
			current->IOBurst=(int*)malloc(current->numIOBurst*sizeof(int));
			int k=0;
			for ( int j=3; j<count ; j++ ){
				current->CPUBurst[k]=atoi(tokens[j]);
				k++;
				j++;
			}	
			k=0;
                        for ( int j=4; j<count ; j++ ){
                                current->IOBurst[k]=atoi(tokens[j]);
                                k++;
				j++;
                        }
			//printf("CPU BURSTS: ");
			for ( int j=0 ; j<current->numCPUBurst ; j++ ) { 
				printf("%i ",current->CPUBurst[j]);
			}	
			//printf("\nIO BURSTS: ");
                        for ( int j=0 ; j<current->numIOBurst ; j++ ) {
                                printf("%i ",current->IOBurst[j]);
                        } printf("\n");
			clock_gettime(CLOCK_MONOTONIC, &current->ts_begin);
			current->ts_initialized = current->ts_begin;
			sem_post(&sem_cpu);
		}
		else if ( strcmp(token, "sleep") == 0){
			//printf("\nSLEEP\n");
			token=strtok(NULL, " ");
			//printf("Sleep amount: %s\n",token);
			usleep(atoi(token)); // Sleep for the given number of ms
			while( token != NULL ){ // Not really needed
				token=strtok(NULL, " ");
			}
		}
		else if (strcmp(token, "stop") == 0 ){
			//printf("\nSTOP\n");
			while( token != NULL ){
				break;
				token=strtok(NULL, " ");
			}
			break;
		} 
	}	


	file_read_done=1;
	fclose(fp);
	file_read_done=1;
	return 1;
}
