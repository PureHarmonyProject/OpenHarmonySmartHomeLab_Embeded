#ifndef BSP_SR501_H
#define BSP_SR501_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

//管脚定义
#define SR_501_PIN            HI_IO_NAME_GPIO_2
#define SR_501_GPIO_FUN       HI_IO_FUNC_GPIO_2_GPIO

//函数声明
void sr501_init(void);
// int sr501_read(void);

int sr501_check(void);
adjust_led_brightness_by_motion_sensor();
#endif
