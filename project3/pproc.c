#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/sched.h>
// #include <linux/sched/signal.h>

int tasks_init(void)
{
    struct task_struct *task;
    printk(KERN_INFO "Loading Module...\n");
    for_each_process(task)
    {
      printk(KERN_INFO "%s %d\n",
      			task->comm,
			task_pid_nr(task));
    }
    return 0;
}

void dfs_task(struct task_struct *this)
{
    struct task_struct *task;
    struct list_head *list;

    printk(KERN_INFO "%s %d\n",
     		      this->comm,
		      task_pid_nr(this));
    list_for_each(list, &this->children)
    {
    	task = list_entry(list, struct task_struct, sibling);
	dfs_task(task);

    }
}
void tasks_exit(void)
{
    printk(KERN_INFO "Removing Module...\n");
    dfs_task(&init_task);
}

module_init(tasks_init);
module_exit(tasks_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is the module task...\n");
MODULE_AUTHOR("wfs");
