#ifndef BSP_SG90_H
#define BSP_SG90_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"


//管脚定义
#define SG90_PIN         HI_IO_NAME_GPIO_5
#define SG90_GPIO_FUN    HI_IO_FUNC_GPIO_5_GPIO

#define DOOR_OPEN_ANGLE 180
#define DOOR_CLOSE_ANGLE 0

//函数声明
void sg90_init(void);
void set_sg90_angle(uint16_t angle);
int door_open(void);
int door_close(void);
uint8_t door_get_curstate(void);
void door_set_curstate(uint8_t cur_door_state);
#endif
