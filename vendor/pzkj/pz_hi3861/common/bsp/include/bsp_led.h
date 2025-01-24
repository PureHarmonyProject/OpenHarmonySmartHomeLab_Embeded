/**
 ****************************************************************************************************
 * @file        bsp_led.h
 * @author      普中科技
 * @version     V1.0
 * @date        2024-06-05
 * @brief       LED实验
 * @license     Copyright (c) 2024-2034, 深圳市普中科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:普中-Hi3861
 * 在线视频:https://space.bilibili.com/2146492485
 * 公司网址:www.prechin.cn
 * 购买地址:
 *
 */

#ifndef BSP_LED_H
#define BSP_LED_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

//警告管脚定义
#define LED_PIN         HI_IO_NAME_GPIO_2
#define LED_GPIO_FUN    HI_IO_FUNC_GPIO_2_GPIO

#define LED(a)          hi_gpio_set_ouput_val(LED_PIN,a)

#define MAX_LED_DELAY_TIME 100
#define LED_WARNING_COUNT  15
#define LED_DELAY_TIME_MS  100

#define LED_LOW 0
#define LED_MEDIUM 1
#define LED_HIGH 2

int32_t led_array[] = {20000,30000,40000}; //速度数组
//照明灯引脚定义

//函数声明
void led_init(void);
void led_on(type);
void led_off(void); 
void led_warning(void);

#endif
