#include <linux/export.h>
#include <linux/mm.h>
#include <linux/utsname.h>
#include <linux/mman.h>
#include <linux/reboot.h>
#include <linux/prctl.h>
#include <linux/highuid.h>
#include <linux/fs.h>
#include <linux/kmod.h>
#include <linux/perf_event.h>
#include <linux/resource.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/capability.h>
#include <linux/device.h>
#include <linux/key.h>
#include <linux/times.h>
#include <linux/posix-timers.h>
#include <linux/security.h>
#include <linux/dcookies.h>
#include <linux/suspend.h>
#include <linux/tty.h>
#include <linux/signal.h>
#include <linux/cn_proc.h>
#include <linux/getcpu.h>
#include <linux/task_io_accounting_ops.h>
#include <linux/seccomp.h>
#include <linux/cpu.h>
#include <linux/personality.h>
#include <linux/ptrace.h>
#include <linux/fs_struct.h>
#include <linux/file.h>
#include <linux/mount.h>
#include <linux/gfp.h>
#include <linux/syscore_ops.h>
#include <linux/version.h>
#include <linux/ctype.h>

#include <linux/compat.h>
#include <linux/syscalls.h>
#include <linux/kprobes.h>
#include <linux/user_namespace.h>
#include <linux/binfmts.h>

#include <linux/sched.h>
#include <linux/rcupdate.h>
#include <linux/uidgid.h>
#include <linux/cred.h>

#include <linux/kmsg_dump.h>
/* Move somewhere else to avoid recompiling? */
#include <generated/utsrelease.h>

#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/unistd.h>

/* my code */

typedef struct __myevent {
	int eventNum;
	wait_queue_head_t *p;
	struct __myevent * next;
} myevent_t;

myevent_t * lpmyevent_head = NULL, * lpmyevent_end = NULL;


void print_event_list(myevent_t * head)
{
	printk(KERN_DEBUG"event_list: ");
	while(head) {
		printk(" %d", head->eventNum);
		head = head->next;
	}
	printk("\n");
	return 0;
}

myevent_t * scheventNum(int eventNum, myevent_t **prev)
{
	myevent_t * tmp = lpmyevent_head;
	*prev = NULL;
	while(tmp)
	{
		if (tmp->eventNum == eventNum)
		{
			return tmp;
		}
		*prev = tmp;
		tmp = tmp->next;
	}
	return NULL;
}

SYSCALL_DEFINE1(eventopen, int, eventNum)
{
	myevent_t * new;
	myevent_t * prev;
	if (eventNum)
	{
		if (!scheventNum(eventNum, &prev))
		{
			return -1;
		}
		else {
			return eventNum;
		}
	}
	else
	{
		new = (myevent_t *)kmalloc(sizeof(myevent_t), GFP_KERNEL);
		new->p = (wait_queue_head_t *)kmalloc(sizeof(wait_queue_head_t), GFP_KERNEL);
		init_waitqueue_head(new->p);
		new->next = NULL;
		if (!lpmyevent_head)
		{
			new->eventNum = 2;
			lpmyevent_head = lpmyevent_end = new;
			return new->eventNum;
		}
		else
		{
			new->eventNum = lpmyevent_end->eventNum + 2;
			lpmyevent_end->next = new;
			lpmyevent_end = new;
		}
		return new->eventNum;
	}
	return 0;
}

int do_eventsig(int eventNum)
{
	myevent_t * tmp = NULL;
	myevent_t * prev = NULL;

	if (!(tmp = scheventNum(eventNum, &prev))) {
		return 0;
	}
	wake_up(tmp->p);
	return 1;
}

SYSCALL_DEFINE1(eventsig, int, eventNum)
{
	return do_eventsig(eventNum);
}

SYSCALL_DEFINE1(eventwait, int, eventNum)
{
	myevent_t * tmp = NULL;
	myevent_t * prev = NULL;

	if ((tmp = scheventNum(eventNum, &prev)))
	{
		printk("DECLARE waitqueue.\n");
		DECLARE_WAITQUEUE(wait, current);
		printk("set state.\n");
		set_current_state(TASK_UNINTERRUPTIBLE);
		printk("add wait queue\n");
		add_wait_queue(tmp->p, &wait);
		printk("schedule()\n");
		schedule();

		remove_wait_queue(tmp->p, &wait);
	}

	return 0;
}

SYSCALL_DEFINE1(eventclose, int, eventNum)
{
	myevent_t * prev = NULL;
	myevent_t * releaseItem = NULL;

	do_eventsig(eventNum);
	printk("before delete.\n");
	print_event_list(lpmyevent_head);

	if((releaseItem = scheventNum(eventNum, &prev)))
	{
		printk("ready to delete: %d\n", eventNum);
		if (releaseItem == lpmyevent_end)
		{
			lpmyevent_end = prev;
		}
		if (releaseItem == lpmyevent_head)
		{
			lpmyevent_head = lpmyevent_head->next;
			goto wake;
		}

		prev->next = releaseItem->next;
	}
wake:
	if (releaseItem)
	{
		kfree(releaseItem);

		print_event_list(lpmyevent_head);
		return 1;
	}
	printk("after delete.\n");
	printk("can't find : %d\n", eventNum);


	return 0;
}

SYSCALL_DEFINE0(eventshow)
{
	print_event_list(lpmyevent_head);

	return 0;
}




/* my code end */