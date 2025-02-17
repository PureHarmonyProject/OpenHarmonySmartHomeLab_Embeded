#include "bsp_pcf8575.h"
#include "hi_i2c.h"
#include <unistd.h>

// 初始化 PCF8575
void pcf8575_init(void)
{
    uint32_t result;

    hi_gpio_init();
    // gpio_9 复用为 I2C_SCL
    hi_io_set_pull(HI_IO_NAME_GPIO_9, HI_IO_PULL_UP);
    hi_io_set_func(HI_IO_NAME_GPIO_9, HI_IO_FUNC_GPIO_9_I2C0_SCL);
    // gpio_10 复用为 I2C_SDA
    hi_io_set_pull(HI_IO_NAME_GPIO_10, HI_IO_PULL_UP);
    hi_io_set_func(HI_IO_NAME_GPIO_10, HI_IO_FUNC_GPIO_10_I2C0_SDA);
    
    // I2C 初始化
    result = hi_i2c_init(PCF8575_I2C_IDX, PCF8575_I2C_SPEED);
    if (result != HI_ERR_SUCCESS)
    {
        printf("I2C PCF8575 Init failed with status 0x%x!!!\r\n", result);
        return;
    }
    else {
        printf("I2C PCF8575 Init success!!!\r\n");
    }

    // usleep(100 * 1000); // 延时

    // pcf8575_write_bit(8,1);
    // printf("data = %d\r\n",pcf8575_read_bit(8));
}

static hi_u32 pcf8575_write(uint16_t data)
{
    uint8_t buffer[2] = {0};
    buffer[0] = (data >> 8) & 0xff;
    buffer[1] = data & 0xff;

    hi_i2c_data i2cData = {0};
    i2cData.send_buf = buffer;
    i2cData.send_len = sizeof(buffer);

    return hi_i2c_write(PCF8575_I2C_IDX, PCF8575_I2C_ADDR << 1, &i2cData);
}

static hi_u32 pcf8575_read(uint16_t *data)
{
    hi_i2c_data i2cData = {0};
    i2cData.receive_buf = data;
    i2cData.receive_len = sizeof(uint16_t);

    return hi_i2c_read(PCF8575_I2C_IDX, (PCF8575_I2C_ADDR << 1) | 1, &i2cData);
}

static void swap_high_low(uint16_t *data) 
{
    uint8_t high = *data >> 8; // 高8位
    uint8_t low = *data & 0xff; // 低8位
    *data = (low << 8) | high;
}

uint8_t pcf8575_read_bit(uint8_t bit)
{
    uint16_t all_bits;    
    if(!pcf8575_read(&all_bits)) {
        swap_high_low(&all_bits);
        return (all_bits >> bit) & 0x01;
    }
}

void pcf8575_write_bit(uint8_t bit, uint8_t value)
{
    uint16_t all_bits;
    if(!pcf8575_read(&all_bits)) {
        swap_high_low(&all_bits);
        if(value) {
            all_bits |= (1 << bit);
        } else {
            all_bits &= ~(1 << bit);
        }
        pcf8575_write(all_bits);
    }
}
