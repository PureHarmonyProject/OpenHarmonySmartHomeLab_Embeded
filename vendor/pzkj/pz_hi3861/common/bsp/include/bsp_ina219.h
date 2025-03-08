#ifndef BSP_INA219_H
#define BSP_INA219_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

// 定义INA219的I2C地址和寄存器地址
#define INA219_I2C_ADDR 0x40  // 默认地址，根据硬件连接可能需要修改
#define INA219_CONFIG_REG 0x00
#define INA219_BUS_VOLTAGE_REG 0x02 //电压寄存器
#define INA219_POWER_REG 0x03 //功率寄存器
#define INA219_CURRENT_I_REG 0x04 //电流寄存器
#define INA219_BASE_REG 0x05 //基准寄存器

#define INA219_I2C_IDX         0             // 使用的 I2C 总线号
#define INA219_I2C_SPEED       100000        // 100KHz I2C 速度

// #define INA219_CURRENT_LSB 0x1000          //未校准前的配置寄存器,current 电流
#define INA219_MAX_VOLTAGE 0.32
#define INA219_TEST_R      10f
#define INA219_MAX_CURRENT INA219_MAX_VOLTAGE / INA219_TEST_R
#define INA219_CURRENT_LSB_MIN INA219_MAX_CURRENT / 0x8000
#define INA219_CURRENT_LSB_MAX INA219_MAX_CURRENT / 0x1000
#define INA219_CURRENT_LSB_DEFAULT INA219_CURRENT_LSB_MIN   //0.001ma
#define INA219_CONFIG_REG_DEFAULT 0.04096 / (INA219_CURRENT_LSB_DEFAULT * INA219_TEST_R)
#define INA219_CONFIG_REG_VAL 0x3c1f    //配置寄存器
#define INA219_BASE_REG_VAL 0x1000      //4096,10欧
 
// 函数声明
void ina219_init(void);                        // 初始化INA219模块
float ina219_get_current(void);
float ina219_get_bus_voltage(void);
float ina219_get_power(void);
static hi_u32 ina219_write_register(uint8_t reg, uint16_t data);
static hi_u32 ina219_read_register(uint8_t reg, uint16_t *data);
void ina219_configure(void);
static void ina219_swap_high_low(uint16_t *data);
#endif
