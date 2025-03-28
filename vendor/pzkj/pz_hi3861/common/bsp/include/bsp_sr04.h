#ifndef BSP_SR04_H
#define BSP_SR04_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

//管脚定义
#define TRIG_PIN            HI_IO_NAME_GPIO_13
#define TRIG_GPIO_FUN       HI_IO_FUNC_GPIO_13_GPIO

#define ECHO_PIN            HI_IO_NAME_GPIO_14
#define ECHO_GPIO_FUN       HI_IO_FUNC_GPIO_14_GPIO

#define TRIG(a)             hi_gpio_set_ouput_val(TRIG_PIN,a)

//函数声明
void sr04_init(void);
float sr04_read_distance(void);

#endif
