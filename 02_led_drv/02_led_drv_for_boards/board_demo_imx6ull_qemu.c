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

	if(0 == which)
	{
		/* 1.enable GPIO5 */
		*CCM_CCGR1 =(3 << 30);

		/* 2. set GPIOS as GPIO */
		*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = 0x5;

		/* 3. set GPIOS as GPIO */
		p_gpio5->gdir |= (1<<3);
	}
	else if(1 == which)
	{
		/* 1.enable GPIO5 */
		*CCM_CCGR1 =(3 << 26);

		/* 2. set GPIOS as GPIO */
		p_iomux->IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03 = 0x5;

		/* 3. set GPIOS as GPIO */
		p_gpio1->gdir |= (1<<3);
	}
	else if(2 == which)
	{
		/* 1.enable GPIO5 */
		*CCM_CCGR1 =(3 << 26);

		/* 2. set GPIOS as GPIO */
		p_iomux->IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO05 = 0x5;

		/* 3. set GPIOS as GPIO */
		p_gpio1->gdir |= (1<<5);
	}
	else if(3 == which)
	{
		/* 1.enable GPIO5 */
		*CCM_CCGR1 =(3 << 26);

		/* 2. set GPIOS as GPIO */
		p_iomux->IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO06 = 0x5;

		/* 3. set GPIOS as GPIO */
		p_gpio1->gdir |= (1<<6);
	}

	return 0;
}

/*
* 指定哪盏灯进行控制,status 1 打开 0 关闭
*/
static int borad_demo_led_ctl(int which, char status)
{
	//printk("%s %s line %d, led %d  %d %s\n",__FILE__,__FUNCTION__,__LINE__, which, status, status ? "on":"off");
#if 0
	if(0 == which)
	{
		if(status)
		{
			p_gpio5->dr &= ~(1<<3);
		}
		else
		{
			p_gpio5->dr |= (1 << 3);
		}
	}
	else if(1 == which)
	{
		if(status)
		{
			p_gpio1->dr &= ~(1<<3);
		}
		else
		{
			p_gpio1->dr |= (1 << 3);
		}
	}
	else if(2 == which)
	{
		if(status)
		{
			p_gpio1->dr &= ~(1<<5);
		}
		else
		{
			p_gpio1->dr |= (1 << 5);
		}
	}
	else if(3 == which)
	{
		if(status)
		{
			p_gpio1->dr &= ~(1<<6);
		}
		else
		{
			p_gpio1->dr |= (1 << 6);
		}
	}
#else
	switch(which)
	{
		case 0:
			if(status)
			{
				p_gpio5->dr &= ~(1<<3);
			}
			else
			{
				p_gpio5->dr |= (1 << 3);
			}
		break;
			
		case 1:
			if(status)
			{
				p_gpio1->dr &= ~(1<<3);
			}
			else
			{
				p_gpio1->dr |= (1 << 3);
			}
		break;

		case 2:
			if(status)
			{
				p_gpio1->dr &= ~(1<<5);
			}
			else
			{
				p_gpio1->dr |= (1<<5);
			}
		break;

		case 3:
			if(status)
			{
				p_gpio1->dr &= ~(1<<6);
			}
			else
			{
				p_gpio1->dr |= (1<<6);
			}
		break;
	}
#endif
	
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

static void board_demo_led_close(void)
{
	printk("%s %s line %d\n",__FILE__,__FUNCTION__,__LINE__);
	
	if(p_gpio5)
	{
		iounmap(p_gpio5);
		p_gpio5 = NULL;
	}

	if(p_gpio1)
	{
		iounmap(p_gpio1);
		p_gpio1 = NULL;
	}

	if(p_iomux)
	{
		iounmap(p_iomux);
		p_iomux = NULL;
	}

	if(IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3)
	{
		iounmap(IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3);
		IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = NULL;
	}

	if(CCM_CCGR1)
	{
		iounmap(CCM_CCGR1);
		CCM_CCGR1 = NULL;
	}
}

static struct led_operations board_demo_led_opr = {
	.init = board_demo_led_init,
	.ctl = borad_demo_led_ctl,
	.read = borad_demo_led_read,
	.close = board_demo_led_close,
	.num = 4,
};

struct led_operations *board_demo_opr_get(void)
{
	return &board_demo_led_opr;
}