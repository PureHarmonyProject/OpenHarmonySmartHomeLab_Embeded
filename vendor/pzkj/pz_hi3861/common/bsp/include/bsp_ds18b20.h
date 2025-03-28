#ifndef BSP_DS18B20_H
#define BSP_DS18B20_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

//管脚定义
#define DS18B20_PIN         HI_IO_NAME_GPIO_7
#define DS18B20_GPIO_FUN    HI_IO_FUNC_GPIO_7_GPIO

#define DS18B20_DQ_OUT(a)   hi_gpio_set_ouput_val(DS18B20_PIN,a)

//函数声明
void ds18b20_io_out(void);
void ds18b20_io_in(void);
void ds18b20_reset(void);
uint8_t ds18b20_check(void);
uint8_t ds18b20_read_bit(void);
uint8_t ds18b20_read_byte(void);
void ds18b20_write_byte(uint8_t dat);
void ds18b20_start(void);
uint8_t ds18b20_init(void);
float ds18b20_gettemperture(void);

#endif
