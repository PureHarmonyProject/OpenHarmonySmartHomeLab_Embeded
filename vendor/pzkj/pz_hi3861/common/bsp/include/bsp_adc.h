#ifndef BSP_ADC_H
#define BSP_ADC_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

//管脚定义
#define ADC5_PIN         HI_IO_NAME_GPIO_11
#define ADC2_PIN         HI_IO_NAME_GPIO_5
#define ADC6_PIN         HI_IO_NAME_GPIO_13
#define ADC3_PIN         HI_IO_NAME_GPIO_7
//函数声明
void adc5_init(void);
uint16_t get_adc5_value(void);
float get_adc5_voltage(void);

void adc6_init(void);
uint16_t get_adc6_value(void);
float get_adc6_voltage(void);

void adc2_init(void);
uint16_t get_adc2_value(void);
float get_adc2_voltage(void);

void adc3_init(void);
uint16_t get_adc3_value(void);
float get_adc3_voltage(void);
#endif
