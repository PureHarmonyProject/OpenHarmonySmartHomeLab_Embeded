#ifndef BSP_DC_MOTOR_H
#define BSP_DC_MOTOR_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

//管脚定义
#define DC_MOTOR_PIN         HI_IO_NAME_GPIO_6
#define DC_MOTOR_GPIO_FUN    HI_IO_FUNC_GPIO_6_PWM3_OUT


#define DC_MOTOR_LOW 0
#define DC_MOTOR_MEDIUM 1
#define DC_MOTOR_HIGH 2

//函数声明
void dc_motor_init(void);
void dc_motor_run(int type);

#endif
