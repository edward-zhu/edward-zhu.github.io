#define _GNU_SOURCE
#define MODULE_NAME "pfcount"


#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/seq_file.h>
#include <linux/module.h>

#define MODULE_VERSION "1.0"

extern unsigned long volatile jiffies, pfcount;

int pfcount_proc_show(struct seq_file * m, void * v) {
	seq_printf(m, "jiffies: %lu, pfcount: %lu\n", jiffies, pfcount);
	printk("pfcount showed.\n");
	return 0;
}

int pfcount_proc_open(struct inode * inode, struct file * file) {
	return single_open(file, pfcount_proc_show, NULL);
}

struct proc_dir_entry * proc_pfcount;

const struct file_operations pfcount_proc_fops = {
	.owner = THIS_MODULE,
	.open = pfcount_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

int __init init_pfcount(void)
{
	printk("pfcount: init_module()\n");

	proc_pfcount = proc_create("pfcount", 0, NULL, &pfcount_proc_fops);
	printk(KERN_INFO"%s %s has initialized.\n", MODULE_NAME, MODULE_VERSION);

	return 0;
}

void __exit cleanup_pfcount(void)
{
	printk("pfcount: cleanup_pfcount()\n");
	remove_proc_entry("pfcount", NULL);
	printk(KERN_INFO"%s %s has removed.\n", MODULE_NAME, MODULE_VERSION);
}

module_init(init_pfcount);

module_exit(cleanup_pfcount);
