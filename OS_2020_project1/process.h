#include<sys/types.h>
#include<sys/syscall.h>

typedef struct process{
	char name[64];
	int ready_time;
	int exec_time;
	pid_t pid;
}Process;

void TIME_UNIT(void);

int assign_proc_core(pid_t pid, int core);

pid_t proc_exec(Process work);

int proc_out(pid_t pid);

int proc_wakeup(pid_t pid, int priority);
