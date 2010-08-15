#include <linux/module.h>		/* needed by all modules */
#include <linux/kernel.h>		/* printk: 8 level, DEFAULT_MESSAGE_LOGLEVEL */

/* info:
 * printk will appear on console if priority is less than "int console_loglevel" */

int init_module (void)
{
	printk(KERN_ALERT "Hello world 1\n");

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_ALERT "Bye!\n");
}
