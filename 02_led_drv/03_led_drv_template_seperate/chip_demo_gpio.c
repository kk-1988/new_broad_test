#include <linux/gfp.h>
#include "led_opr.h"
#include "led_reource.h"

static struct led_resource *led_rsc;

/*
* 初始化LED，which - 哪个led
* 已知晓group和pin，则可以具体知道是哪个pin了，然后我们可以进行统一的操作了(针对某一款芯片)
*/
static int board_demo_led_init(int which)
{
	//printk("%s %s line %d, led %d\n",__FILE__,__FUNCTION__,__LINE__, which);
	if(!led_rsc)
	{
		led_rsc = led_resource_get();
	}

	printk("init gpio: group %d pin %d\n",GROUP(led_rsc->pin),PIN(led_rsc->pin));
	switch(GROUP(led_rsc->pin))
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
	
	switch(PIN(led_rsc->pin))
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

struct led_operations *board_demo_opr_get(void)
{
	return &board_demo_led_opr;
}