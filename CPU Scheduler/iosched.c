#include "mylib.h"

int iosched (void * args){
	int res;
	struct timespec ts;
	ts.tv_sec=1;

	while(1){
		if (Ready_Q.next==NULL && !cpu_busy && IO_Q.next==NULL && !io_busy && file_read_done==1) break;

		res=sem_timedwait(&sem_io,&ts);

		if (res == -1 && errno == ETIMEDOUT) continue;
		io_busy = 1;

		pthread_mutex_lock(&mutex);
		PCB *job = IO_Q.next;
		IO_Q.next = (IO_Q.next)->next;
		IO_Q.prev = &IO_Q;
		pthread_mutex_unlock(&mutex);


		//printf("%d - IOBurst is sleeping for %d ms\n", job->ioindex, job->IOBurst[job->ioindex]);
		usleep(job->IOBurst[job->ioindex]*1000);
		job->ioindex++;

		pthread_mutex_lock(&mutex);
		PCB *seekend = &Ready_Q;
		clock_gettime(CLOCK_MONOTONIC, &job->ts_begin);
		while (seekend->next != NULL){
			seekend = seekend->next;
		}
		job->next = NULL;
		job->prev = seekend;
		seekend->next = job;

		io_busy = 0;
		sem_post(&sem_cpu);
		pthread_mutex_unlock(&mutex);
	}
}