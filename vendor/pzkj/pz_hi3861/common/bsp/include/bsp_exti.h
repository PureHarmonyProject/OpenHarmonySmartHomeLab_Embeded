#ifndef BSP_EXTI_H
#define BSP_EXTI_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"


//管脚定义
#define KEY1_PIN         HI_IO_NAME_GPIO_11
#define KEY1_GPIO_FUN    HI_IO_FUNC_GPIO_11_GPIO

#define KEY2_PIN         HI_IO_NAME_GPIO_12
#define KEY2_GPIO_FUN    HI_IO_FUNC_GPIO_12_GPIO


//函数声明
void exti_init(void);

#endif
