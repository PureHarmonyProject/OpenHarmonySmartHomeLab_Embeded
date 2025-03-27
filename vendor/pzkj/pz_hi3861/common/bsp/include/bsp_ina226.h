#ifndef BSP_INA226_H
#define BSP_INA226_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

// INA226 I2C 地址 & 寄存器地址定义
#define INA226_I2C_ADDR         0x40  
#define INA226_CONFIG_REG       0x00
#define INA226_SINGLE_VOLTAGE_REG 0x01
#define INA226_BUS_VOLTAGE_REG  0x02
#define INA226_POWER_REG        0x03
#define INA226_CURRENT_I_REG    0x04
#define INA226_BASE_REG         0x05
#define INA226_ENABLE_REG       0x06
#define INA226_ALERT_REG        0x07
#define INA226_CAL_REG          0x05

// I2C 配置
#define INA226_I2C_IDX          0
#define INA226_I2C_SPEED        100000

// 计算得出的校准与LSB值
#define INA226_CAL_VAL          34133       // 校准寄存器值 (根据0.01Ω 和 15uA/bit计算)
#define INA226_CURRENT_LSB      0.000015f   // 15µA per bit (单位：A/bit)
#define INA226_VOLTAGE_LSB      1.25f       // 固定分辨率 1.25mV/bit
#define INA226_POWER_LSB        (25 * INA226_CURRENT_LSB) // 0.375 mW/bit

// 寄存器配置值（默认）
#define INA226_CONFIG_REG_VAL   0x4527      
#define INA226_BASE_REG_VAL     INA226_CAL_VAL

// 函数声明
void ina226_init(void);
float ina226_get_current(void);
float ina226_get_bus_voltage(void);
float ina226_get_power(void);
void ina226_configure(void);

#endif
