#ifndef BSP_INA219_H
#define BSP_INA219_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"
#include "hi_i2c.h"

// INA219 I2C 地址和配置
#define INA219_I2C_ADDR         0x40       // INA219 I2C从机地址
#define INA219_I2C_IDX          0          // 使用的I2C总线号
#define INA219_I2C_SPEED        100000     // I2C通信速度 100KHz

// INA219 寄存器地址
#define INA219_REG_CONFIG       0x00       // 配置寄存器
#define INA219_REG_SHUNTVALUE   0x01       // Shunt电压寄存器
#define INA219_REG_BUSVOLTAGE  0x02       // 总线电压寄存器
#define INA219_REG_POWER        0x03       // 功率寄存器
#define INA219_REG_CURRENT      0x04       // 电流寄存器
#define INA219_REG_CALIBRATION 0x05       // 校准寄存器

// 函数声明
void ina219_init(void);                        // 初始化INA219模块
void ina219_deinit(void);                      // 关闭INA219模块
void ina219_read_voltage(float *bus_voltage, float *shunt_voltage); // 读取电压
int16_t ina219_read_current(void);    // 读取电流
void ina219_read_power(float *power);         // 读取功率
void ina219_calibrate(void);                   // 校准INA219
void ina219_write_register(uint8_t reg, uint16_t value); // 写寄存器
uint16_t ina219_read_register(uint8_t reg);    // 读寄存器

#endif
