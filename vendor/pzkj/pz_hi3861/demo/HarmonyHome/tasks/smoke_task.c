#include "smoke_task.h"
#include "template.h"

osThreadId_t SMOKE_Task_ID;

#define MAX_SMOKE 2000
#define MAX_MQ5 2000
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
        }
        else
        {
            beep_stop();
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
