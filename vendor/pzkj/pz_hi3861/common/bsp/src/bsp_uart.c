#include "bsp_uart.h"
#include "hi_uart.h"


//串口初始化
void uart0_init(uint32_t buad)
{
    hi_uart_attribute uart_attr;
    
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_pull(UART0_TX_PIN, HI_IO_PULL_UP);                // 设置GPIO上拉
    hi_io_set_func(UART0_TX_PIN, UART0_TX_GPIO_FUN);            // 设置IO为串口功能
    hi_gpio_set_dir(UART0_TX_PIN, HI_GPIO_DIR_OUT);             // 设置GPIO为输出模式

    hi_io_set_pull(UART0_RX_PIN, HI_IO_PULL_NONE);                // 设置GPIO浮空
    hi_io_set_func(UART0_RX_PIN, UART0_RX_GPIO_FUN);            // 设置IO为串口功能
    hi_gpio_set_dir(UART0_RX_PIN, HI_GPIO_DIR_IN);             // 设置GPIO为输入模式

    uart_attr.baud_rate=buad;
    uart_attr.data_bits=HI_UART_DATA_BIT_8;
    uart_attr.stop_bits=HI_UART_STOP_BIT_1;
    uart_attr.parity=HI_UART_PARITY_NONE;
    hi_uart_init(HI_UART_IDX_0,&uart_attr,NULL);
}

//串口发送数据
void uart0_send_data(uint8_t *dat,uint8_t len)
{
    hi_uart_write(HI_UART_IDX_0,dat,len);
}

//串口读取数据
uint32_t uart0_read_data(uint8_t *dat,uint8_t len)
{
    return hi_uart_read(HI_UART_IDX_0,dat,len);
}

void uart1_init(uint32_t buad)
{
    hi_uart_attribute uart_attr;
    
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_pull(UART1_TX_PIN, HI_IO_PULL_UP);                // 设置GPIO上拉
    hi_io_set_func(UART1_TX_PIN, UART1_TX_GPIO_FUN);            // 设置IO为串口功能
    hi_gpio_set_dir(UART1_TX_PIN, HI_GPIO_DIR_OUT);             // 设置GPIO为输出模式

    hi_io_set_pull(UART1_RX_PIN, HI_IO_PULL_NONE);                // 设置GPIO浮空
    hi_io_set_func(UART1_RX_PIN, UART1_RX_GPIO_FUN);            // 设置IO为串口功能
    hi_gpio_set_dir(UART1_RX_PIN, HI_GPIO_DIR_IN);             // 设置GPIO为输入模式

    uart_attr.baud_rate=buad;
    uart_attr.data_bits=HI_UART_DATA_BIT_8;
    uart_attr.stop_bits=HI_UART_STOP_BIT_1;
    uart_attr.parity=HI_UART_PARITY_NONE;
    hi_uart_init(HI_UART_IDX_1,&uart_attr,NULL);
}

//串口发送数据
void uart1_send_data(uint8_t *dat,uint8_t len)
{
    hi_uart_write(HI_UART_IDX_1,dat,len);
}

//串口读取数据
uint32_t uart1_read_data(uint8_t *dat,uint8_t len)
{
    return hi_uart_read(HI_UART_IDX_1,dat,len);
}

void uart2_init(uint32_t buad)
{
    hi_uart_attribute uart_attr;
    
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_pull(UART2_TX_PIN, HI_IO_PULL_UP);                // 设置GPIO上拉
    hi_io_set_func(UART2_TX_PIN, UART2_TX_GPIO_FUN);            // 设置IO为串口功能
    hi_gpio_set_dir(UART2_TX_PIN, HI_GPIO_DIR_OUT);             // 设置GPIO为输出模式

    hi_io_set_pull(UART2_RX_PIN, HI_IO_PULL_NONE);                // 设置GPIO浮空
    hi_io_set_func(UART2_RX_PIN, UART2_RX_GPIO_FUN);            // 设置IO为串口功能
    hi_gpio_set_dir(UART2_RX_PIN, HI_GPIO_DIR_IN);             // 设置GPIO为输入模式

    uart_attr.baud_rate=buad;
    uart_attr.data_bits=HI_UART_DATA_BIT_8;
    uart_attr.stop_bits=HI_UART_STOP_BIT_1;
    uart_attr.parity=HI_UART_PARITY_NONE;
    hi_uart_init(HI_UART_IDX_2,&uart_attr,NULL);
}

//串口发送数据
void uart2_send_data(uint8_t *dat,uint8_t len)
{
    hi_uart_write(HI_UART_IDX_2,dat,len);
}

//串口读取数据
uint32_t uart2_read_data(uint8_t *dat,uint8_t len)
{
    return hi_uart_read(HI_UART_IDX_2,dat,len);
}