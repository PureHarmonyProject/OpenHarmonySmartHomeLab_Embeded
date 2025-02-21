#ifndef BSP_UART_H
#define BSP_UART_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

//管脚定义
#define UART0_TX_PIN            HI_IO_NAME_GPIO_3
#define UART0_TX_GPIO_FUN       HI_IO_FUNC_GPIO_3_UART0_TXD

#define UART0_RX_PIN            HI_IO_NAME_GPIO_4
#define UART0_RX_GPIO_FUN       HI_IO_FUNC_GPIO_4_UART0_RXD

#define UART1_TX_PIN            HI_IO_NAME_GPIO_0
#define UART1_TX_GPIO_FUN       HI_IO_FUNC_GPIO_0_UART1_TXD

#define UART1_RX_PIN            HI_IO_NAME_GPIO_1
#define UART1_RX_GPIO_FUN       HI_IO_FUNC_GPIO_1_UART1_RXD

//函数声明
void uart0_init(uint32_t buad);
void uart0_send_data(uint8_t *dat,uint8_t len);
uint32_t uart0_read_data(uint8_t *dat,uint8_t len);

void uart1_init(uint32_t buad);
void uart1_send_data(uint8_t *dat,uint8_t len);
uint32_t uart1_read_data(uint8_t *dat,uint8_t len);
#endif
