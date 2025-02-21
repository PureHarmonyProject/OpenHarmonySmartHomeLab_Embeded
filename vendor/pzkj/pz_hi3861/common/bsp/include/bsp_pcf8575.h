#ifndef BSP_PC86575_H
#define BSP_PC86575_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

// PCF8575 I2C 地址和配置
#define PCF8575_I2C_ADDR        0x20         // 默认 I2C 从设备地址（可通过 A0, A1, A2 设置）
#define PCF8575_I2C_IDX         0             // 使用的 I2C 总线号
#define PCF8575_I2C_SPEED       100000        // 100KHz I2C 速度

// #define Max_Column              128           // 最大列数
// #define Max_Row                 64            // 最大行数

#define PCF8575_CMD             0             // 写命令标志
#define PCF8575_DATA            1             // 写数据标志

// 函数声明
void pcf8575_init(void);                               // 初始化 PCF8575
// hi_u32 pcf8575_write(uint16_t data);
// hi_u32 pcf8575_read(uint16_t *data);
// void swap_high_low(uint16_t *data);
uint8_t pcf8575_read_bit(uint8_t bit);
void pcf8575_write_bit(uint8_t bit, uint8_t value);
hi_u32 pcf8575_write(uint16_t data);

hi_u32 pcf8575_read(uint16_t *data);

void swap_high_low(uint16_t *data) ;
#endif
