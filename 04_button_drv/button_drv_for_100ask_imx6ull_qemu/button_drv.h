#ifndef __BUTTON_DRV_H__
#define __BUTTON_DRV_H__

struct button_operations{
	int count;					/* 说明有几个按键 */
	void (*init) (int which);	/* 初始化button */
	int (*read) (int which);	/* 读取button按键值 */
};

void button_operations_register(struct button_operations *opr);
void button_operations_unregister(void);
#endif

