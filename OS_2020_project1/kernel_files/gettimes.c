#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

asmlinkage void sys_my_clock(struct timespec *t){
    getnstimeofday(t);
    return;
}