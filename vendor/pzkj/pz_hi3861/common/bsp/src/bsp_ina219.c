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
    // uint16_t data;
    // ina219_read_register(INA219_CONFIG_REG, &data);
    // printf("data1 = %04x\n", data); 
    // ina219_read_register(INA219_BASE_REG, &data);
    // printf("data2 = %04x\n", data); 
    // printf("INA219 Init success!!!\r\n");
}

// 写入INA219寄存器
static hi_u32 ina219_write_register(uint8_t reg, uint16_t data)
{
    uint8_t buffer[3] = {0};
    buffer[0] = reg;  // 寄存器地址
    buffer[1] = data & 0xff; 
    buffer[2] = (data >> 8) & 0xff;  // 要写入的数据
    
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
    if(ina219_write_register(INA219_CONFIG_REG, INA219_CONFIG_REG_VAL)) {
        printf("config reg write failed\r\n");
    }
    if(ina219_write_register(INA219_BASE_REG, INA219_BASE_REG_VAL)) {
        printf("base reg write failed\r\n");
    }
}

float cur_current, cur_voltage;
// // 获取INA219的电流值
float ina219_get_current(void)
{
    uint16_t current_reg;
    if (ina219_read_register(INA219_CURRENT_I_REG, &current_reg) != HI_ERR_SUCCESS)
    {
        printf("Failed to read current register.\r\n");
        return -1.0f;
    }
    ina219_swap_high_low(&current_reg);//需要高低位转换一下数据才正常

    // printf("current_reg = %4x\n",current_reg);
    // printf("current_reg = %d\n",current_reg);
    // 将寄存器值转换为电流值（单位：mA）
    float current = (float)current_reg * 0.1; 
    cur_current = current;
    // printf("current = %.2f\n",current);
    return current;
}

//获取INA219的电压值 mv
float ina219_get_bus_voltage(void)
{
    uint16_t bus_voltage_reg;
    if (ina219_read_register(INA219_BUS_VOLTAGE_REG, &bus_voltage_reg) != HI_ERR_SUCCESS)
    {
        printf("Failed to read bus voltage register.\r\n");
        return -1.0f;
    }

    // 将寄存器值转换为电压值（单位：V）
    ina219_swap_high_low(&bus_voltage_reg);
    float bus_voltage = ((int)(bus_voltage_reg >> 3)) * 4;  // 16位寄存器，单位为mV
    cur_voltage = bus_voltage;
    return bus_voltage;
}

//获得功率
float ina219_get_power(void)
{
    return cur_voltage * cur_current;
}