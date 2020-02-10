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
#include <linux/io.h>

#include "button_drv.h"

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
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO09;		//80
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA;	//84
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA;	//88
	volatile unsigned int IOMUXC_SW_MUX_CTL_PAD_UART1_CTS_B;	//8c=> GPIO1_18
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
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER1;
static struct iomux *p_iomux;
static struct imx6ull_gpio *p_gpio1;
static struct imx6ull_gpio *p_gpio5;

static void board_imx6ull_button_init_gpio(int which)
{
	//printk("%s %s line %d, led %d\n",__FILE__,__FUNCTION__,__LINE__, which);
	if(!CCM_CCGR1)
	{
		CCM_CCGR1 = ioremap(0x20c406c, 4);
		IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER1 = ioremap(0x229000c, 4);
		p_iomux = ioremap(0x20e0000,sizeof(struct iomux));
		p_gpio1 = ioremap(0x209c000,sizeof(struct imx6ull_gpio));
		p_gpio5 = ioremap(0x20ac000,sizeof(struct imx6ull_gpio));
	}

	if(0 == which)
	{
		/* 1.enable GPIO5 */
		*CCM_CCGR1 =(3 << 30);

		/* 2. set GPIOS as GPIO */
		*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER1 = 0x5;

		/* 3. set GPIOS as input */
		p_gpio5->gdir &= ~(1<<1);
	}
	else if(1 == which)
	{
		/* 1.enable GPIO1 */
		*CCM_CCGR1 =(3 << 26);

		/* 2. set GPIOS as GPIO */
		p_iomux->IOMUXC_SW_MUX_CTL_PAD_UART1_CTS_B = 0x5;

		/* 3. set GPIOS as input */
		p_gpio1->gdir &= ~(1<<18);
	}

	return 0;
}

static int board_imx6ull_button_read_gpio(int which)
{
	if(which == 0)
		return (p_gpio5->psr & (1 << 1))?1:0;
	else
		return (p_gpio1->psr & (1 << 18))?1:0;
}

static struct button_operations my_buttons_ops = {
	.count = 2,
	.init = board_imx6ull_button_init_gpio,
	.read = board_imx6ull_button_read_gpio,
};

static int __init board_imx6ull_button_init(void)
{
	button_operations_register(&my_buttons_ops);
	return 0;
}

static void __exit board_imx6ull_button_exit(void)
{
	button_operations_unregister();
}

module_init(board_imx6ull_button_init);
module_exit(board_imx6ull_button_exit);
MODULE_LICENSE("GPL");
