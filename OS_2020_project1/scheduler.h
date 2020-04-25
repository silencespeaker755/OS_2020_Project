#include <sys/types.h>

int FIFO(Process* job, int num_jobs, int now_running);

int SJF(Process* job,int num_jobs, int now_running);

int PSJF(Process* job,int num_jobs, int now_running);

int RR(Process* job,int num_jobs, int now_running, int time_slice);

void scheduler(Process* job,int num_jobs,char* method);