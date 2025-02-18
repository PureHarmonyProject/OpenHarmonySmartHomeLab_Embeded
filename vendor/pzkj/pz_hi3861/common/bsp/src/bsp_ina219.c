#include "bsp_ina219.h"  // 假设你有一个头文件定义了INA219的相关宏和函数声明
#include "hi_i2c.h"
#include <stdio.h>


// 初始化INA219
void ina219_init(void)
{
    uint32_t result;

    hi_gpio_init();
    // gpio_9 复用为 I2C_SCL
    hi_io_set_pull(HI_IO_NAME_GPIO_9, HI_IO_PULL_UP);
    hi_io_set_func(HI_IO_NAME_GPIO_9, HI_IO_FUNC_GPIO_9_I2C0_SCL);
    // gpio_10 复用为 I2C_SDA
    hi_io_set_pull(HI_IO_NAME_GPIO_10, HI_IO_PULL_UP);
    hi_io_set_func(HI_IO_NAME_GPIO_10, HI_IO_FUNC_GPIO_10_I2C0_SDA);

    // 初始化I2C接口
    result = hi_i2c_init(INA219_I2C_IDX, INA219_I2C_SPEED);
    if (result != HI_ERR_SUCCESS)
    {
        printf("I2C INA219 Init failed with status 0x%x!!!\r\n", result);
    }
    else
    {
        printf("I2C INA219 Init success!!!\r\n");
    }

    // 配置INA219
    ina219_configure();
    uint16_t data;
    ina219_read_register(INA219_BASE_REG, &data);
    ina219_swap_high_low(&data);
    printf("data = %04x\n", data); //读到的依旧是高低反转过后的数据
    // printf("INA219 Init success!!!\r\n");
}

// 写入INA219寄存器
static hi_u32 ina219_write_register(uint8_t reg, uint16_t data)
{
    uint8_t buffer[3] = {0};
    buffer[0] = reg;  // 寄存器地址
    buffer[1] = (data >> 8) & 0xff;  // 要写入的数据
    buffer[2] = data & 0xff; 

    hi_i2c_data i2cData = {0};
    i2cData.send_buf = buffer;
    i2cData.send_len = sizeof(buffer);

    return hi_i2c_write(INA219_I2C_IDX, INA219_I2C_ADDR << 1, &i2cData);
}
static void ina219_swap_high_low(uint16_t *data) 
{
    uint8_t high = *data >> 8; // 高8位
    uint8_t low = *data & 0xff; // 低8位
    *data = (low << 8) | high;
}
// 读取INA219寄存器
static hi_u32 ina219_read_register(uint8_t reg, uint16_t *data)
{
    uint8_t reg_addr = reg;
    hi_i2c_data i2cData = {0};

    // 先发送寄存器地址
    i2cData.send_buf = &reg_addr;
    i2cData.send_len = sizeof(reg_addr);
    hi_u32 result = hi_i2c_write(INA219_I2C_IDX, INA219_I2C_ADDR << 1, &i2cData);//这个api想写多少写多少
    if (result != HI_ERR_SUCCESS)
    {
        return result;
    }

    // 读取寄存器数据
    i2cData.receive_buf = data;
    i2cData.receive_len = sizeof(uint16_t);
    return hi_i2c_read(INA219_I2C_IDX, (INA219_I2C_ADDR << 1) | 1, &i2cData);
}

//配置INA219
void ina219_configure(void)
{
    if(ina219_write_register(INA219_CONFIG_REG, INA219_CONFIG_REG_DEFAULT)) {
        printf("config reg write failed\r\n");
    }
    if(ina219_write_register(INA219_BASE_REG, INA219_CURRENT_LSB)) {
        printf("base reg write failed\r\n");
    }
}

// // 获取INA219的电流值
float ina219_get_current(void)
{
    uint16_t current_reg;
    if (ina219_read_register(INA219_CURRENT_I_REG, &current_reg) != HI_ERR_SUCCESS)
    {
        printf("Failed to read current register.\r\n");
        return -1.0f;
    }

    // 将寄存器值转换为电流值（单位：mA）
    float current = current_reg * 0.1;  // 假设分流电阻为0.1Ω
    return current;
}

// 获取INA219的电压值
float ina219_get_bus_voltage(void)
{
    uint16_t bus_voltage_reg;
    if (ina219_read_register(INA219_POWER_REG, &bus_voltage_reg) != HI_ERR_SUCCESS)
    {
        printf("Failed to read bus voltage register.\r\n");
        return -1.0f;
    }

    // 将寄存器值转换为电压值（单位：V）
    float bus_voltage = (bus_voltage_reg >> 3) * 0.004;  // 16位寄存器，单位为mV
    return bus_voltage;
}

float ina219_get_power(void)
{
    uint16_t power;
    if (ina219_read_register(INA219_BUS_VOLTAGE_REG, &power) != HI_ERR_SUCCESS)
    {
        printf("Failed to read bus voltage register.\r\n");
        return -1.0f;
    }

    // 将寄存器值转换为电压值（单位：V）
    power *= 2;  // 16位寄存器，单位为mV
    return power;
}