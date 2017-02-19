/*  
 *  auto_char_dev.c - The simplest kernel module.
 */

#include <linux/init.h>		// Needed by init/exit macros
#include <linux/module.h>	// Needed by all modules
#include <linux/kernel.h>	// Needed for KERN_INFO
#include <linux/fs.h>		// Needed for file operations

MODULE_AUTHOR("Allen Mamaril");      // The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux driver for the BBB.");  // The description -- see modinfo
MODULE_VERSION("0.1");              // The version of the module
MODULE_LICENSE("gpl");

static int dev_file_mj_num = 0;
static const char dev_name[] = "Simplest-driver";

static struct file_operations simplest_driver_fops =
{
	.owner	= THIS_MODULE,
};

int __init km_init(void)
{
	int ret = 0;

	printk(KERN_NOTICE "%s: Registering Device\n", __FUNCTION__);
	ret = register_chrdev(0, dev_name, &simplest_driver_fops);

	if(ret < 0)
	{
		printk(KERN_WARNING "%s: Character device registration failed ERROR %d\n", __FUNCTION__, ret);
	}

	dev_file_mj_num = ret;
	printk(KERN_NOTICE "%s: Character Device assigned Major Number %d\n", __FUNCTION__, dev_file_mj_num);

	return 0;
}

void __exit km_exit(void)
{
	printk(KERN_INFO "%s: Unregistering Character Device\n", __FUNCTION__);

	if(dev_file_mj_num != 0)
	{
		unregister_chrdev(dev_file_mj_num, dev_name);
	}

	printk(KERN_INFO "%s: Goodbye Kernal Land!\n", __FUNCTION__);
}

module_init(km_init);
module_exit(km_exit);
