#ifndef __LED_RESOURSE_H__
#define __LED_RESOURSE_H__

#define GROUP(x) (x >> 16)
#define PIN(x) (x & 0xFFFF)
#define GROUP_PIN(g,p)	(g << 16 | (p))

/*
* Ibit[31:16] = group
* Lbit[15:0] = which pin
*/
struct led_resource {
	int pin;
};

struct led_resource *led_resource_get(void);
#endif
