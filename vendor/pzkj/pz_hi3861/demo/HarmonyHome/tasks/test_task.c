#include "test_task.h"
#include "template.h"

osThreadId_t Test_Task_ID;

void test_task(void)
{
    ina226_init();
    while (1)
    {
        printf("cur = %.2f\n", ina226_get_current());
        printf("bus_v = %.2f\n", ina226_get_bus_voltage());
        osDelay(500); // TASK_DELAY_5000MS
    }
}

void test_task_create(void)
{
    osThreadAttr_t taskOptions = {
        .name = "test_task",
        .attr_bits = 0,
        .cb_mem = NULL,
        .cb_size = 0,
        .stack_mem = NULL,
        .stack_size = 4096,
        .priority = osPriorityNormal2,
    };

    Test_Task_ID = osThreadNew((osThreadFunc_t)test_task, NULL, &taskOptions);
    if (Test_Task_ID != NULL)
    {
        printf("ID = %d, Test_Task_ID Create OK!\n", Test_Task_ID);
    }
}
