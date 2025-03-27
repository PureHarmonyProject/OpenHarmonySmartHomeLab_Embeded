#include "bsp_ina226.h"
#include "hi_i2c.h"
#include <stdio.h>

static float ina226_cur_current = 0;
static float ina226_cur_voltage = 0;

// 内部函数声明
static hi_u32 ina226_write_register(uint8_t reg, uint16_t data);
static hi_u32 ina226_read_register(uint8_t reg, uint16_t *data);
static void ina226_swap_high_low(uint16_t *data);

// 初始化INA226
void ina226_init(void)
{
    uint32_t result;

    hi_gpio_init();

    hi_io_set_pull(HI_IO_NAME_GPIO_9, HI_IO_PULL_UP);
    hi_io_set_func(HI_IO_NAME_GPIO_9, HI_IO_FUNC_GPIO_9_I2C0_SCL);
    hi_io_set_pull(HI_IO_NAME_GPIO_10, HI_IO_PULL_UP);
    hi_io_set_func(HI_IO_NAME_GPIO_10, HI_IO_FUNC_GPIO_10_I2C0_SDA);

    result = hi_i2c_init(INA226_I2C_IDX, INA226_I2C_SPEED);
    if (result != HI_ERR_SUCCESS)
    {
        printf("I2C INA226 Init failed with status 0x%x!!!\r\n", result);
    }
    else
    {
        printf("I2C INA226 Init success!!!\r\n");
    }

    ina226_configure();
}

// 写入寄存器
static hi_u32 ina226_write_register(uint8_t reg, uint16_t data)
{
    uint8_t buffer[3] = {0};
    buffer[0] = reg;
    buffer[1] = (data >> 8) & 0xff;  // 高字节
    buffer[2] = data & 0xff;         // 低字节

    hi_i2c_data i2cData = {0};
    i2cData.send_buf = buffer;
    i2cData.send_len = sizeof(buffer);

    return hi_i2c_write(INA226_I2C_IDX, INA226_I2C_ADDR << 1, &i2cData);
}

// 读取寄存器
static hi_u32 ina226_read_register(uint8_t reg, uint16_t *data)
{
    uint8_t reg_addr = reg;
    hi_i2c_data i2cData = {0};

    // 写入寄存器地址
    i2cData.send_buf = &reg_addr;
    i2cData.send_len = sizeof(reg_addr);
    hi_u32 result = hi_i2c_write(INA226_I2C_IDX, INA226_I2C_ADDR << 1, &i2cData);
    if (result != HI_ERR_SUCCESS)
    {
        return result;
    }

    // 读取2字节
    i2cData.receive_buf = data;
    i2cData.receive_len = sizeof(uint16_t);
    result = hi_i2c_read(INA226_I2C_IDX, (INA226_I2C_ADDR << 1) | 1, &i2cData);
    if (result == HI_ERR_SUCCESS)
    {
        ina226_swap_high_low(data);
    }
    return result;
}

// 交换高低字节
static void ina226_swap_high_low(uint16_t *data)
{
    uint8_t high = (*data >> 8) & 0xff;
    uint8_t low  = (*data) & 0xff;
    *data = (low << 8) | high;
}

// 配置INA226
void ina226_configure(void)
{
    if (ina226_write_register(INA226_CONFIG_REG, INA226_CONFIG_REG_VAL))
    {
        printf("config reg write failed\r\n");
    }
    if (ina226_write_register(INA226_CAL_REG, INA226_CAL_VAL))
    {
        printf("calibration reg write failed\r\n");
    }
}

// 读取当前电流（单位：mA）
float ina226_get_current(void)
{
    uint16_t current_reg;
    if (ina226_read_register(INA226_CURRENT_I_REG, &current_reg) != HI_ERR_SUCCESS)
    {
        printf("Failed to read current register.\r\n");
        return -1.0f;
    }

    float current = current_reg * INA226_CURRENT_LSB * 1000.0f; // 转换为 mA
    ina226_cur_current = current;
    return current;
}

// 读取总线电压（单位：mV）
float ina226_get_bus_voltage(void)
{
    uint16_t bus_voltage_reg;
    if (ina226_read_register(INA226_BUS_VOLTAGE_REG, &bus_voltage_reg) != HI_ERR_SUCCESS)
    {
        printf("Failed to read bus voltage register.\r\n");
        return -1.0f;
    }

    float bus_voltage = bus_voltage_reg * INA226_VOLTAGE_LSB; // 单位 mV
    ina226_cur_voltage = bus_voltage;
    return bus_voltage;
}

// 计算功率（单位：mW）
float ina226_get_power(void)
{
    uint16_t power_reg;
    if (ina226_read_register(INA226_POWER_REG, &power_reg) != HI_ERR_SUCCESS)
    {
        printf("Failed to read power register.\r\n");
        return -1.0f;
    }

    float power = power_reg * INA226_POWER_LSB * 1000.0f; // 转为 mW
    return power;
}
