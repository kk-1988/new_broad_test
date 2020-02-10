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

static int major = 0;

static struct button_operations *p_button_opr = NULL;
static struct class *button_class;

static int button_open (struct inode *node, struct file *file)
{
	int minor = iminor(node);

	p_button_opr->init(minor);
	return 0;
}

static ssize_t button_read (struct file *file, char __user *user, size_t size, loff_t *offset)
{
	struct inode *inode = file_inode(file);
	int minor = iminor(inode);
	int level;
	int err;
	level = p_button_opr->read(minor);
	err = copy_to_user(user, &level, sizeof(int));
	return sizeof(int);
}

static struct file_operations button_fops = {
	.open = button_open,
	.read = button_read,
};

void button_operations_register(struct button_operations *opr)
{
	int i;
	p_button_opr = opr;

	for(i = 0;i < p_button_opr->count; i++)
	{
		device_create(button_class, NULL, MKDEV(major, i), NULL, "kxb_button%d",i);
	}
}
EXPORT_SYMBOL(button_operations_register);

void button_operations_unregister(void)
{
	int i;

	for(i = 0;i < p_button_opr->count; i++)
		device_destroy(button_class, MKDEV(major, i));
}
EXPORT_SYMBOL(button_operations_unregister);

static int __init button_init(void)
{
	int i;
	int err;
	major = register_chrdev(0,"kxb_button", &button_fops);

	button_class = class_create(THIS_MODULE, "kxb_button_class");
	err = PTR_ERR(button_class);
	if (IS_ERR(button_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "kxb_button");
		return -1;
	}

	return 0;
}

static void __exit button_exit(void)
{
	class_destroy(button_class);
	unregister_chrdev(major, "kxb_button");
}

module_init(button_init);
module_exit(button_exit);
MODULE_LICENSE("GPL");