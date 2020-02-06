#ifndef __LED_DRV_H__
#define __LED_DRV_H__

#include "led_opr.h"

void led_device_create(int minor);
void led_device_destroy(int minor);
void led_operations_register(struct led_operations *opr);

#endif
