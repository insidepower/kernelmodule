#include <linux/module.h>		/* needed by all modules */
#include <linux/kernel.h>		/* printk: 8 level, DEFAULT_MESSAGE_LOGLEVEL */
#include <linux/init.h>         /* for macro module_init(), module_exit() */
#include <linux/moduleparam.h>  /* for module_param() macro */
#include <linux/stat.h>         /* for permission bit: S_IRUSR ... */
#include "hello_call.h"			/* for modules span multiple file */

/* info:
 * printk will appear on console if priority is less than "int console_loglevel" */

/* declare global variable to be passed by user when module is loaded 
 * insmod xxx.ko myvar=5 myarray=5,6 mystr="haha" */
static int myvar = 3;
static char *mystr = "blah";
static int myarray[2] = {2,3};
static int arr_argc = 0;

/* module_param(var_name, var_type, permission) */
module_param(myvar, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
module_param(mystr, charp, 0000);
module_param_array(myarray, int, &arr_argc, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

/* __initdata will ask kernel to free the data after init_module */
static int data __initdata = 3;

/* __init & __exit ask kernel to destroy this function once the functions finish */
static int __init hello_3_init (void)
{
	int i=0;

	printk(KERN_ALERT "Hello world 1\n");

	/// param could be passed by user
	printk(KERN_INFO "myvar = %d\n", myvar);
	printk(KERN_INFO "mystr=%s\n", mystr);
	for (i=0; i < (sizeof myarray / sizeof(int)); i++)
		printk(KERN_INFO "myarray[%d]=%d\n", i, myarray[i]);
	printk(KERN_INFO "got %d arg for myarray\n", arr_argc);

	return 0;
}

static void __exit hello3_exit (void)
{
	myprint();
	printk(KERN_ALERT "Bye!\n");
}

module_init(hello_3_init);
module_exit(hello3_exit);
