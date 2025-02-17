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

    usleep(100 * 1000); // 延时

    // 设置 PCF8575 输出模式，可以设置哪些引脚为输出
    // pcf8575_set_output_mode(0xFFFF); // 默认所有引脚设置为输出
    // pcf8575_write_data(0x0000);      // 初始化时，将所有输出设置为low电平

    if(!pcf8575_write(0xff0f)) {
        printf("write success\r\n");
    }else {
        printf("write fail\r\n");
    }

    uint16_t data;
    if(!pcf8575_read(&data)) {
        printf("data = %04x, read success\r\n",data);
    }else {
        printf("read fail\r\n");
    }
    // // 尝试读取数据并确认通信
    // uint16_t data = pcf8575_read_data();
    // if (data == 0x0000) {
    //     printf("I2C PCF8575 communication failed!!!\r\n");
    // } else {
    //     printf("I2C PCF8575 communication successful!!!\r\n");
    // }
}

hi_u32 pcf8575_write(uint16_t data)
{
    uint8_t buffer[2] = {0};
    buffer[0] = (data >> 8) & 0xff;
    buffer[1] = data & 0xff;

    hi_i2c_data i2cData = {0};
    i2cData.send_buf = buffer;
    i2cData.send_len = sizeof(buffer);

    return hi_i2c_write(PCF8575_I2C_IDX, PCF8575_I2C_ADDR << 1, &i2cData);
}

hi_u32 pcf8575_read(uint16_t *data)
{
    hi_i2c_data i2cData = {0};
    i2cData.receive_buf = data;
    i2cData.receive_len = sizeof(uint16_t);

    return hi_i2c_read(PCF8575_I2C_IDX, (PCF8575_I2C_ADDR << 1) | 1, &i2cData);
}

