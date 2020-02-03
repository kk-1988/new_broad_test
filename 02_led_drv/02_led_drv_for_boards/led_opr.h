#ifndef __LED_OPR_H__
#define __LED_OPR_H__

struct led_operations{
	int (*init) (int which);	/* 初始化LED，which-哪个LED */
	int	(*ctl)(int which, char status);	/* 控制LED，which-哪个LED，status:1 亮 0 灭*/
	char (*read) (int which);	/* 读取led的状态值 */
};

/*
* 获得单板相关的结构体指针
*/
struct led_operations *board_demo_opr_get(void);

#endif
