#include <linux/module.h>		/* needed by all modules */
#include <linux/kernel.h>		/* printk: 8 level, DEFAULT_MESSAGE_LOGLEVEL */
#include <linux/init.h>         /* for macro module_init(), module_exit() */

/* info:
 * printk will appear on console if priority is less than "int console_loglevel" */

/* __initdata will ask kernel to free the data after init_module */
static int data __initdata = 3;

/* __init & __exit ask kernel to destroy this function once the functions finish */
static int __init hello_3_init (void)
{
	printk(KERN_ALERT "Hello world 1\n");

	return 0;
}

static int __exit hello3_exit (void)
{
	printk(KERN_ALERT "Bye!\n");
}

module_init(hello_3_init);
module_exit(hello3_exit);
