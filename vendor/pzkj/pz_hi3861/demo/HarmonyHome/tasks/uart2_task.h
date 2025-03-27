#ifndef __UART2_TASK_H__
#define __UART2_TASK_H__

void uart2_task_create(void);
void uart2_task(void);
void usart2_rx_process(char usart2_receive_buffer[]);

#endif // __UART2_TASK_H__
