/**
 ****************************************************************************************************
 * @file        bsp_dc_motor.c
 * @author      普中科技
 * @version     V1.0
 * @date        2024-06-05
 * @brief       直流电机实验
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

#include "bsp_dc_motor.h"
#include "hi_pwm.h"

//直流电机初始化
void dc_motor_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_func(DC_MOTOR_PIN, DC_MOTOR_GPIO_FUN);           // 设置IO为pwm模式
    hi_gpio_set_dir(DC_MOTOR_PIN, HI_GPIO_DIR_OUT);            // 设置GPIO为输出模式
    hi_pwm_init(HI_PWM_PORT_PWM3);
}

//pwm占空比输出调节
void pwm_set_duty(uint16_t duty)
{
    hi_pwm_start(HI_PWM_PORT_PWM3,duty,40000);//指定分频系数为40000 可认为重装载值就是40000 duty为ccr
}

//用pwm实现电机调速
//只分三个档 低 中 高速
void dc_motor_run(int type)
{
    if(type < DC_MOTOR_LOW || type > DC_MOTOR_HIGH) {
        return;
    }
    pwm_set_duty(speed_array[type]);
}