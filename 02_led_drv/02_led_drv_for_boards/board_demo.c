#include <linux/gfp.h>
#include "led_opr.h"

/*
* 初始化LED，which - 哪个led
*/
static int board_demo_led_init(int which)
{
	printk("%s %s line %d, led %d\n",__FILE__,__FUNCTION__,__LINE__, which);
	return 0;
}

/*
* 指定哪盏灯进行控制,status 1 打开 0 关闭
*/
static int borad_demo_led_ctl(int which, char status)
{
	printk("%s %s line %d, led %d  %d %s\n",__FILE__,__FUNCTION__,__LINE__, which, status, status ? "on":"off");
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
	.read = borad_demo_led_read,
	.num = 1,
};

struct led_operations *board_demo_opr_get(void)
{
	return &board_demo_led_opr;
}