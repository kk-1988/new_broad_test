#include "led_reource.h"

static struct led_resource board_imx6ull_qemu_led = {
	.pin = GROUP_PIN(5,3),
};

struct led_resource *led_resource_get(void)
{
	return &board_imx6ull_qemu_led;
}