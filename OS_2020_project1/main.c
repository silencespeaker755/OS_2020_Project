#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "process.h"
#include "scheduler.h"

int main(int argc, char const *argv[])
{
	char policy[16];
	int process_num;
	scanf("%s", policy);
	scanf("%d", &process_num);
	Process *job = (Process*) malloc(process_num*(sizeof(Process)));
	for(int i = 0;i < process_num;i++){
		scanf("%s%d%d", job[i].name, &(job[i].ready_time), &(job[i].exec_time));
	}

	scheduler(job, process_num, policy);
	free(job);
	return 0;
}