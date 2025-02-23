/**
 ****************************************************************************************************
 * @file        bsp_led.c
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

#include "bsp_led.h"

//LED初始化
void led_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_pull(LED_PIN, HI_IO_PULL_DOWN);                  // 设置GPIO下拉
    hi_io_set_func(LED_PIN, LED_GPIO_FUN);                      // 设置IO为GPIO功能
    hi_gpio_set_dir(LED_PIN, HI_GPIO_DIR_OUT);                 // 设置GPIO为输出模式
}

void led_on() 
{
    LED(1);
}

void led_off(void) 
{
    LED(0);
}

void led_warning(void)
{
    printf("准备开始闪烁\n");
    uint32_t count = LED_WARNING_COUNT;
    while(count--)
    {
        led_on();
        usleep(LED_DELAY_TIME_MS * 1000);
        led_off();
        usleep(LED_DELAY_TIME_MS * 1000);
    }
    printf("闪烁完毕\n");
}

void led_on_by_pcf8575() 
{
    LED(1);
}

void led_off_by_pcf8575(void) 
{
    LED(0);
}