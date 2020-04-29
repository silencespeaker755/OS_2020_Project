#include <sys/types.h>

int FIFO(Process* job, int num_jobs, int now_running);

int SJF(Process* job,int num_jobs, int now_running);

int PSJF(Process* job,int num_jobs, int now_running);

int RR(int *queue, Process* job, int now_running, int *start, int *end, int time_slice);

void scheduler(Process* job,int num_jobs,char* method);
