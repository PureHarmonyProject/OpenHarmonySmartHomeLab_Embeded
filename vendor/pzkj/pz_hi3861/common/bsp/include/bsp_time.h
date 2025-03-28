#ifndef BSP_TIME_H
#define BSP_TIME_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"



//函数声明
void time_init(uint16_t times);
void time_onoff(uint8_t sta);

#endif
