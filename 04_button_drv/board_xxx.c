#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>

#include "button_drv.h"

static void board_xxx_button_init_gpio(int which)
{
	printk("%s %s %d, init gpio for button %d\n",__FILE__, __FUNCTION__,__LINE__,which);
}

static int board_xxx_button_read_gpio(int which)
{
	printk("%s %s %d, read gpio for button %d\n",__FILE__, __FUNCTION__,__LINE__,which);
	//默认为高电平
	return 1;
}

static struct button_operations my_buttons_ops = {
	.count = 2,
	.init = board_xxx_button_init_gpio,
	.read = board_xxx_button_read_gpio,
};

static int __init board_xxx_button_init(void)
{
	button_operations_register(&my_buttons_ops);
	return 0;
}

static void __exit board_xxx_button_exit(void)
{
	button_operations_unregister();
}

module_init(board_xxx_button_init);
module_exit(board_xxx_button_exit);
MODULE_LICENSE("GPL");
