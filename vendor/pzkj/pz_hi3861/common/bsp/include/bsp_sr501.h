/**
 ****************************************************************************************************
 * @file        bsp_sr501.c
 * @author      Dukiya
 * @version     V1.0
 * @date        2025/1/16
 * @brief       HC-SR501人体红外感应模块
 ****************************************************************************************************
*/

#ifndef BSP_SR501_H
#define BSP_SR501_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

//管脚定义
#define SR_501_PIN            HI_IO_NAME_GPIO_12
#define SR_501_GPIO_FUN       HI_IO_FUNC_GPIO_12_GPIO

//函数声明
void sr501_init(void);
// int sr501_read(void);

int sr501_check(void);
#endif
