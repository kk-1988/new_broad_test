#include <linux/gfp.h>
#include <linux/module.h>
#include "led_opr.h"
#include "led_reource.h"
#include "led_drv.h"
#include <linux/platform_device.h>

static int g_ledpins[100];
static int g_ledcnt = 0;

/*
* 初始化LED，which - 哪个led
* 已知晓group和pin，则可以具体知道是哪个pin了，然后我们可以进行统一的操作了(针对某一款芯片)
*/
static int board_demo_led_init(int which)
{
	//printk("%s %s line %d, led %d\n",__FILE__,__FUNCTION__,__LINE__, which);

	printk("init gpio: group %d pin %d\n",GROUP(g_ledpins[which]),PIN(g_ledpins[which]));
	switch(GROUP(g_ledpins[which]))
	{
		case 0:
			printk("init pin of group 0\n");
		break;

		case 1:
			printk("init pin of group 1\n");
		break;

		case 2:
			printk("init pin of group 2\n");
		break;

		case 3:
			printk("init pin of group 3\n");
		break;
		
		case 4:
			printk("init pin of group 4\n");
		break;

		case 5:
			printk("init pin of group 5\n");
		break;

		case 6:
			printk("init pin of group 6\n");
		break;
	}

	
	return 0;
}

/*
* 指定哪盏灯进行控制,status 1 打开 0 关闭
*/
static int borad_demo_led_ctl(int which, char status)
{
	printk("%s %s line %d, led %d  %d %s\n",__FILE__,__FUNCTION__,__LINE__, which, status, status ? "on":"off");
	
	switch(PIN(g_ledpins[which]))
	{
		case 0:
			printk("init pin of 0\n");
		break;

		case 1:
			printk("init pin of 1\n");
		break;

		case 2:
			printk("init pin of 2\n");
		break;

		case 3:
			printk("init pin of 3\n");
		break;
		
		case 4:
			printk("init pin of 4\n");
		break;

		case 5:
			printk("init pin of 5\n");
		break;

		case 6:
			printk("init pin of 6\n");
		break;
	}
	
	return 0;
}

/*
* 读取实际灯的状态
*/
static char borad_demo_led_read(int which)
{
	printk("%s %s line %d, led %d\n",__FILE__,__FUNCTION__,__LINE__, which);
	return 1;
}

static struct led_operations board_demo_led_opr = {
	.init = board_demo_led_init,
	.ctl = borad_demo_led_ctl,
	.read=  borad_demo_led_read,
};

/*
* 当dev和drv匹配上之后，则我们进行probe，把相关引脚等资源信息从platform_device中获取出来
*/
static int board_demo_gpio_drv_probe(struct platform_device *dev)
{
	int i;
	struct resource *p_res = NULL;

	/*
	* 获得资源,因为不知道具体是存在哪些资源的
	*/
	while(1)
	{
		p_res = platform_get_resource(dev,IORESOURCE_IRQ, i++);
		if(!p_res)
			break;

		g_ledpins[g_ledcnt] = p_res->start;

		led_device_create(g_ledcnt);
		g_ledcnt++;
	}

	//device_create
	
	return 0;
}

static int board_demo_gpio_drv_remove(struct platform_device *dev)
{
	//device_destroy
	int i;
	for(i = 0; i < g_ledcnt;i++)
	{
		led_device_destroy(i);
	}

	g_ledcnt = 0;
	
	return 0;
}

static struct platform_driver board_demo_gpio_drv = {
	.probe		= board_demo_gpio_drv_probe,
	.remove		= board_demo_gpio_drv_remove,
	.driver		= {
		.name	= "kxb_led",
	},
};

static int __init board_demo_gpio_drv_init(void)
{
	int err;
	err = platform_driver_register(&board_demo_gpio_drv);
	led_operations_register(&board_demo_led_opr);
	return 0;
}

static void __exit board_demo_gpio_drv_exit(void)
{
	platform_driver_unregister(&board_demo_gpio_drv);
}

module_init(board_demo_gpio_drv_init);
module_exit(board_demo_gpio_drv_exit);
MODULE_LICENSE("GPL");