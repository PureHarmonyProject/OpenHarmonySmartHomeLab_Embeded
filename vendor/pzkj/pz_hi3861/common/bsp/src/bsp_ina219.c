/**
 ****************************************************************************************************
 * @file        bsp_ina219.c
 * @author      普中科技
 * @version     V1.0
 * @date        2024-06-05
 * @brief       INA219电流传感器驱动
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

#include "bsp_ina219.h"
#include <unistd.h>
#include "hi_i2c.h"

// I2C发送数据
uint32_t ina219_iic_senddate(uint8_t *data, size_t size)
{
    hi_i2c_data i2cData = {0};
    i2cData.send_buf = data;
    i2cData.send_len = size;

    return hi_i2c_write(INA219_I2C_IDX, INA219_I2C_ADDR, &i2cData);
}

// I2C读取数据
uint32_t ina219_iic_readdata(uint8_t reg, uint8_t *data, size_t size)
{
    hi_i2c_data i2cData = {0};
    uint8_t buffer[1] = {reg};

    i2cData.send_buf = buffer;
    i2cData.send_len = 1;
    i2cData.receive_buf = data;
    i2cData.receive_len = size;

    return hi_i2c_read(INA219_I2C_IDX, INA219_I2C_ADDR, &i2cData);
}

// 写配置寄存器
void ina219_write_config(uint16_t config)
{
    uint8_t data[2];
    data[0] = (config >> 8) & 0xFF;
    data[1] = config & 0xFF;

    ina219_iic_senddate(data, sizeof(data));
}

// 读取电流值
int16_t ina219_read_current(void)
{
    uint8_t data[2];
    if (ina219_iic_readdata(INA219_REG_CURRENT, data, sizeof(data)) != HI_ERR_SUCCESS)
    {
        return -1; // 读取失败
    }

    int16_t current = (data[0] << 8) | data[1];
    return current;
}

// 读取总线电压
uint16_t ina219_read_bus_voltage(void)
{
    uint8_t data[2];
    if (ina219_iic_readdata(INA219_REG_BUSVOLTAGE, data, sizeof(data)) != HI_ERR_SUCCESS)
    {
        return 0xFFFF; // 读取失败
    }

    uint16_t voltage = ((data[0] << 8) | data[1]) >> 3; // 电压值
    return voltage;
}

// 读取分流电压
int16_t ina219_read_shunt_voltage(void)
{
    uint8_t data[2];
    if (ina219_iic_readdata(INA219_REG_SHUNTVALUE, data, sizeof(data)) != HI_ERR_SUCCESS)
    {
        return -1; // 读取失败
    }

    int16_t shunt_voltage = (data[0] << 8) | data[1];
    return shunt_voltage;
}

// 初始化INA219传感器
void ina219_init(void)
{
    uint16_t config = 0x399F; // 配置INA219
    ina219_write_config(config); // 写入配置寄存器
}

// 计算功率
uint32_t ina219_calculate_power(int16_t current, uint16_t bus_voltage)
{
    // 计算功率 = 电流 * 电压
    return (uint32_t)current * bus_voltage;
}

