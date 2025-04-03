#include "smoke_task.h"
#include "template.h"

osThreadId_t SMOKE_Task_ID;

#define MAX_SMOKE 2000
#define MAX_MQ5 2000

extern uint8_t page_mode;
void smoke_sensor_task(void)
{
    uint16_t data1, data2;

    while (1)
    {
        data1 = smoke_get_value();

        data2 = MQ5_get_value();

        if(data1 > MAX_SMOKE || data2 > MAX_MQ5)
        {
            beep_warning();
            page_mode = 1;
            const char *word = "WARING!!!";
            uint8_t font_size = 24;     // 可改为 16 或 12
            uint8_t char_width = 12;    // 每个字宽度（与字体大小对应）
            uint8_t word_len = strlen(word);
            uint8_t total_width = word_len * char_width;

            uint8_t x = (128 - total_width) / 2;
            uint8_t y = (64 - font_size) / 2;

            oled_clear();
            oled_showstring(x, y, (uint8_t *)word, font_size);
            oled_refresh_gram();
        }
        else
        {
            beep_stop();
            page_mode = 0;
        }
        osDelay(100);
    }
}

void smoke_sensor_task_create(void)
{
    osThreadAttr_t taskOptions = {
        .name = "ModuleTask",
        .attr_bits = 0,
        .cb_mem = NULL,
        .cb_size = 0,
        .stack_mem = NULL,
        .stack_size = 4096,
        .priority = osPriorityNormal2,
    };

    SMOKE_Task_ID = osThreadNew((osThreadFunc_t)smoke_sensor_task, NULL, &taskOptions);
    if (SMOKE_Task_ID != NULL)
    {
        printf("ID = %d, Create SMOKE_Task_ID is OK!\n", SMOKE_Task_ID);
    }
}
