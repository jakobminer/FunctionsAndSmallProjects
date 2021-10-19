#include "mylib.h"

int pr (void * args){
	int res;
	struct timespec ts;
	ts.tv_sec=1;
	while (1){
		if (Ready_Q.next==NULL && !cpu_busy && IO_Q.next==NULL && !io_busy && file_read_done==1) break;

		res=sem_timedwait(&sem_cpu,&ts);

		if (res == -1 && errno == ETIMEDOUT) continue;
		cpu_busy = 1;

		pthread_mutex_lock(&mutex);
		while(Ready_Q.next == NULL) {
			pthread_mutex_unlock(&mutex);
			usleep(5);
			pthread_mutex_lock(&mutex);
		}
		PCB *job = Ready_Q.next;
		PCB *temp = job;
		while(temp->next != NULL){
			temp = temp->next;
			if(temp->PR < job->PR){
				job = temp;
			}
		}

		if (job->prev != NULL) (job->prev)->next = job->next;
		if (job->next != NULL) (job->next)->prev = job->prev;
		pthread_mutex_unlock(&mutex);

		clock_gettime(CLOCK_MONOTONIC, &job->ts_end);
		avgWait += (job->ts_end.tv_nsec - job->ts_begin.tv_nsec) 
			+ (job->ts_end.tv_sec - job->ts_begin.tv_sec)*1000000000;
		numWaits++;

		//printf("%d - CPUBurst is sleeping for %d ms\n", job->cpuindex, job->CPUBurst[job->cpuindex]);
		usleep(job->CPUBurst[job->cpuindex]*1000);
		job->cpuindex++;
		if (job->cpuindex >= job->numCPUBurst){
			clock_gettime(CLOCK_MONOTONIC, &job->ts_finished);
			job->turnaround = (job->ts_finished.tv_nsec-job->ts_initialized.tv_nsec)
				+ (job->ts_finished.tv_sec - job->ts_initialized.tv_sec)*1000000000;
			printf("turnaround = %fms\n", job->turnaround*0.000001);
			printf("timeinq = %fms\n", job->timeInQ*0.000001);
			avgTurnaround += job->turnaround*0.000001;
			numProcs ++;
			cpu_busy = 0;
		}
		else {

			pthread_mutex_lock(&mutex);
			PCB *seekend = &IO_Q;
			while (seekend->next != NULL){
				seekend = seekend->next;
			}
			job->next = NULL;
			job->prev = seekend;
			seekend->next = job;
			cpu_busy = 0;
			sem_post(&sem_io);
			pthread_mutex_unlock(&mutex);
		}
	}
	cpu_sch_done = 1;
	return 1;
}