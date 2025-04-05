#include "uart2_task.h"
#include "template.h"

osThreadId_t Uart2_Task_ID;

extern uint8_t page_mode;
extern void oled_showstring_center(uint8_t y, uint8_t *str, uint8_t font_size);
void usart2_rx_process(char usart2_receive_buffer[])
{
    uint32_t received_response = (usart2_receive_buffer[0] << 24) |
                                 (usart2_receive_buffer[1] << 16) |
                                 (usart2_receive_buffer[2] << 8) |
                                 (usart2_receive_buffer[3]);

    memset(usart2_receive_buffer, 0, sizeof(usart2_receive_buffer));

    uint8_t response_type = (received_response >> 24) & 0xF;
    uint8_t response_code = (received_response >> 20) & 0xF;
    uint8_t device_id = received_response & 0xF;
    uint8_t switch_state = (received_response >> 4) & 0xF;
    uint8_t param1 = (received_response >> 8) & 0xF;
    uint8_t param2 = (received_response >> 12) & 0xF;

    if (((received_response >> 28) & 0xF) != 0xB || response_type != 0x1)
    {
        printf("Invalid response received! response = %04x\n", received_response);
        return;
    }

    printf("Response Received: Device %X, State %X, Param1 %X, Param2 %X, Success %X, response = %04x\n",
           device_id, switch_state, param1, param2, response_code, received_response);

    if (response_code == 0x1)
    {
        char line1[32];
        char line2[32];
        char line3[32];
        switch (device_id)
        {
        case 0x1:
            door_set_curstate(switch_state);
            oled_clear();
            snprintf(line1, sizeof(line1), "---Device Action---");
            oled_showstring_center(1, (uint8_t *)line1, 12);
            snprintf(line2, sizeof(line2), "DOOR %s", switch_state ? "OPEN" : "CLOSE");
            oled_showstring_center(18, (uint8_t *)line2, 12);
            oled_refresh_gram();
            page_mode = 2;
            break;
        
        case 0x2:
            curtain_update_curangle(switch_state, param1 * 10);
            oled_clear();
            snprintf(line1, sizeof(line1), "---Device Action---");
            oled_showstring_center(1, (uint8_t *)line1, 12);
            snprintf(line2, sizeof(line2), "CURTAIN :%s", switch_state ? "OPEN" : "CLOSE");
            oled_showstring_center(17, (uint8_t *)line2, 12);
            snprintf(line3, sizeof(line3), "CURTAIN PERCENT :%d", curtain_get_curangle());
            oled_showstring_center(37, (uint8_t *)line3, 12);
            oled_refresh_gram();
            page_mode = 2;
            break;
        
        case 0x3:
            airConditioner_set_curstate((uint8_t)((param1 << 1) | param2));
            oled_clear();
            snprintf(line1, sizeof(line1), "---Device Action---");
            oled_showstring_center(1, (uint8_t *)line1, 12);
            if(param1 != 0)
            {
                snprintf(line2, sizeof(line2), "AC MODE :%s", param2 ? "HEAT" : "COOL");
                oled_showstring_center(17, (uint8_t *)line2, 12);
            }
            else
            {
                snprintf(line2, sizeof(line2), "AC MODE :%s", "STOP");
                oled_showstring_center(17, (uint8_t *)line2, 12);
            }
            snprintf(line3, sizeof(line3), "AC SPEED :%d", param1);
            oled_showstring_center(37, (uint8_t *)line3, 12);
            oled_refresh_gram();
            page_mode = 2;
            break;        
        default:
            printf("Unknown device ID: %X\n", device_id);
            break;
        }
    }
}

void uart2_task(void)
{
    char usart2_receive_buffer[128];
    int len2;
    uart2_init(115200);

    while (1)
    {
        len2 = uart2_read_data(usart2_receive_buffer, sizeof(usart2_receive_buffer));
        if (len2 > 0)
        {
            printf("usart2 process , len2 = %d\n", len2);
            usart2_receive_buffer[len2] = '\0';
            usart2_rx_process(usart2_receive_buffer);
        }
    }
}

void uart2_task_create(void)
{
    osThreadAttr_t taskOptions = {
        .name = "Uart2Task",
        .attr_bits = 0,
        .cb_mem = NULL,
        .cb_size = 0,
        .stack_mem = NULL,
        .stack_size = 4096,
        .priority = osPriorityNormal1,
    };

    Uart2_Task_ID = osThreadNew((osThreadFunc_t)uart2_task, NULL, &taskOptions);
    if (Uart2_Task_ID != NULL)
    {
        printf("ID = %d, Create Uart2_Task_ID is OK!\n", Uart2_Task_ID);
    }
}
