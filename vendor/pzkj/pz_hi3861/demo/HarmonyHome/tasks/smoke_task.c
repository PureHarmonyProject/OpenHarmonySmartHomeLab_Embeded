#include "smoke_task.h"
#include "template.h"

osThreadId_t SMOKE_Task_ID;

void smoke_sensor_task(void)
{
    uint16_t data1, data2;

    // printf("SMOKE is initing !!!\r\n");
    // smoke_init();
    // printf("SMOKE init success !!!\r\n");

    // printf("MQ5 is initing !!!\r\n");
    // MQ5_init();
    // printf("MQ5 init success !!!\r\n");

    while (1)
    {
        data1 = smoke_get_value();
        printf("[SMOKE] 烟雾传感器数值 = %d\n", data1);
        if (data1 > 10000)
            printf("[SMOKE] 检测到烟雾\n");
        else
            printf("[SMOKE] 没有检测到烟雾\n");

        data2 = MQ5_get_value();
        printf("[MQ5] 烟雾传感器数值 = %d\n", data2);
        if (data2 > 10000)
            printf("[MQ5] 检测到燃气\n");
        else
            printf("[MQ5] 没有检测到燃气\n");

        adjust_led_brightness_by_light_sensor();
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
