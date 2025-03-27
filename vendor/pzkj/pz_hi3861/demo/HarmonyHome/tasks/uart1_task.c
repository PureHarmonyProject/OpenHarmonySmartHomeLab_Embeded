#include "uart1_task.h"
#include "template.h"

osThreadId_t Uart1_Task_ID;

void usart1_rx_process(char usart1_receive_buffer[])
{
    uint8_t buffer[4] = {0};
    memcpy(buffer, usart1_receive_buffer, 4);

    printf("Raw Data from ASRPRO: %02X %02X %02X %02X\n",
           buffer[0], buffer[1], buffer[2], buffer[3]);

    uint32_t received_command = (buffer[0] << 24) |
                                (buffer[1] << 16) |
                                (buffer[2] << 8) |
                                (buffer[3]);

    memset(usart1_receive_buffer, 0, sizeof(usart1_receive_buffer));

    uint8_t command_type = (received_command >> 28) & 0xF;

    if (command_type == 0xA)
    {
        uint8_t brightnessLevel = (received_command >> 24) & 0xF;
        uint8_t r = (received_command >> 16) & 0xFF;
        uint8_t g = (received_command >> 8) & 0xFF;
        uint8_t b = received_command & 0xFF;

        printf("LED State Update: Brightness = %X, RGB = (%02X, %02X, %02X)\n",
               brightnessLevel, r, g, b);

        led_update_color_and_brightness(received_command);
        return;
    }

    if (command_type == 0xC)
    {
        uint8_t device_id = received_command & 0xF;
        uint8_t switch_state = (received_command >> 4) & 0xF;
        uint8_t param1 = (received_command >> 8) & 0xF;
        uint8_t param2 = (received_command >> 12) & 0xF;

        printf("ASRPRO Command: Device %X, State %X, Param1 %X, Param2 %X, command = %08X\n",
               device_id, switch_state, param1, param2, received_command);

        uint32_t stm32_command = ((uint32_t)0xB << 28) |
                                 (device_id) |
                                 (switch_state << 4) |
                                 (param1 << 8) |
                                 (param2 << 12);

        uint8_t stm32_command_buffer[4];
        stm32_command_buffer[0] = (stm32_command >> 24) & 0xFF;
        stm32_command_buffer[1] = (stm32_command >> 16) & 0xFF;
        stm32_command_buffer[2] = (stm32_command >> 8) & 0xFF;
        stm32_command_buffer[3] = stm32_command & 0xFF;

        printf("Sending Command to STM32: %08X\n", stm32_command);
        uart2_send_data(stm32_command_buffer, 4);
        return;
    }

    printf("Invalid command received from ASRPRO! command = %08X\n", received_command);
}

void uart1_task(void)
{
    char usart1_receive_buffer[128];
    int len1;
    uart1_init(115200);

    while (1)
    {
        len1 = uart1_read_data(usart1_receive_buffer, 4);
        if (len1 > 0)
        {
            printf("usart1 process , len1 = %d\n", len1);
            usart1_receive_buffer[len1] = '\0';
            usart1_rx_process(usart1_receive_buffer);
        }
    }
}

void uart1_task_create(void)
{
    osThreadAttr_t taskOptions = {
        .name = "Uart1Task",
        .attr_bits = 0,
        .cb_mem = NULL,
        .cb_size = 0,
        .stack_mem = NULL,
        .stack_size = 4096,
        .priority = osPriorityNormal1,
    };

    Uart1_Task_ID = osThreadNew((osThreadFunc_t)uart1_task, NULL, &taskOptions);
    if (Uart1_Task_ID != NULL)
    {
        printf("ID = %d, Create Uart1_Task_ID is OK!\n", Uart1_Task_ID);
    }
}
