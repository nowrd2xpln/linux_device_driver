/*  
 *  hello_driver.c - The simplest kernel module.
 */
#include <linux/init.h>
#include <linux/module.h>	// Needed by all modules
#include <linux/kernel.h>	// Needed for KERN_INFO

MODULE_AUTHOR("Allen Mamaril");      // The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux driver for the BBB.");  // The description -- see modinfo
MODULE_VERSION("0.1");              // The version of the module
MODULE_LICENSE("gpl");

int __init km_init(void)
{
	printk(KERN_INFO "Hello world from Kernel Land!\n");

	// A non 0 return means init_module failed; module can't be loaded.
	return 0;
}

void __exit km_exit(void)
{
	printk(KERN_INFO "Goodbye world\n");
}

module_init(km_init);
module_exit(km_exit);
