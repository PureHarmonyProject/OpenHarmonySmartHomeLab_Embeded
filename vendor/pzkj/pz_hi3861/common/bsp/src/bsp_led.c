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
#include "hi_pwm.h"

//LED初始化
void led_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_func(LED_PIN, HI_IO_FUNC_GPIO_2_PWM2_OUT);       // 设置IO为pwm功能
    hi_gpio_set_dir(LED_PIN, HI_GPIO_DIR_OUT);                 // 设置pwm为输出模式
    hi_pwm_init(HI_PWM_PORT_PWM2);
}

static void pwm_set_duty(uint16_t duty)
{
    hi_pwm_start(HI_PWM_PORT_PWM2,duty,40000);//指定分频系数为40000 可认为重装载值就是40000 duty为ccr
}

//三档亮度 低 中 高
void led_on(type) 
{
    if(type < LED_LOW || type > LED_HIGH) {
        return;
    }
    pwm_set_duty(led_array[type]);
}

void led_off(void) 
{
    pwm_set_duty(0);
}

void led_warning(void)
{
    uint32_t count = LED_WARNING_COUNT;
    while(count--)
    {
        led_on(LED_HIGH);
        usleep(LED_DELAY_TIME_MS * 1000);
        led_off();
        usleep(LED_DELAY_TIME_MS * 1000);
    }
}
