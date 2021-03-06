#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>
#include<unistd.h>
#include<limits.h>
#include <sched.h>
#include "process.h"
#include "scheduler.h"

#define MM 32

int queue_pop(int *queue, int* start, int* end){
	int front = -1;
	if(*start != *end){
		front = queue[*start];
		*start = ((*start) + 1) % MM;
	}
	return front;
}
void queue_push(int *queue, int k, int* start, int* end){
	queue[(*end) % MM] = k;
	*end = ((*end) + 1) % MM;
	return;
}


int FIFO(Process* job, int num_jobs, int now_running){
	int next = -1;
	if(now_running == -1){
		int temp =  INT_MAX;
		for(int i = 0;i < num_jobs;i++){
			if(job[i].pid != -1 && job[i].ready_time < temp){
				temp = job[i].ready_time;
				now_running = i;
			}
		}
	}
	next = now_running;

	return next;
}
int SJF(Process* job,int num_jobs, int now_running){
	int next = -1;
	if(now_running == -1){
		int temp =  INT_MAX;
		for(int i = 0;i < num_jobs;i++){
			if(job[i].pid != -1 && job[i].exec_time < temp){
				temp = job[i].exec_time;
				now_running = i;
			}
		}
	}
	next = now_running;
	
	return next;
}
int PSJF(Process* job,int num_jobs, int now_running){
	int next = -1;
	int temp;
	if(now_running == -1)
		temp = INT_MAX;
	else
		temp = job[now_running].exec_time;
	for(int i = 0;i < num_jobs;i++){
		if(job[i].pid != -1 && job[i].exec_time < temp){
			temp = job[i].exec_time;
			now_running = i;
		}
	}
	next = now_running;

	return next;
}
int RR(int *queue, Process* job, int now_running, int* start, int* end, int time_slice){
	int next = now_running;
	int i;
	if(time_slice == 500 || now_running == -1){
		//fprintf(stderr, "before: now_running = %d ", now_running);
		if((i = queue_pop(queue, start, end)) != -1){
			next = i;
			if(now_running != -1){
				queue_push(queue, now_running, start, end);
			}
			//fprintf(stderr,"RR next = %d\n", next);
		}
	}

	//fprintf(stderr, "after: now_running = %d ", next);
	return next;
}

void scheduler(Process* job,int num_jobs,char* method){
	assign_proc_core(getpid(), 0);
	Sched_p sp;
	sp.sched_priority = 99;
	if(sched_setscheduler(getpid(), SCHED_FIFO, &sp) < 0){
		perror("error : sched_setaffinity");
		exit(-1);
	}
	/*Initialize*/
	for(int i = 0;i < num_jobs;i++){
		job[i].pid = -1;
	}
	unsigned long current_time = 0;
	int finish_works_number = 0,now_running = -1,last_process = -1;
	int time_slice = 500, start = 0, end = 0;
	int *queue;
	/*Use a queue*/
	if(method[0] == 'R')
		queue = (int*) malloc(MM * sizeof(int));

	/*Check which job is already finish in this round*/
	while(1){
		if(now_running != -1 && job[now_running].exec_time == 0){
			waitpid(job[now_running].pid, NULL, 0);
			job[now_running].pid = -1;
			//printf("%s %d\n", job[now_running].name, finish_works_number+1);
			now_running = -1;
			finish_works_number++;
			if(finish_works_number >= num_jobs){
				break;
			}
		}

		/*Set jobs which is ready into ready state*/
		for(int i = 0;i < num_jobs;i++){
			if(job[i].ready_time == current_time){
				job[i].pid = proc_exec(job[i]);
				if(method[0] == 'R'){
					queue_push(queue, i, &start, &end);
					//fprintf(stderr, "push = %d\n", i);
				}
				//fprintf(stderr," %d ",job[i].pid);
			}
		}

		/*Find nextone*/
		int next;
		char c = method[0];
		switch(c){
			case 'F':
				next = FIFO(job, num_jobs, now_running);
				break;
			case 'S':
				next = SJF(job, num_jobs, now_running);
				break;
			case 'P':
				next = PSJF(job, num_jobs, now_running);
				break;
			case 'R':
				if(time_slice == 0 || now_running == -1){
					time_slice = 500;
				}
				next = RR(queue, job, now_running, &start, &end, time_slice);
				time_slice--;
				break;
			default:
				perror("Please enter the schedule strategy again!");
		}

		/*Context Switch*/
		if(next != now_running && next != -1){
			if(now_running != -1){
				proc_out(job[now_running].pid);
			}
			proc_wakeup(job[next].pid, 99);
			now_running = next;
		}
		//fprintf(stderr, "after: now_running = %d ", next);
		TIME_UNIT();
		current_time++;
		if(now_running != -1){
			job[now_running].exec_time--;
		}
		//fprintf(stderr," %ld ", current_time);
	}
	return;
}







