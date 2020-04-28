#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage void sys_my_printk(char *str){
    printk(str);
    return;
}