#ifndef BSP_DC_MOTOR_H
#define BSP_DC_MOTOR_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

//管脚定义
#define DC_MOTOR_PIN1         HI_IO_NAME_GPIO_2
#define DC_MOTOR_GPIO2_FUN    HI_IO_FUNC_GPIO_2_PWM2_OUT

#define DC_MOTOR_PIN2         HI_IO_NAME_GPIO_6
#define DC_MOTOR_GPIO6_FUN    HI_IO_FUNC_GPIO_6_PWM3_OUT

#define DC_MOTOR_LOW 0
#define DC_MOTOR_MEDIUM 1
#define DC_MOTOR_HIGH 2

//函数声明
// void airConditioner_init(void);
void airConditioner_send_cmd(uint8_t switch_state, uint8_t speed, uint8_t mode);
void airConditioner_heat(uint8_t speed);
void airConditioner_cool(uint8_t speed);
void airConditioner_stop(void);
void airConditioner_work(uint8_t airConditioner_state);
uint8_t airConditioner_get_curstate(void);
void airConditioner_set_curstate(uint8_t state);
#endif
