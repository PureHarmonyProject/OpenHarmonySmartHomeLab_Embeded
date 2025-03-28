#ifndef BSP_PWM_H
#define BSP_PWM_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"



//函数声明
void pwm_init(void);
void pwm_set_duty(uint16_t duty);

#endif
