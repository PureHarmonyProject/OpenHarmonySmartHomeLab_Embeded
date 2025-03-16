#ifndef BSP_LED_H
#define BSP_LED_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

//警告管脚定义
#define LED_PIN         HI_IO_NAME_GPIO_2
#define LED_GPIO_FUN    HI_IO_FUNC_GPIO_2_GPIO

#define LED(a)          hi_gpio_set_ouput_val(LED_PIN,a)

#define PF_LED_INDEX    14
#define PF_LED(a)         pcf8575_write_bit(PF_LED_INDEX,a)

#define LED_WARNING_COUNT  15
#define LED_DELAY_TIME_MS  100
//照明灯引脚定义

//函数声明
void led_init(void);
void led_on(void);
void led_off(void); 
void led_warning(void);
void led_set_color_and_brightness(uint32_t led_color_and_brightness);
uint32_t led_get_color_and_brightness(void);
void adjust_led_brightness_by_light_sensor(void);
#endif
