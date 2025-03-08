#ifndef BSP_INA226_H
#define BSP_INA226_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

// 定义INA226的I2C地址和寄存器地址
#define INA226_I2C_ADDR 0x40  
#define INA226_CONFIG_REG 0x00
#define INA226_SINGLE_VOLTAGE_REG 0x01 //分流电压寄存器
#define INA226_BUS_VOLTAGE_REG 0x02 //总线电压寄存器
#define INA226_POWER_REG 0x03 //功率寄存器
#define INA226_CURRENT_I_REG 0x04 //电流寄存器
#define INA226_BASE_REG 0x05 //校准寄存器
#define INA226_ENABLE_REG 0x06 //屏蔽使能寄存器
#define INA226_ALERT_REG 0x07 //警报寄存器

#define INA226_I2C_IDX         0             // 使用的 I2C 总线号
#define INA226_I2C_SPEED       100000        // 100KHz I2C 速度

// #define INA226_CURRENT_LSB 0x1000          //未校准前的配置寄存器,current 电流
// #define INA226_MAX_VOLTAGE 0.32
// #define INA226_TEST_R      10f
// #define INA226_MAX_CURRENT INA226_MAX_VOLTAGE / INA226_TEST_R
// #define INA226_CURRENT_LSB_MIN INA226_MAX_CURRENT / 0x8000
// #define INA226_CURRENT_LSB_MAX INA226_MAX_CURRENT / 0x1000
// #define INA226_CURRENT_LSB_DEFAULT INA226_CURRENT_LSB_MIN   //0.001ma
// #define INA226_CONFIG_REG_DEFAULT 0.04096 / (INA226_CURRENT_LSB_DEFAULT * INA226_TEST_R)
#define INA226_CONFIG_REG_VAL 0x4527    //配置寄存器
#define INA226_BASE_REG_VAL 0x800      //4096,10欧
 
// 函数声明
void ina226_init(void);                        // 初始化INA226模块
float ina226_get_current(void);
float ina226_get_bus_voltage(void);
float ina226_get_power(void);
static hi_u32 ina226_write_register(uint8_t reg, uint16_t data);
static hi_u32 ina226_read_register(uint8_t reg, uint16_t *data);
void ina226_configure(void);
static void ina226_swap_high_low(uint16_t *data);
#endif
