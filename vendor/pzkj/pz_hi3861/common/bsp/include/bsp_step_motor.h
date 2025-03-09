#ifndef BSP_STEP_MOTOR_H
#define BSP_STEP_MOTOR_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"
#include "bsp_pcf8575.h"


// 定义步进电机速度，值越小，速度越快
// 最小不能小于1
#define STEPMOTOR_MAXSPEED        1  
#define STEPMOTOR_MINSPEED        5  

//管脚定义
#define IN1_PIN             HI_IO_NAME_GPIO_1
#define IN1_GPIO_FUN        HI_IO_FUNC_GPIO_1_GPIO

#define IN2_PIN             HI_IO_NAME_GPIO_14
#define IN2_GPIO_FUN        HI_IO_FUNC_GPIO_14_GPIO

#define IN3_PIN             HI_IO_NAME_GPIO_13
#define IN3_GPIO_FUN        HI_IO_FUNC_GPIO_13_GPIO

#define IN4_PIN             HI_IO_NAME_GPIO_7
#define IN4_GPIO_FUN        HI_IO_FUNC_GPIO_7_GPIO

#define STEP_MOTOR_BYTE           4
#define STEP_MOTOR_DIR_CLOCKWISE  1
#define STEP_MOTOR_SPEEP          1.8
#define STEP_MOTOR_ANGLE          180
#define STEP_MOTOR_START          1
#define STEP_MOTOR_STOP           0

#define MOTOR_IN1(a)        hi_gpio_set_ouput_val(IN1_PIN,a)
#define MOTOR_IN2(a)        hi_gpio_set_ouput_val(IN2_PIN,a)
#define MOTOR_IN3(a)        hi_gpio_set_ouput_val(IN3_PIN,a)
#define MOTOR_IN4(a)        hi_gpio_set_ouput_val(IN4_PIN,a)

#define PF_MOTOR_IN1_INDEX     7
#define PF_MOTOR_IN2_INDEX     6
#define PF_MOTOR_IN3_INDEX     5
#define PF_MOTOR_IN4_INDEX     4
#define PF_MOTOR_IN1(a)        pcf8575_write_bit(PF_MOTOR_IN1_INDEX,a)
#define PF_MOTOR_IN2(a)        pcf8575_write_bit(PF_MOTOR_IN2_INDEX,a)
#define PF_MOTOR_IN3(a)        pcf8575_write_bit(PF_MOTOR_IN3_INDEX,a)
#define PF_MOTOR_IN4(a)        pcf8575_write_bit(PF_MOTOR_IN4_INDEX,a)

//函数声明
void curtain_open(void);
void curtain_open_by_pcf8575(void);
void step_motor_init(void);
void step_motor_run(uint8_t step,uint8_t dir,uint8_t speed,uint16_t angle,uint8_t sta);
void step_motor_run_by_pcf8575(uint8_t step, uint8_t dir, uint8_t speed, uint16_t angle, uint8_t sta);
void curtain_open_angle(uint16_t angle);
void curtain_open_angle_by_pcf8575(uint16_t angle);
uint16_t curtain_get_curangle(void);
float curtain_get_curstate(void);

void curtain_test(void);
#endif
