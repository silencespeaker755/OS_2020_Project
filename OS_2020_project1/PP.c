#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<sched.h>
#include<time.h>
#include<errno.h>
#include<unistd.h>

#include "process.h"
#include "scheduler.h"

#define sys_my_clock 333
#define my_printk 334

#define CHILD_CORE 1

typedef struct sched_param Sched_p;

void inline TIME_UNIT(void){
	volatile unsigned long i;
	for(i = 0;i < 1000000UL;i++);
	return;
}

int assign_proc_core(pid_t pid, int core){
	cpu_set_t my_cpu;
	if(core > sizeof(my_cpu)){
		fprintf(stderr, "Invalid Core Number: %d\n", core);
		return -1;
	}
	CPU_ZERO(&my_cpu);
	CPU_SET(core, &my_cpu);

	if(sched_setaffinity(pid, sizeof(my_cpu), &my_cpu) != 0){
		perror("error : sched_setaffinity");
		exit(-1);
	}

	return 0;
}

pid_t proc_exec(Process work){
	pid_t child_proc = fork();
	if(child_proc < 0){
		fprintf(stderr, "error : fork");
		exit(2);
	}
	else if(child_proc == 0){
		// Child won't run before wake up.
		
		while(1){
			Sched_p sp;
			sched_getparam(getpid(), &sp);
			if(sp.sched_priority == 99){
				break;
			}
		}
		
		struct timespec start_time, end_time;
		//start timer
		int t = clock_gettime(CLOCK_REALTIME, &start_time);
		//syscall(sys_my_clock, &start_time);

		while(work.exec_time > 0){
			TIME_UNIT();
			work.exec_time--;
		}
		int t = clock_gettime(CLOCK_REALTIME, &end_time);
		//end timer

		//generate dmsg
		char dmsg[128];
		sprintf(dmsg, "[Project1] %d %lu.%09lu %lu.%09lu\n", getpid(),start_time.tv_sec, start_time.tv_nsec
			,end_time.tv_sec, end_time.tv_nsec);
#ifdef DEBUG
		fputs(dmsg, stdout);
#endif
		syscall(my_printk, dmsg);
		printf("%s %d\n", work.name, getpid());

		exit(0);
	}
	else{
		proc_out(child_proc);
		//assign_proc_core(child_proc, CHILD_CORE);
	}

	return child_proc;
}

int proc_out(pid_t pid){
	Sched_p sp;
	sp.sched_priority = 1;
	if(sched_setscheduler(pid, SCHED_FIFO, &sp) < 0){
		perror("error : sched_setaffinity");
		exit(-1);
	}
	assign_proc_core(pid, 0);
	return 0;
}

int proc_wakeup(pid_t pid, int priority){
	assign_proc_core(pid, CHILD_CORE);
	Sched_p sp;
	sp.sched_priority = priority;
	if(sched_setscheduler(pid, SCHED_FIFO, &sp) < 0){
		perror("error : sched_setaffinity");
		exit(-1);
	}
	return 0;
}



