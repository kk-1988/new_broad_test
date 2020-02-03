#include <linux/gfp.h>
#include <linux/io.h>
#include "led_opr.h"

struct iomux{
	volatile unsigned int unnames[23];	//23 * 4 = 92 = 0x5c
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO00;	/* offset 0x5c */
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO01;
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO02;
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03;
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO04;
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO05;
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO06;
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO07;
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO08;
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO09;
};

struct imx6ull_gpio{
	volatile unsigned int dr;
	volatile unsigned int gdir;
	volatile unsigned int psr;
	volatile unsigned int icr1;
	volatile unsigned int icr2;
	volatile unsigned int imr;
	volatile unsigned int isr;
	volatile unsigned int edge_sel;
};

static volatile unsigned int *CCM_CCGR1;
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
static struct iomux *p_iomux;
static struct imx6ull_gpio *p_gpio1;
static struct imx6ull_gpio *p_gpio5;

/*
* 初始化LED，which - 哪个led
*/
static int board_demo_led_init(int which)
{
	//printk("%s %s line %d, led %d\n",__FILE__,__FUNCTION__,__LINE__, which);
	if(!CCM_CCGR1)
	{
		CCM_CCGR1 = ioremap(0x20c406c, 4);
		IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x2290014, 4);
		p_iomux = ioremap(0x20e0000,sizeof(struct iomux));
		p_gpio1 = ioremap(0x209c000,sizeof(struct imx6ull_gpio));
		p_gpio5 = ioremap(0x20ac000,sizeof(struct imx6ull_gpio));
	}

	
	
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
	.num = 4,
};

struct led_operations *board_demo_opr_get(void)
{
	return &board_demo_led_opr;
}