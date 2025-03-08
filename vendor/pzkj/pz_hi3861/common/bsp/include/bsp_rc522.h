#ifndef BSP_RC522_H
#define BSP_RC522_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"
#include <hi_spi.h>
#include <hi_types.h>
#include <hi_time.h>
#include <hi_errno.h>
#include <string.h>

#define RC522_SPI_ID   HI_SPI_ID_0  // 选择 SPI0

// **RC522 片选 (CS) 引脚**
#define RC522_CS_PIN         HI_IO_NAME_GPIO_5
#define RC522_CS_GPIO_FUN    HI_IO_FUNC_GPIO_5_SPI0_CSN

// **RC522 时钟 (SCK) 引脚**
#define RC522_SCK_PIN        HI_IO_NAME_GPIO_6
#define RC522_SCK_GPIO_FUN   HI_IO_FUNC_GPIO_6_SPI0_CK

// **RC522 主发送 (MOSI) 引脚**
#define RC522_MOSI_PIN       HI_IO_NAME_GPIO_8
#define RC522_MOSI_GPIO_FUN  HI_IO_FUNC_GPIO_8_SPI0_TXD

// **RC522 主接收 (MISO) 引脚**
#define RC522_MISO_PIN       HI_IO_NAME_GPIO_7
#define RC522_MISO_GPIO_FUN  HI_IO_FUNC_GPIO_7_SPI0_RXD

// **RC522 复位 (RST) 引脚**
#define RC522_RST_PIN        HI_IO_NAME_GPIO_0
#define RC522_RST_GPIO_FUN   HI_IO_FUNC_GPIO_0_GPIO

// **RC522 中断 (IRQ) (可选)**
#define RC522_IRQ_PIN        HI_IO_NAME_GPIO_9
#define RC522_IRQ_GPIO_FUN   HI_IO_FUNC_GPIO_9_GPIO

// **RC522 寄存器定义**
#define PCD_IDLE       0x00  // 空闲命令
#define PCD_AUTHENT    0x0E  // 验证密钥
#define PCD_RECEIVE    0x08  // 接收数据
#define PCD_TRANSMIT   0x04  // 发送数据
#define PCD_TRANSCEIVE 0x0C  // 发送并接收数据
#define PCD_RESET      0x0F  // 复位 RC522

// **卡片操作指令**
#define PICC_REQIDL    0x26  // 寻找未进入休眠状态的卡
#define PICC_ANTICOLL  0x93  // 防冲突指令
#define PICC_SELECTTAG 0x93  // 选卡
#define PICC_AUTHENT1A 0x60  // A 密钥认证
#define PICC_READ      0x30  // 读块
#define PICC_WRITE     0xA0  // 写块
#define PICC_HALT      0x50  // 休眠

// **函数声明**
hi_u32 rc522_init(void);               // 初始化 RC522
hi_u32 rc522_reset(void);                // 复位 RC522
void rc522_antenna_on(void);           // 打开 RC522 天线
hi_u8 rc522_check_card(void);          // 检测是否有 NFC 卡片
hi_u8 rc522_read_card_uid(hi_u8 *uid); // 读取 NFC UID

// **SPI 读写**
hi_u8 rc522_read_register(hi_u8 reg);
hi_u8 rc522_write_register(hi_u8 reg, hi_u8 value);
void rc522_write_read_register(hi_u8 addr, hi_u8 writedata, hi_u8 *read_data, hi_u8 read_len, hi_bool is_read);
// **卡片操作**
hi_u8 rc522_request(hi_u8 reqMode, hi_u8 *buffer, hi_u8 *bufferSize); // 寻卡
hi_u8 rc522_anticoll(hi_u8 *serial);  // 读取卡片序列号
hi_u8 rc522_to_card(hi_u8 command, hi_u8 *sendData, hi_u8 sendLen, hi_u8 *backData, hi_u8 *backLen); // 发送命令

#endif // BSP_RC522_H
