#ifndef BSP_KEY_H
#define BSP_KEY_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"


//管脚定义
#define KEY1_PIN         HI_IO_NAME_GPIO_11
#define KEY1_GPIO_FUN    HI_IO_FUNC_GPIO_11_GPIO

#define KEY2_PIN         HI_IO_NAME_GPIO_12
#define KEY2_GPIO_FUN    HI_IO_FUNC_GPIO_12_GPIO


//使用宏定义独立按键按下的键值
#define KEY1_PRESS  1
#define KEY2_PRESS  2
#define KEY_UNPRESS 0 


//函数声明
void key_init(void);
uint8_t key_scan(uint8_t mode);

#endif
