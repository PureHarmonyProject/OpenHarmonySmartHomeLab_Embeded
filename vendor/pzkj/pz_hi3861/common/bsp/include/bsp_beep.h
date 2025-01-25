#ifndef BSP_BEEP_H
#define BSP_BEEP_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

//管脚定义
#define BEEP_PIN         HI_IO_NAME_GPIO_8
#define BEEP_GPIO_FUN    HI_IO_FUNC_GPIO_8_GPIO

#define BEEP(a)          hi_gpio_set_ouput_val(BEEP_PIN,a)

#define BEEP_WARNING_COUNT 100
#define BEEP_WARNING_TIME 250   //2khz
//函数声明
void beep_init(void);
void beep_alarm(uint16_t cnt,uint16_t time);
void beep_warning(void);
#endif
