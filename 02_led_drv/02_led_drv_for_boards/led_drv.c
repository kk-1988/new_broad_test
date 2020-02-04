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
#include "led_opr.h"

struct led_operations	*p_led_opr = NULL;

/* 1.设置主设备号                                                              */
static int major = 0;
static struct class *led_class;

#define MIN(a, b) (a < b ? a : b)

/* 3. 实现对应的open/read/write等函数，填入file_operations结构体                   */
//课后练习，即在这里进行读取即可
static ssize_t led_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	int err;
	char status;
	struct inode *inode = file_inode(file);
	int minor = iminor(inode);
	status = p_led_opr->read(minor);
	
	err = copy_to_user(buf, &status, 1);
	return 1;
}

static ssize_t led_drv_write (struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
	int err;
	char status;
	struct inode *inode = file_inode(file);
	int minor = iminor(inode);
	
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_from_user(&status, buf, 1);
	/* 根据次设备号和status来控制LED */
	p_led_opr->ctl(minor,status);
	
	return 1;
}

static int led_drv_open (struct inode *node, struct file *file)
{
	int minor = iminor(node);

	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* 根据次设备号来初始化LED */
	p_led_opr->init(minor);

	return 0;
}

static int led_drv_close (struct inode *node, struct file *file)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* 调用iounremap 释放相关资源 */
	p_led_opr->close();
	
	return 0;
}

/* 2. 定义自己的file_operations结构体                                              */
static struct file_operations led_drv = {
	.owner	 = THIS_MODULE,
	.open    = led_drv_open,
	.read    = led_drv_read,
	.write   = led_drv_write,
	.release = led_drv_close,
};

/* 4. 把file_operations结构体告诉内核：注册驱动程序                                */
/* 5. 谁来注册驱动程序啊？得有一个入口函数：安装驱动程序时，就会去调用这个入口函数 */
static int __init led_init(void)
{
	int err;
	int i;

	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	major = register_chrdev(0, "kxb_led", &led_drv);  /* /dev/kxb_led */

	led_class = class_create(THIS_MODULE, "led_class");
	err = PTR_ERR(led_class);
	if (IS_ERR(led_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "kxb_led");
		return -1;
	}

	p_led_opr = board_demo_opr_get();

	for(i = 0;i < p_led_opr->num; i++)
		device_create(led_class, NULL, MKDEV(major, i), NULL, "kxb_led%d",i); /* /dev/kxb_ledn */
	
	return 0;
}

/* 6. 有入口函数就应该有出口函数：卸载驱动程序时，就会去调用这个出口函数           */
static void __exit led_exit(void)
{
	int i;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	/* 调用iounremap 释放相关资源 */
	p_led_opr->close();
	
	for(i = 0;i < p_led_opr->num; i++)
		device_destroy(led_class, MKDEV(major, i)); /* /dev/kxb_ledn */
	
	class_destroy(led_class);
	unregister_chrdev(major, "kxb_led");
}


/* 7. 其他完善：提供设备信息，自动创建设备节点                                     */

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");


