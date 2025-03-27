#ifndef __UART1_TASK_H__
#define __UART1_TASK_H__

void uart1_task_create(void);
void uart1_task(void);
void usart1_rx_process(char usart1_receive_buffer[]);

#endif // __UART1_TASK_H__
